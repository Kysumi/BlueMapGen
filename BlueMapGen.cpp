#include "ChakraCore.h"
#include <string>
#include <iostream>
#include <queue>

#include "Helper.h"

void CALLBACK PromiseContinuationCallback(JsValueRef task, void* callbackState)
{
    // Save promise task in taskQueue.
    std::queue<JsValueRef>* q = (std::queue<JsValueRef>*)callbackState;
    q->push(task);
    JsAddRef(task, nullptr);
}

void runPromiseSample()
{
	std::queue<JsValueRef> taskQueue;
    JsValueRef result;
    JsSetPromiseContinuationCallback(PromiseContinuationCallback, &taskQueue);
    JsRunScript(
        L"//The JavaScript ES6 Promise code goes here\n" \
        L"new Promise((resolve, reject) => resolve('basic:success'))" \
        L".then(() => {return 'second:success'});",
        JS_SOURCE_CONTEXT_NONE, L"", &result);

    JsValueRef global;
    JsGetGlobalObject(&global);

    // Execute promise tasks stored in taskQueue
    while (!taskQueue.empty()) {
        JsValueRef task = taskQueue.front();
        taskQueue.pop();
        JsCallFunction(task, &global, 1, &result);
        JsRelease(task, nullptr);
    }
}

int main()
{
    JsRuntimeHandle runtime;
    JsContextRef context;
    JsValueRef result;
    unsigned currentSourceContext = 0;

    // Your script; try replace hello-world with something else
    std::wstring script = Helper::getFileContents("tesast.js");

    // Create a runtime. 
    JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);

    // Create an execution context. 
    JsCreateContext(runtime, &context);

    // Now set the current execution context.
    JsSetCurrentContext(context);

    // Run the script.
    JsRunScript(script.c_str(), currentSourceContext++, L"", &result);

    // Convert your script result to String in JavaScript; redundant if your script returns a String
    JsValueRef resultJSString;
    JsConvertValueToString(result, &resultJSString);

    // Project script result back to C++.
    const wchar_t* resultWC;
    size_t stringLength;
    JsStringToPointer(resultJSString, &resultWC, &stringLength);

    std::wstring resultW(resultWC);
    std::cout << std::string(resultW.begin(), resultW.end()) << std::endl;

    // Dispose runtime
    JsSetCurrentContext(JS_INVALID_REFERENCE);
    JsDisposeRuntime(runtime);

    runPromiseSample();

    return 0;
}