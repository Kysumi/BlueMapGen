#include "Binding.h"
#include <cassert>
#include "Task.h"

Chakra* Binding::host = new Chakra;

// add all native bindings
void Binding::BindNativeFunctions() {
	// project general methods - console.log, setTimeout, setInterval
	JsValueRef globalObject;
	JsGetGlobalObject(&globalObject);
	JsValueRef console;
	JsCreateObject(&console);
	
	setProperty(globalObject, L"console", console);
	setCallback(console, L"log", JSLog, nullptr);
	setCallback(globalObject, L"setTimeout", JSSetTimeout, nullptr);
	setCallback(globalObject, L"setInterval", JSSetInterval, nullptr);
}

void Binding::setCallback(JsValueRef object, const wchar_t* propertyName, JsNativeFunction callback, void* callbackState)
{
	JsPropertyIdRef propertyId;
	JsGetPropertyIdFromName(propertyName, &propertyId);
	JsValueRef function;
	JsCreateFunction(callback, callbackState, &function);
	JsSetProperty(object, propertyId, function, true);
}

void Binding::setProperty(JsValueRef object, const wchar_t* propertyName, JsValueRef property)
{
	JsPropertyIdRef propertyId;
	JsGetPropertyIdFromName(propertyName, &propertyId);
	JsSetProperty(object, propertyId, property, true);
}

JsValueRef Binding::getProperty(JsValueRef object, const wchar_t* propertyName)
{
	JsValueRef output;
	JsPropertyIdRef propertyId;
	JsGetPropertyIdFromName(propertyName, &propertyId);
	JsGetProperty(object, propertyId, &output);
	return output;
}

// JsNativeFunction for console.log(expr)
JsValueRef CALLBACK Binding::JSLog(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
	for (unsigned int index = 1; index < argumentCount; index++)
	{
		if (index > 1)
		{
			wprintf(L" ");
		}
		JsValueRef stringValue;
		JsConvertValueToString(arguments[index], &stringValue);
		const wchar_t* string;
		size_t length;
		JsStringToPointer(stringValue, &string, &length);
		wprintf(L"%s", string);
	}
	wprintf(L"\n");
	return JS_INVALID_REFERENCE;
}

// JsNativeFunction for setTimeout(func, delay)
JsValueRef CALLBACK Binding::JSSetTimeout(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
	assert(!isConstructCall && argumentCount == 3);
	JsValueRef func = arguments[1];
	int delay = 0;
	
	JsNumberToInt(arguments[2], &delay);

	host->pushTask(new Task(func, delay, arguments[0], JS_INVALID_REFERENCE));

	return JS_INVALID_REFERENCE;
}

// JsNativeFunction for setInterval(func, delay)
JsValueRef CALLBACK Binding::JSSetInterval(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
	assert(!isConstructCall && argumentCount == 3);
	JsValueRef func = arguments[1];
	int delay = 0;
	JsNumberToInt(arguments[2], &delay);
	
	host->pushTask(new Task(func, delay, arguments[0], JS_INVALID_REFERENCE, true));
	
	return JS_INVALID_REFERENCE;
}

// project a custom native class and its member functions to JS
// there must be a one-to-one mapping between elements in memberNames and memberFunc
void Binding::ProjectNativeClass(const wchar_t* className, JsNativeFunction constructor, JsValueRef& prototype, std::vector<const wchar_t*> memberNames, std::vector<JsNativeFunction> memberFuncs){
	// project constructor to global scope 
	JsValueRef globalObject;
	JsGetGlobalObject(&globalObject);

	JsValueRef jsConstructor;
	JsCreateFunction(constructor, nullptr, &jsConstructor);
	setProperty(globalObject, className, jsConstructor);

	// create class's prototype and project its member functions
	JsCreateObject(&prototype);
	assert(memberNames.size() == memberNames.size());

	for (int i = 0; i < memberNames.size(); ++i) {
		setCallback(prototype, memberNames[i], memberFuncs[i], nullptr);
	}
	
	setProperty(jsConstructor, L"prototype", prototype);
}