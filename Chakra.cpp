#include "Chakra.h"
#include <queue>
#include <iostream>
#include "Helper.h"

void CALLBACK PromiseContinuationCallback(JsValueRef task, void* callbackState)
{
    // Save promises in taskQueue.
    JsValueRef global;
    JsGetGlobalObject(&global);
    std::queue<Task*>* q = (std::queue<Task*>*)callbackState;
    q->push(new Task(task, 0, global, JS_INVALID_REFERENCE));
}

Chakra::Chakra()
{
    // Create a runtime. 
    JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime);

    // Create an execution context. 
    JsCreateContext(runtime, &context);

    // Now set the current execution context.
    JsSetCurrentContext(context);

    // Set up ES6 Promise 
    if (JsSetPromiseContinuationCallback(PromiseContinuationCallback, &taskQueue) != JsNoError)
    {
		throw "failed to set PromiseContinuationCallback.";
    }       
}

void Chakra::runScript(std::string fileName)
{
    const auto script = Helper::getFileContents(fileName.c_str());

    JsValueRef result;
	
    try
    {
	    if (JsRunScript(script.c_str(), currentSourceContext++, L"", &result))
	    {
	        // Get error message
	        JsValueRef exception;
	        if (JsGetAndClearException(&exception) != JsNoError)
	        {
	            std::wcout << L"failed to get and clear exception" << std::endl;
	        }

	        JsPropertyIdRef messageName;
	        if (JsGetPropertyIdFromName(L"message", &messageName) != JsNoError)
	        {
	            std::wcout << L"failed to get error message id" << std::endl;
	        }


	        JsValueRef messageValue;
	        if (JsGetProperty(exception, messageName, &messageValue))
	        {
	            std::wcout << L"failed to get error message" << std::endl;
	        }

	        const wchar_t* message;
	        size_t length;

	        if (JsStringToPointer(messageValue, &message, &length) != JsNoError)
	        {
	            std::wcout << L"failed to convert error message" << std::endl;
	        }

	        std::wcout << message << std::endl;

			return;
	    }

	    // Execute tasks stored in taskQueue
	    while (!taskQueue.empty()) {
	        auto task = taskQueue.front();
	        taskQueue.pop();
	        int currentTime = clock() / (double)(CLOCKS_PER_SEC / 1000);
	        if (currentTime - task->_time > task->_delay) {
	            task->invoke();
	            if (task->_repeat) {
	                task->_time = currentTime;
	                taskQueue.push(task);
	            }
	            else {
	                delete task;
	            }
	        }
	        else {
	            taskQueue.push(task);
	        }
	    }

	    // Convert your script result to String in JavaScript; redundant if your script returns a String
	    JsValueRef resultJSString;
	    JsConvertValueToString(result, &resultJSString);

	}
    catch (...)
    {
		std::cout << L"chakrahost: fatal error: internal error.\n" << std::endl;
    }

    // Project script result back to C++.
    // const wchar_t* resultWC;
	
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

void Chakra::pushTask(Task* task)
{
	taskQueue.push(task);
}
