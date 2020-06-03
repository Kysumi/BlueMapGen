#include "Chakra.h"

#include <ChakraCore.h>


#include "Helper.h"

Chakra::Chakra()
{
    // Create a runtime. 
    JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);

    // Create an execution context. 
    JsCreateContext(runtime, &context);

    // Now set the current execution context.
    JsSetCurrentContext(context);

    consoleLog();
}

void Chakra::runScript(std::string fileName)
{
    std::wstring script = Helper::getFileContents(fileName.c_str());
	
    JsRunScript(script.c_str(), currentSourceContext++, L"", &result);

    // Convert your script result to String in JavaScript; redundant if your script returns a String
    JsValueRef resultJSString;
    JsConvertValueToString(result, &resultJSString);

    // Project script result back to C++.
    const wchar_t* resultWC;
    // size_t stringLength;
    // JsStringToPointer(resultJSString, &resultWC, &stringLength);
    //
    // std::wstring resultW(resultWC);
    // std::cout << std::string(resultW.begin(), resultW.end()) << std::endl;
	
}

void Chakra::shutDown()
{
    // Dispose runtime
    JsSetCurrentContext(JS_INVALID_REFERENCE);
    JsDisposeRuntime(runtime);
}


JsValueRef CALLBACK happyConsole(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
    for (unsigned int index = 1; index < argumentCount; index++)
    {
        if (index > 1)
        {
            wprintf(L" ");
        }

        JsValueRef stringValue;
        JsConvertValueToString(arguments[index], &stringValue), L"invalid argument";

        const wchar_t* string;
        size_t length;
        JsStringToPointer(stringValue, &string, &length), L"invalid argument";

        wprintf(L"%s", string);
    }

    wprintf(L"\n");

    return JS_INVALID_REFERENCE;
}

void Chakra::consoleLog()
{  
    JsValueRef console, logFunc, global;
    JsPropertyIdRef consolePropId, logPropId;
    const char* logString = "log";
    const char* consoleString = "console";
	
    // create console object, log function, and set log function as property of console
    JsCreateObject(&console);
    JsCreateFunction(happyConsole, nullptr, &logFunc);
    JsCreatePropertyId(logString, strlen(logString), &logPropId);
    JsSetProperty(console, logPropId, logFunc, true);
	
    // set console as property of global object
    JsGetGlobalObject(&global);
    JsCreatePropertyId(consoleString, strlen(consoleString), &consolePropId);
    JsSetProperty(global, consolePropId, console, true);
}