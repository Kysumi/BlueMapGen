#include "stdafx.h"
#include "Chakra.h"
#include <iostream>

JsRuntimeHandle Chakra::chRuntime = JS_INVALID_RUNTIME_HANDLE;
UINT32 Chakra::startEventCount = 1;
BOOL Chakra::doTTRecord = false;
size_t Chakra::ttUriLength = 0;
JsRuntimeAttributes Chakra::jsrtAttributes = JsRuntimeAttributeNone;

HRESULT Chakra::RunBgParseSync(LPCSTR fileContents, UINT lengthBytes, const char *fileName) {
    JsValueRef scriptSource;
    JsErrorCode e = (JsCreateExternalArrayBuffer((void *) fileContents,
                                                 (unsigned int) lengthBytes,
                                                 nullptr, (void *) fileContents, &scriptSource));

    // What's the preferred way of doing this?
    WCHAR fileNameWide[MAX_PATH] = {0};
    size_t fileNameLength = strlen(fileName);
    for (size_t i = 0; i < fileNameLength; i++) {
        fileNameWide[i] = fileName[i];
    }

    JsScriptContents scriptContents = {0};
    scriptContents.container = (LPVOID) fileContents;
    scriptContents.containerType = JsScriptContainerType::HeapAllocatedBuffer;
    scriptContents.encodingType = JsScriptEncodingType::Utf8;
    scriptContents.contentLengthInBytes = lengthBytes;
    scriptContents.fullPath = fileNameWide;

    DWORD cookie = 0;
    e = JsQueueBackgroundParse_Experimental(&scriptContents, &cookie);
    Assert(e == JsErrorCode::JsNoError);

    JsValueRef bgResult = nullptr;
    e = JsExecuteBackgroundParse_Experimental(
            cookie,
            scriptSource,
            WScriptJsrt::GetNextSourceContext(),
            (WCHAR *) scriptContents.fullPath,
            JsParseScriptAttributes::JsParseScriptAttributeNone,
            nullptr,//_In_ JsValueRef parserState,
            &bgResult
    );

    return e;
}

bool CHAKRA_CALLBACK Chakra::DummyJsSerializedScriptLoadUtf8Source(
        JsSourceContext sourceContext,
        JsValueRef *scriptBuffer,
        JsParseScriptAttributes *parseAttributes) {
    auto *serializedCallbackInfo = reinterpret_cast<SerializedCallbackInfo *>(sourceContext);

    Assert(!serializedCallbackInfo->freeingHandled);

    serializedCallbackInfo->freeingHandled = true;
    size_t length = strlen(reinterpret_cast<const char *>(serializedCallbackInfo->scriptBody));

    // sourceContext is source ptr, see RunScript below
    if (JsCreateExternalArrayBuffer(serializedCallbackInfo->scriptBody, (unsigned int) length,
                                    serializedCallbackInfo->scriptBodyFinalizeCallback,
                                    serializedCallbackInfo->scriptBody, scriptBuffer) != JsNoError) {
        return false;
    }

    *parseAttributes = JsParseScriptAttributeNone;
    return true;
}

HRESULT Chakra::RunScript(const char *fileName, LPCSTR fileContents, size_t fileLength,
                          JsFinalizeCallback fileContentsFinalizeCallback, JsValueRef bufferValue, char *fullPath,
                          JsValueRef parserStateCache) {
    HRESULT hr = S_OK;
    auto *messageQueue = new MessageQueue();
    WScriptJsrt::AddMessageQueue(messageQueue);

    IfJsErrorFailLogLabel(JsSetPromiseContinuationCallback(WScriptJsrt::PromiseContinuationCallback,
                                                           (void *) messageQueue), ErrorRunFinalize);

    if (strlen(fileName) >= 14 && strcmp(fileName + strlen(fileName) - 14, "ttdSentinal.js") == 0) {
        if (fileContentsFinalizeCallback != nullptr) {
            fileContentsFinalizeCallback((void *) fileContents);
        }
#if !ENABLE_TTD
            wprintf(_u("Sential js file is only ok when in TTDebug mode!!!\n"));
            return E_FAIL;
#else
//        if (!doTTReplay) {
//            wprintf(_u("Sential js file is only ok when in TTReplay mode!!!\n"));
//            return E_FAIL;
//        }

        IfFailedReturn(JsTTDStart());

        try {
            JsTTDMoveMode moveMode = JsTTDMoveMode::JsTTDMoveKthEvent;
            int64_t snapEventTime = -1;
            int64_t nextEventTime = -2;

            while (true) {
                JsErrorCode error = JsTTDGetSnapTimeTopLevelEventMove(chRuntime, moveMode,
                                                                      startEventCount,
                                                                      &nextEventTime, &snapEventTime,
                                                                      nullptr);

                if (error != JsNoError) {
                    if (error == JsErrorCategoryUsage) {
                        wprintf(_u("Start time not in log range.\n"));
                    }

                    return error;
                }

                IfFailedReturn(
                        JsTTDMoveToTopLevelEvent(chRuntime, moveMode, snapEventTime, nextEventTime));

                JsErrorCode res = JsTTDReplayExecution(&moveMode, &nextEventTime);

                //handle any uncaught exception by immediately time-traveling to the throwing line in the debugger -- in replay just report and exit
                if (res == JsErrorCategoryScript) {
                    wprintf(_u("An unhandled script exception occurred!!!\n"));

                    ExitProcess(0);
                }

                if (nextEventTime == -1) {
                    wprintf(_u("\nReached end of Execution -- Exiting.\n"));
                    break;
                }
            }
        }
        catch (...) {
            wprintf(_u("Terminal exception in Replay -- exiting.\n"));
            ExitProcess(0);
        }
#endif
    } else {
        Assert(fileContents != nullptr || bufferValue != nullptr);

        JsErrorCode runScript;
        JsValueRef fname;
        IfJsErrorFailLogLabel(JsCreateString(fullPath,
                                             strlen(fullPath), &fname), ErrorRunFinalize);

        if (bufferValue != nullptr) {
            if (fileContents == nullptr) {
                // if we have no fileContents, no worry about freeing them, and the call is simple.
                runScript = JsRunSerialized(
                        bufferValue,
                        nullptr /*JsSerializedLoadScriptCallback*/,
                        0 /*SourceContext*/,
                        fname,
                        nullptr /*result*/
                );
            } else // fileContents != nullptr
            {
                // Memory management is a little more complex here
                SerializedCallbackInfo serializedCallbackInfo;
                serializedCallbackInfo.scriptBody = (void *) fileContents;
                serializedCallbackInfo.scriptBodyFinalizeCallback = fileContentsFinalizeCallback;
                serializedCallbackInfo.freeingHandled = false;

                // Now we can run our script, with this serializedCallbackInfo as the sourcecontext
                runScript = JsRunSerialized(
                        bufferValue,
                        DummyJsSerializedScriptLoadUtf8Source,
                        reinterpret_cast<JsSourceContext>(&serializedCallbackInfo),
                        // Use source ptr as sourceContext
                        fname,
                        nullptr /*result*/);
                // Now that we're down here, we can free the fileContents if they weren't sent into
                // a GC-managed object.
                if (!serializedCallbackInfo.freeingHandled) {
                    if (fileContentsFinalizeCallback != nullptr) {
                        fileContentsFinalizeCallback((void *) fileContents);
                    }
                }
            }
        } else if (parserStateCache != nullptr) {
            JsValueRef scriptSource;
            IfJsErrorFailLog(JsCreateExternalArrayBuffer((void *) fileContents,
                                                         (unsigned int) fileLength,
                                                         fileContentsFinalizeCallback,
                                                         (void *) fileContents, &scriptSource));

            // TODO: Support TTD
            runScript = JsRunScriptWithParserState(
                    scriptSource,
                    WScriptJsrt::GetNextSourceContext(),
                    fname,
                    JsParseScriptAttributeNone,
                    parserStateCache,
                    nullptr);
        } else if (HostConfigFlags::flags.Module) {
            runScript = WScriptJsrt::ModuleEntryPoint(fileName, fileContents, fullPath);
        } else if (HostConfigFlags::flags.ExecuteWithBgParse && !HostConfigFlags::flags.DebugLaunch) {
            unsigned int lengthBytes = (unsigned int) fileLength;
            runScript = (JsErrorCode) RunBgParseSync(fileContents, lengthBytes, fileName);
        } else // bufferValue == nullptr && parserStateCache == nullptr
        {
            JsValueRef scriptSource;
            IfJsErrorFailLog(JsCreateExternalArrayBuffer((void *) fileContents,
                                                         (unsigned int) fileLength,
                                                         fileContentsFinalizeCallback,
                                                         (void *) fileContents, &scriptSource));

#if ENABLE_TTD
            if (doTTRecord) {
                JsPropertyIdRef ttProperty = nullptr;
                JsValueRef ttString = nullptr;
                JsValueRef global = nullptr;

                IfFailedReturn(JsCreatePropertyId("ttdLogURI", strlen("ttdLogURI"), &ttProperty));
                IfFailedReturn(JsCreateString(ttUri, ttUriLength, &ttString));
                IfFailedReturn(JsGetGlobalObject(&global));

                IfFailedReturn(JsSetProperty(global, ttProperty, ttString, false));

                IfFailedReturn(JsTTDStart());
            }

            auto sourceContext = WScriptJsrt::GetNextSourceContext();
            WScriptJsrt::RegisterScriptDir(sourceContext, fullPath);
            runScript = JsRun(scriptSource,
                              sourceContext, fname,
                              JsParseScriptAttributeNone, nullptr /*result*/);
            if (runScript == JsErrorCategoryUsage) {
                wprintf(_u("FATAL ERROR: Core was compiled without ENABLE_TTD is defined. CH is trying to use TTD interface\n"));
                abort();
            }
#else
            runScript = JsRun(scriptSource,
                                                 WScriptJsrt::GetNextSourceContext(), fname,
                                                 JsParseScriptAttributeNone,
                                                 nullptr /*result*/);
#endif
        }

        //Do a yield after the main script body executes
        JsTTDNotifyYield();

        if (runScript != JsNoError) {
            WScriptJsrt::PrintException(fileName, runScript);
        } else {
            // Repeatedly flush the message queue until it's empty. It is necessary to loop on this
            // because setTimeout can add scripts to execute.
            do {
                IfFailGo(messageQueue->ProcessAll(fileName));
            } while (!messageQueue->IsEmpty());
        }
    }

    if (false) {
        ErrorRunFinalize:
        if (fileContentsFinalizeCallback != nullptr) {
            fileContentsFinalizeCallback((void *) fileContents);
        }
    }
    Error:
#if ENABLE_TTD
    if (doTTRecord) {
        IfFailedReturn(JsTTDStop());
    }
#endif

    if (messageQueue != nullptr) {
        messageQueue->RemoveAll();
        // clean up possible pinned exception object on exit to avoid potential leak
        bool hasException;
        if (JsHasException(&hasException) == JsNoError && hasException) {
            JsValueRef exception = JS_INVALID_REFERENCE;
            JsGetAndClearException(&exception);
        }
        delete messageQueue;
    }

    // We only call RunScript() once, safe to Uninitialize()
    WScriptJsrt::Uninitialize();

    return hr;
}

bool Chakra::runScripty() {

    HRESULT hr = S_OK;
    LPCSTR fileContents = nullptr;
    JsRuntimeHandle runtime = JS_INVALID_RUNTIME_HANDLE;
    UINT lengthBytes = 0;

    LPCOLESTR contentsRaw = nullptr;

    char fullPath[_MAX_PATH];
    size_t len = 0;


    const char* fileName = "test.js";

    hr = Helpers::LoadScriptFromFile(fileName, fileContents, &lengthBytes);
    contentsRaw; lengthBytes; // Unused for now.

//    IfFailGo(hr);
    if (HostConfigFlags::flags.GenerateLibraryByteCodeHeaderIsEnabled)
    {
        jsrtAttributes = (JsRuntimeAttributes)(jsrtAttributes | JsRuntimeAttributeSerializeLibraryByteCode);
    }

    // Spin up bindings
    if (!WScriptJsrt::Initialize())
    {

        std::cout << "AHHHHHH" << std::endl;
//        IfFailGo(E_FAIL);
    }

    if (_fullpath(fullPath, fileName, _MAX_PATH) == nullptr)
    {
        IfFailGo(E_FAIL);
    }

    if (HostConfigFlags::flags.TrackRejectedPromises)
    {
        JsSetHostPromiseRejectionTracker(WScriptJsrt::PromiseRejectionTrackerCallback, nullptr);
    }

    len = strlen(fullPath);
    if (HostConfigFlags::flags.GenerateLibraryByteCodeHeaderIsEnabled)
    {

        if (HostConfigFlags::flags.GenerateLibraryByteCodeHeader != nullptr)
        {
            if (wcslen(HostConfigFlags::flags.GenerateLibraryByteCodeHeader) == 0)
            {
                HostConfigFlags::flags.GenerateLibraryByteCodeHeader = nullptr;
            }
        }
        CHAR libraryName[_MAX_PATH];
        CHAR ext[_MAX_EXT];
        _splitpath_s(fullPath, NULL, 0, NULL, 0, libraryName, _countof(libraryName), ext, _countof(ext));

        IfFailGo(CreateLibraryByteCodeHeader(fileContents, WScriptJsrt::FinalizeFree, lengthBytes, HostConfigFlags::flags.GenerateLibraryByteCodeHeader, libraryName));
    }
    else if (HostConfigFlags::flags.SerializedIsEnabled)
    {
        CreateAndRunSerializedScript(fileName, fileContents, lengthBytes, WScriptJsrt::FinalizeFree, fullPath);
    }
    else if (HostConfigFlags::flags.GenerateParserStateCacheIsEnabled)
    {
        CreateParserState(fileContents, lengthBytes, WScriptJsrt::FinalizeFree, nullptr);
    }
    else if (HostConfigFlags::flags.UseParserStateCacheIsEnabled)
    {
        CreateParserStateAndRunScript(fileName, fileContents, lengthBytes, WScriptJsrt::FinalizeFree, fullPath);
    }
    else
    {
        IfFailGo(RunScript(fileName, fileContents, lengthBytes, WScriptJsrt::FinalizeFree, nullptr, fullPath, nullptr));
    }

    return false;
}
