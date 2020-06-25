//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft Corporation and contributors. All rights reserved.
// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "stdafx.h"

void RuntimeThreadLocalData::Initialize(RuntimeThreadData* threadData)
{
    this->threadData = threadData;
}

void RuntimeThreadLocalData::Uninitialize()
{
}

THREAD_LOCAL RuntimeThreadLocalData threadLocalData;

RuntimeThreadLocalData& GetRuntimeThreadLocalData()
{
    return threadLocalData;
}

RuntimeThreadData::RuntimeThreadData() :
    hSemaphore(nullptr), 
    hThread(nullptr),
    sharedContent(nullptr),
    receiveBroadcastCallbackFunc(nullptr),
    runtime(nullptr),
    context(nullptr),
    parent(nullptr),
    leaving(false)
{
    this->hevntInitialScriptCompleted = CreateEvent(NULL, TRUE, FALSE, NULL);
    this->hevntReceivedBroadcast = CreateEvent(NULL, FALSE, FALSE, NULL);
    this->hevntShutdown = CreateEvent(NULL, TRUE, FALSE, NULL);

    InitializeCriticalSection(&csReportQ);
}

RuntimeThreadData::~RuntimeThreadData()
{
    CloseHandle(this->hevntInitialScriptCompleted);
    CloseHandle(this->hevntReceivedBroadcast);
    CloseHandle(this->hevntShutdown);
    CloseHandle(this->hThread);
    DeleteCriticalSection(&csReportQ);
}

DWORD RuntimeThreadData::ThreadProc()
{
    JsValueRef scriptSource;
    JsValueRef fname;
    const char* fullPath = "agent source";
    HRESULT hr = S_OK;

    threadLocalData.Initialize(this);

    IfJsErrorFailLog(JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));
    IfJsErrorFailLog(JsCreateContext(runtime, &context));
    IfJsErrorFailLog(JsSetCurrentContext(context));


    if (!WScriptJsrt::Initialize())
    {
        IfFailGo(E_FAIL);
    }


    IfJsErrorFailLog(JsCreateExternalArrayBuffer((void*)this->initialSource.c_str(),
        (unsigned int)this->initialSource.size(), nullptr, nullptr, &scriptSource));


    JsCreateString(fullPath, strlen(fullPath), &fname);

    JsRun(scriptSource, WScriptJsrt::GetNextSourceContext(), fname, JsParseScriptAttributeNone, nullptr);

    SetEvent(this->parent->hevntInitialScriptCompleted);

    // loop waiting for work;

    while (true)
    {
        HANDLE handles[] = { this->hevntReceivedBroadcast, this->hevntShutdown };
        DWORD waitRet = WaitForMultipleObjects(_countof(handles), handles, false, INFINITE);

        if (waitRet == WAIT_OBJECT_0)
        {
            JsValueRef args[3];
            JsGetGlobalObject(&args[0]);
            JsCreateSharedArrayBufferWithSharedContent(this->parent->sharedContent, &args[1]);
            JsDoubleToNumber(1, &args[2]);

            // notify the parent we received the data
            ReleaseSemaphore(this->parent->hSemaphore, 1, NULL);

            if (this->receiveBroadcastCallbackFunc)
            {
                JsCallFunction(this->receiveBroadcastCallbackFunc, args, 3, nullptr);
            }
        }

        if (waitRet == WAIT_OBJECT_0 + 1 || this->leaving)
        {
            WScriptJsrt::Uninitialize();

            if (this->receiveBroadcastCallbackFunc)
            {
                JsRelease(this->receiveBroadcastCallbackFunc, nullptr);
            }
            JsSetCurrentContext(nullptr);
            JsDisposeRuntime(runtime);

            threadLocalData.Uninitialize();
            return 0;
        }
        else if (waitRet != WAIT_OBJECT_0)
        {
            Assert(false);
            break;
        }
    }

Error:

    JsSetCurrentContext(nullptr);
    JsDisposeRuntime(runtime);
    threadLocalData.Uninitialize();
    return 0;
}
