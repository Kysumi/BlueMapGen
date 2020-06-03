#include "Console.h"

#include <ChakraCommon.h>
#include <ChakraCore.h>
#include <cwchar>

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

Console::Console()
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