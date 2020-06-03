#include "ChakraCore.h"
#include <string>
#include <iostream>
#include <queue>


#include "Chakra.h"
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
    Chakra chakra;
    chakra.runScript("test.js");
    chakra.shutDown();

    // runPromiseSample();

    return 0;
}