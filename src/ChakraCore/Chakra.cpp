#include "stdafx.h"
#include "Chakra.h"
#include <iostream>

unsigned int MessageBase::s_messageCount = 0;
JsRuntimeHandle Chakra::chRuntime = JS_INVALID_RUNTIME_HANDLE;
BOOL Chakra::doTTRecord = false;

JsRuntimeHandle Chakra::runtime;
JsContextRef Chakra::context;

JsRuntimeAttributes Chakra::jsrtAttributes = JsRuntimeAttributeNone;

HRESULT Chakra::RunScript(const char *fileName, LPCSTR fileContents, size_t fileLength,
                          JsFinalizeCallback fileContentsFinalizeCallback, JsValueRef bufferValue, char *fullPath,
                          JsValueRef parserStateCache) {
    HRESULT hr = S_OK;
    auto *messageQueue = new MessageQueue();
    WScriptJsrt::AddMessageQueue(messageQueue);

    IfJsErrorFailLogLabel(JsSetPromiseContinuationCallback(WScriptJsrt::PromiseContinuationCallback,
                                                           (void *) messageQueue), ErrorRunFinalize);

    Assert(fileContents != nullptr || bufferValue != nullptr);

    JsErrorCode runScript;
    JsValueRef fname;
    IfJsErrorFailLogLabel(JsCreateString(fullPath,
                                         strlen(fullPath), &fname), ErrorRunFinalize);

    runScript = WScriptJsrt::ModuleEntryPoint(fileName, fileContents, fullPath);

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
    // Required because of the macro using GOTO
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

// On success the param byteCodeBuffer will be allocated in the function.
HRESULT Chakra::GetSerializedBuffer(LPCSTR fileContents, JsFinalizeCallback fileContentFinalizeCallback,
                                    JsValueRef *byteCodeBuffer) {
    HRESULT hr = S_OK;

    JsValueRef scriptSource;
    IfJsErrorFailLog(JsCreateExternalArrayBuffer((void *) fileContents,
                                                 (unsigned int) strlen(fileContents), fileContentFinalizeCallback,
                                                 (void *) fileContents, &scriptSource));
    IfJsErrorFailLog(JsSerialize(scriptSource, byteCodeBuffer,
                                 JsParseScriptAttributeNone));

    Error:
    return hr;
}

HRESULT Chakra::CreateRuntime(JsRuntimeHandle *runtime) {
    HRESULT hr = E_FAIL;

#ifndef _WIN32
    // On Posix, malloc optimistically returns a non-null address without
    // checking if it's actually able to back that allocation in memory
    // Upon use of that address however, if the address space for that allocation
    // can't be committed, the process is killed
    // See the man page for malloc
    //
    // In order to avoid having to deal with this, we set the memory limit for the
    // runtime to the size of the physical memory on the system
    // TODO:
    // We could move the following into its own platform agnostic API
    // but in this case, this is a one-time call thats not applicable
    // on Windows so decided to leave as is
    // Additionally, we can probably do better than just limit to the physical memory
    // size

#if defined(__APPLE__) || defined(__linux__)
    size_t memoryLimit;
#ifdef __APPLE__
    int totalRamHW[] = { CTL_HW, HW_MEMSIZE };
    size_t length = sizeof(memoryLimit);
    if (sysctl(totalRamHW, 2, &memoryLimit, &length, NULL, 0) == -1)
    {
        memoryLimit = 0;
    }
#else
    struct sysinfo sysInfo;
    if (sysinfo(&sysInfo) == -1)
    {
        memoryLimit = 0;
    }
    else
    {
        memoryLimit = sysInfo.totalram;
    }
#endif // __APPLE__
#endif // __APPLE__ || __linux
#endif // !_WIN32

    IfJsErrorFailLog(JsCreateRuntime(jsrtAttributes, nullptr, runtime));

#ifndef _WIN32
    IfJsErrorFailLog(JsSetRuntimeMemoryLimit(*runtime, memoryLimit));
#endif

    hr = S_OK;
    Error:
    return hr;
}

HRESULT Chakra::CreateAndRunSerializedScript(const char *fileName, LPCSTR fileContents, size_t fileLength,
                                             JsFinalizeCallback fileContentsFinalizeCallback, char *fullPath) {
    HRESULT hr = S_OK;
    JsRuntimeHandle runtime = JS_INVALID_RUNTIME_HANDLE;
    JsContextRef context = JS_INVALID_REFERENCE, current = JS_INVALID_REFERENCE;
    JsValueRef bufferVal;

    // We don't want this to free fileContents when it completes, so the finalizeCallback is nullptr
    IfFailedGoLabel(GetSerializedBuffer(fileContents, nullptr, &bufferVal), ErrorRunFinalize);

    // Bytecode buffer is created in one runtime and will be executed on different runtime.

    IfFailedGoLabel(CreateRuntime(&runtime), ErrorRunFinalize);
    chRuntime = runtime;

    IfJsErrorFailLogLabel(JsCreateContext(runtime, &context), ErrorRunFinalize);
    IfJsErrorFailLogLabel(JsGetCurrentContext(&current), ErrorRunFinalize);
    IfJsErrorFailLogLabel(JsSetCurrentContext(context), ErrorRunFinalize);

    // Initialized the WScript object on the new context
    if (!WScriptJsrt::Initialize()) {
        IfFailedGoLabel(E_FAIL, ErrorRunFinalize);
    }

    // This is our last call to use fileContents, so pass in the finalizeCallback
    IfFailGo(RunScript(fileName, fileContents, fileLength, fileContentsFinalizeCallback, bufferVal, fullPath, nullptr));

    if (false) {
        ErrorRunFinalize:
        if (fileContentsFinalizeCallback != nullptr) {
            fileContentsFinalizeCallback((void *) fileContents);
        }
    }
    Error:
    if (current != JS_INVALID_REFERENCE) {
        JsSetCurrentContext(current);
    }

    if (runtime != JS_INVALID_RUNTIME_HANDLE) {
        JsDisposeRuntime(runtime);
    }

    return hr;
}

bool Chakra::RunScript(const std::string& filePath) {

    HRESULT hr = S_OK;
    LPCSTR fileContents = nullptr;
    JsRuntimeHandle runtime = JS_INVALID_RUNTIME_HANDLE;
    UINT lengthBytes = 0;

    LPCOLESTR contentsRaw = nullptr;

    char fullPath[_MAX_PATH];
    size_t len = 0;


    const char *fileName = filePath.c_str();

    hr = Helpers::LoadScriptFromFile(fileName, fileContents, &lengthBytes);
    contentsRaw;
    lengthBytes; // Unused for now.

//    IfFailGo(hr);
    if (HostConfigFlags::flags.GenerateLibraryByteCodeHeaderIsEnabled) {
        jsrtAttributes = (JsRuntimeAttributes) (jsrtAttributes | JsRuntimeAttributeSerializeLibraryByteCode);
    }

    // Spin up bindings
    if (!WScriptJsrt::Initialize()) {

        std::cout << "Failed to Initialize Chakra!" << std::endl;
//        IfFailGo(E_FAIL);
    }

    if (_fullpath(fullPath, fileName, _MAX_PATH) == nullptr) {
//        IfFailGo(E_FAIL);
        std::cout << "File path could not be found" << std::endl;
    }

    if (HostConfigFlags::flags.TrackRejectedPromises) {
        JsSetHostPromiseRejectionTracker(WScriptJsrt::PromiseRejectionTrackerCallback, nullptr);
    }

    len = strlen(fullPath);
    if (HostConfigFlags::flags.GenerateLibraryByteCodeHeaderIsEnabled) {

        if (HostConfigFlags::flags.GenerateLibraryByteCodeHeader != nullptr) {
            if (wcslen(HostConfigFlags::flags.GenerateLibraryByteCodeHeader) == 0) {
                HostConfigFlags::flags.GenerateLibraryByteCodeHeader = nullptr;
            }
        }
        CHAR libraryName[_MAX_PATH];
        CHAR ext[_MAX_EXT];
        _splitpath_s(fullPath, NULL, 0, NULL, 0, libraryName, _countof(libraryName), ext, _countof(ext));

//        IfFailGo(CreateLibraryByteCodeHeader(fileContents, WScriptJsrt::FinalizeFree, lengthBytes, HostConfigFlags::flags.GenerateLibraryByteCodeHeader, libraryName));
        CreateLibraryByteCodeHeader(fileContents, WScriptJsrt::FinalizeFree, lengthBytes,
                                    HostConfigFlags::flags.GenerateLibraryByteCodeHeader, libraryName);
    } else if (HostConfigFlags::flags.SerializedIsEnabled) {
        CreateAndRunSerializedScript(fileName, fileContents, lengthBytes, WScriptJsrt::FinalizeFree, fullPath);
    } else if (HostConfigFlags::flags.GenerateParserStateCacheIsEnabled) {
        CreateParserState(fileContents, lengthBytes, WScriptJsrt::FinalizeFree, nullptr);
    } else if (HostConfigFlags::flags.UseParserStateCacheIsEnabled) {
        CreateParserStateAndRunScript(fileName, fileContents, lengthBytes, WScriptJsrt::FinalizeFree, fullPath);
    } else {
//        IfFailGo(RunScript(fileName, fileContents, lengthBytes, WScriptJsrt::FinalizeFree, nullptr, fullPath, nullptr));
        RunScript(fileName, fileContents, lengthBytes, WScriptJsrt::FinalizeFree, nullptr, fullPath, nullptr);
    }

    return false;
}

HRESULT GetParserStateBuffer(LPCSTR fileContents, JsFinalizeCallback fileContentsFinalizeCallback,
                             JsValueRef *parserStateBuffer) {
    HRESULT hr = S_OK;
    JsValueRef scriptSource = nullptr;

    IfJsErrorFailLog(JsCreateExternalArrayBuffer((void *) fileContents,
                                                 (unsigned int) strlen(fileContents), fileContentsFinalizeCallback,
                                                 (void *) fileContents, &scriptSource));

    IfJsErrorFailLog(JsSerializeParserState(scriptSource, parserStateBuffer, JsParseScriptAttributeNone));

    Error:
    return hr;
}

HANDLE GetFileHandle(LPCWSTR filename) {
    if (filename != nullptr) {
        // TODO find out what this should be
//        return CreateFile(filename, GENERIC_WRITE, FILE_SHARE_DELETE,
//                          nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    }
    return GetStdHandle(STD_OUTPUT_HANDLE);
}

HRESULT
Chakra::CreateParserState(LPCSTR fileContents, size_t fileLength, JsFinalizeCallback fileContentsFinalizeCallback,
                          LPCWSTR fullPath) {
    HRESULT hr = S_OK;
    HANDLE fileHandle = nullptr;
    JsValueRef parserStateBuffer = nullptr;
    BYTE *buffer = nullptr;
    unsigned int bufferSize = 0;

    IfFailedGoLabel(GetParserStateBuffer(fileContents, fileContentsFinalizeCallback, &parserStateBuffer), Error);
    IfJsErrorFailLog(JsGetArrayBufferStorage(parserStateBuffer, &buffer, &bufferSize));

    fileHandle = GetFileHandle(fullPath);
    IfFalseGo(fileHandle != INVALID_HANDLE_VALUE && fileHandle != nullptr);

    for (unsigned int i = 0; i < bufferSize; i++) {
        const unsigned int BYTES_PER_LINE = 32;
        DWORD written = 0;
        char scratch[3];
        auto scratchLen = sizeof(scratch);
        int num = _snprintf_s(scratch, scratchLen, _countof(scratch), "%02X", buffer[i]);
        Assert(num == 2);
        IfFalseGo(WriteFile(fileHandle, scratch, (DWORD) (scratchLen - 1), &written, nullptr));

        // Add line breaks so this block can be readable
        if (i % BYTES_PER_LINE == (BYTES_PER_LINE - 1) && i < bufferSize - 1) {
            IfFalseGo(WriteFile(fileHandle, "\n", 1, &written, nullptr));
        }
    }

    Error:
    if (fileHandle != nullptr) {
        CloseHandle(fileHandle);
    }
    return hr;
}

HRESULT Chakra::GetParserStateBuffer(LPCSTR fileContents, JsFinalizeCallback fileContentsFinalizeCallback,
                                     JsValueRef *parserStateBuffer) {
    HRESULT hr = S_OK;
    JsValueRef scriptSource = nullptr;

    IfJsErrorFailLog(JsCreateExternalArrayBuffer((void *) fileContents,
                                                 (unsigned int) strlen(fileContents), fileContentsFinalizeCallback,
                                                 (void *) fileContents, &scriptSource));

    IfJsErrorFailLog(JsSerializeParserState(scriptSource, parserStateBuffer, JsParseScriptAttributeNone));

    Error:
    return hr;
}

HRESULT Chakra::CreateParserStateAndRunScript(const char *fileName, LPCSTR fileContents, size_t fileLength,
                                              JsFinalizeCallback fileContentsFinalizeCallback, char *fullPath) {
    HRESULT hr = S_OK;
    JsRuntimeHandle runtime = JS_INVALID_RUNTIME_HANDLE;
    JsContextRef context = JS_INVALID_REFERENCE, current = JS_INVALID_REFERENCE;
    JsValueRef bufferVal;

    // We don't want this to free fileContents when it completes, so the finalizeCallback is nullptr
    IfFailedGoLabel(GetParserStateBuffer(fileContents, nullptr, &bufferVal), ErrorRunFinalize);

    // Bytecode buffer is created in one runtime and will be executed on different runtime.
    IfFailedGoLabel(CreateRuntime(&runtime), ErrorRunFinalize);
    chRuntime = runtime;

    IfJsErrorFailLogLabel(JsCreateContext(runtime, &context), ErrorRunFinalize);
    IfJsErrorFailLogLabel(JsGetCurrentContext(&current), ErrorRunFinalize);
    IfJsErrorFailLogLabel(JsSetCurrentContext(context), ErrorRunFinalize);

    // Initialized the WScript object on the new context
    if (!WScriptJsrt::Initialize()) {
        IfFailedGoLabel(E_FAIL, ErrorRunFinalize);
    }

    // This is our last call to use fileContents, so pass in the finalizeCallback
    IfFailGo(RunScript(fileName, fileContents, fileLength, fileContentsFinalizeCallback, nullptr, fullPath, bufferVal));

    if (false) {
        ErrorRunFinalize:
        if (fileContentsFinalizeCallback != nullptr) {
            fileContentsFinalizeCallback((void *) fileContents);
        }
    }
    Error:
    if (current != JS_INVALID_REFERENCE) {
        JsSetCurrentContext(current);
    }

    if (runtime != JS_INVALID_RUNTIME_HANDLE) {
        JsDisposeRuntime(runtime);
    }

    return hr;
}

HRESULT Chakra::CreateLibraryByteCodeHeader(LPCSTR contentsRaw, JsFinalizeCallback contentsRawFinalizeCallback,
                                            DWORD lengthBytes, LPCWSTR bcFullPath, LPCSTR libraryNameNarrow) {
    HANDLE bcFileHandle = nullptr;
    JsValueRef bufferVal;
    BYTE *bcBuffer = nullptr;
    unsigned int bcBufferSize = 0;
    DWORD written;
    // For validating the header file against the library file
    auto outputStr =
            "//-------------------------------------------------------------------------------------------------------\n"
            "// Copyright (C) Microsoft. All rights reserved.\n"
            "// Licensed under the MIT license. See LICENSE.txt file in the project root for full license information.\n"
            "//-------------------------------------------------------------------------------------------------------\n"
            "#if 0\n";

    std::string normalizedContentStr;
    char *nextToken = nullptr;
    char *token = strtok_s((char *) contentsRaw, "\r", &nextToken);
    while (token) {
        normalizedContentStr.append(token);
        token = strtok_s(nullptr, "\r", &nextToken);
    }
    // We no longer need contentsRaw, so call the finalizer for it if one was provided
    if (contentsRawFinalizeCallback != nullptr) {
        contentsRawFinalizeCallback((void *) contentsRaw);
    }

    const char *normalizedContent = normalizedContentStr.c_str();
    // We still need contentsRaw after this, so pass a null finalizeCallback into it
    HRESULT hr = GetSerializedBuffer(normalizedContent, nullptr, &bufferVal);

    IfFailedGoLabel((hr), ErrorRunFinalize);

    IfJsrtErrorHRLabel(JsGetArrayBufferStorage(bufferVal, &bcBuffer, &bcBufferSize), ErrorRunFinalize);

    bcFileHandle = GetFileHandle(bcFullPath);
    IfFalseGo(bcFileHandle != INVALID_HANDLE_VALUE && bcFileHandle != nullptr);

    IfFalseGoLabel(WriteFile(bcFileHandle, outputStr, (DWORD) strlen(outputStr), &written, nullptr), ErrorRunFinalize);
    IfFalseGoLabel(WriteFile(bcFileHandle, normalizedContent, (DWORD) normalizedContentStr.size(), &written, nullptr),
                   ErrorRunFinalize);
    outputStr = "\n#endif\n";

    IfFalseGo(WriteFile(bcFileHandle, outputStr, (DWORD) strlen(outputStr), &written, nullptr));

    // Write out the bytecode
    outputStr = "namespace Js\n{\n    const char Library_Bytecode_";
    IfFalseGo(WriteFile(bcFileHandle, outputStr, (DWORD) strlen(outputStr), &written, nullptr));
    IfFalseGo(WriteFile(bcFileHandle, libraryNameNarrow, (DWORD) strlen(libraryNameNarrow), &written, nullptr));
    outputStr = "[] = {\n/* 00000000 */";
    IfFalseGo(WriteFile(bcFileHandle, outputStr, (DWORD) strlen(outputStr), &written, nullptr));

    for (unsigned int i = 0; i < bcBufferSize; i++) {
        char scratch[6];
        auto scratchLen = sizeof(scratch);
        int num = _snprintf_s(scratch, scratchLen, _countof(scratch), " 0x%02X", bcBuffer[i]);
        Assert(num == 5);
        IfFalseGo(WriteFile(bcFileHandle, scratch, (DWORD) (scratchLen - 1), &written, nullptr));

        // Add a comma and a space if this is not the last item
        if (i < bcBufferSize - 1) {
            char commaSpace[2];
            _snprintf_s(commaSpace, sizeof(commaSpace), _countof(commaSpace),
                        ",");  // close quote, new line, offset and open quote
            IfFalseGo(WriteFile(bcFileHandle, commaSpace, (DWORD) strlen(commaSpace), &written, nullptr));
        }

        // Add a line break every 16 scratches, primarily so the compiler doesn't complain about the string being too long.
        // Also, won't add for the last scratch
        if (i % 16 == 15 && i < bcBufferSize - 1) {
            char offset[17];
            int actualLen = _snprintf_s(offset, sizeof(offset), _countof(offset), "\n/* %08X */",
                                        i + 1);  // close quote, new line, offset and open quote
            IfFalseGo(WriteFile(bcFileHandle, offset, actualLen, &written, nullptr));
        }
    }
    outputStr = "};\n\n";
    IfFalseGo(WriteFile(bcFileHandle, outputStr, (DWORD) strlen(outputStr), &written, nullptr));

    outputStr = "}\n";
    IfFalseGo(WriteFile(bcFileHandle, outputStr, (DWORD) strlen(outputStr), &written, nullptr));

    ErrorRunFinalize:
    Error:
    if (bcFileHandle != nullptr) {
        CloseHandle(bcFileHandle);
    }

    return hr;
}