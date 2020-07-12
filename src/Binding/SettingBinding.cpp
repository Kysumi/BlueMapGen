//
// Created by Scott on 12/07/2020.
//

#include "SettingBinding.h"
#include <src/ChakraCore/stdafx.h>
#include <src/Setting.h>

JsValueRef CALLBACK SettingBinding::GetRandomNumber(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                         unsigned short argumentCount, void *callbackState) {
    Assert(argumentCount == 3);

    int min, max;
    JsNumberToInt(arguments[1], &min);
    JsNumberToInt(arguments[2], &max);

    auto number = Setting::getRandomNumber(min, max);

    JsRef output = JS_INVALID_REFERENCE;
    JsIntToNumber(number, &output);

    return output;
}

void SettingBinding::bind() {
    JsValueRef globalObject;
    JsGetGlobalObject(&globalObject);

    JsValueRef jsWindow;
    JsCreateObject(&jsWindow);

    WScriptJsrt::setProperty(globalObject, L"Setting", jsWindow);
    WScriptJsrt::InstallObjectsOnObject(jsWindow, "getRandomNumber", GetRandomNumber);
}