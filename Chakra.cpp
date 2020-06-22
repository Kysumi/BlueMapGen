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

struct ModuleResponseData
{
	ModuleResponseData()
		: mainModule(JS_INVALID_REFERENCE), childModule(JS_INVALID_REFERENCE), mainModuleException(JS_INVALID_REFERENCE), mainModuleReady(false)
	{
	}
	JsModuleRecord mainModule;
	JsModuleRecord childModule;
	JsValueRef mainModuleException;
	bool mainModuleReady;
};

ModuleResponseData successTest;

static JsErrorCode CALLBACK Success_FIMC(_In_ JsModuleRecord referencingModule, _In_ JsValueRef specifier, _Outptr_result_maybenull_ JsModuleRecord* dependentModuleRecord)
{
	JsModuleRecord moduleRecord = JS_INVALID_REFERENCE;
	LPCWSTR specifierStr;
	size_t length;

	JsErrorCode errorCode = JsStringToPointer(specifier, &specifierStr, &length);

	if (errorCode != JsNoError)
	{
		throw "Ummm something wrong with a module import";
	}
	
	errorCode = JsInitializeModuleRecord(referencingModule, specifier, &moduleRecord);

	if (errorCode != JsNoError)
	{
		throw "Hit error trying to initialize a module record";
	}
	
	*dependentModuleRecord = moduleRecord;
	successTest.childModule = moduleRecord;
	
	return JsNoError;
}

static JsErrorCode CALLBACK DynamicImport(_In_ JsModuleRecord referencingModule, _In_ JsValueRef specifier, _Outptr_result_maybenull_ JsModuleRecord* dependentModuleRecord)
{
	// TODO
	return JsNoError;
}

static JsErrorCode CALLBACK Success_NMRC(_In_opt_ JsModuleRecord referencingModule, _In_opt_ JsValueRef exceptionVar)
{
	if (successTest.mainModule == referencingModule)
	{
		successTest.mainModuleReady = true;
		successTest.mainModuleException = exceptionVar;
	}
	return JsNoError;
}

std::vector< char > wstring_convert_to_bytes(const wchar_t* str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	std::string string = converter.to_bytes(str);
	return std::vector< char >(string.begin(), string.end());
}

void outputError(JsValueRef errorObject)
{
	JsPropertyIdRef messageName;
	if (JsGetPropertyIdFromName(L"message", &messageName) != JsNoError)
	{
		std::wcout << L"failed to get error message id" << std::endl;
	}

	JsValueRef messageValue;
	if (JsGetProperty(errorObject, messageName, &messageValue))
	{
		std::wcout << L"failed to get error message" << std::endl;
		return;
	}

	const wchar_t* message;
	size_t length;

	if (JsStringToPointer(messageValue, &message, &length) != JsNoError)
	{
		std::wcout << L"failed to convert error message" << std::endl;
	}

	std::wcout << message << std::endl;
}

std::string convert(std::wstring string)
{
	auto cString = string.c_str();
	int size = WideCharToMultiByte(CP_UTF8, 0, cString, -1, NULL, 0, NULL, NULL);

	char* buffer = new char[size + 1];
	WideCharToMultiByte(CP_UTF8, 0, cString, -1, buffer, size, NULL, NULL);

	std::string str(buffer);
	delete[]buffer;

	return str;
}

void Chakra::runScript(std::string fileName)
{
    const auto script = Helper::getFileContents(fileName.c_str());

	////////////////////////////////

	JsModuleRecord requestModule = JS_INVALID_REFERENCE;
	JsValueRef specifier;

	JsPointerToString(L"", 1, &specifier);
	JsInitializeModuleRecord(nullptr, specifier, &requestModule);
	successTest.mainModule = requestModule;
	JsSetModuleHostInfo(requestModule, JsModuleHostInfo_FetchImportedModuleCallback, Success_FIMC);
	JsSetModuleHostInfo(requestModule, JsModuleHostInfo_FetchImportedModuleFromScriptCallback, DynamicImport);
	JsSetModuleHostInfo(requestModule, JsModuleHostInfo_NotifyModuleReadyCallback, Success_NMRC);

	JsValueRef errorObject = JS_INVALID_REFERENCE;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;

	auto temp = convert(script);
	auto tempa = temp.c_str();
	auto tempar = (LPBYTE)tempa;
	
	JsErrorCode errorCode = JsParseModuleSource(requestModule, currentSourceContext++, (LPBYTE)tempa,
		(unsigned int)strlen(tempa), JsParseModuleSourceFlags_DataIsUTF8, &errorObject);

	if (errorObject != JS_INVALID_REFERENCE)
	{
		outputError(errorObject);		
	}

	if (successTest.mainModuleException != JS_INVALID_REFERENCE)
	{
		outputError(successTest.mainModuleException);
	}


	JsValueRef result;
	bool hasError = false;

	JsModuleEvaluation(successTest.mainModule, &result);
	// JsHasException(&hasError);
	//
	// if (hasError)
	// {
	// 	JsGetAndClearException(&errorObject);
	// 	outputError(errorObject);
	// }
	//
	////////////////////////////////////////////
	
    // JsValueRef result;
	
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

			outputError(exception);

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
