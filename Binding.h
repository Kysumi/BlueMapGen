#pragma once

#include <ChakraCommon.h>
#include <vector>
#include "Chakra.h"

class Binding
{
public:
	static Chakra* host;

	static void ProjectNativeClass(const wchar_t* className, JsNativeFunction constructor, JsValueRef& prototype, std::vector<const wchar_t*> memberNames, std::vector<JsNativeFunction> memberFuncs);
	static void BindNativeFunctions();
	
	static void setCallback(JsValueRef object, const wchar_t* propertyName, JsNativeFunction callback, void* callbackState);
	static void setProperty(JsValueRef object, const wchar_t* propertyName, JsValueRef property);

private:
	static JsValueRef getProperty(JsValueRef object, const wchar_t* propertyName);
	static JsValueRef CALLBACK JSLog(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState);
	static JsValueRef CALLBACK JSSetTimeout(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState);
	static JsValueRef CALLBACK JSSetInterval(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState);
	//static JsValueRef CALLBACK JSRender(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState);
	//static JsValueRef CALLBACK JSSetMouseClickCallback(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState);
};