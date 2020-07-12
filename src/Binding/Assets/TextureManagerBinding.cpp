//
// Created by Scott on 12/07/2020.
//

#include "TextureManagerBinding.h"
#include "src/Assets/TextureManager.h"
#include <src/ChakraCore/stdafx.h>
#include <iostream>

JsValueRef TextureManagerBinding::LoadFromFile(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                        unsigned short argumentCount, void *callbackState) {
    Assert(argumentCount == 3);

    auto *output = JS_INVALID_REFERENCE;

    // TODO move to single place
    auto filePath = WScriptJsrt::JSStringToStdString(arguments[1]);
    auto name = WScriptJsrt::JSStringToStdString(arguments[2]);

    auto result = TextureManager::loadFromFile(filePath, name);
    JsBoolToBoolean(result, &output);

    return output;
}

void TextureManagerBinding::bind() {
    JsValueRef globalObject;
    JsGetGlobalObject(&globalObject);

    JsValueRef jsTextureManager;
    JsCreateObject(&jsTextureManager);

    WScriptJsrt::setProperty(globalObject, L"TextureManager", jsTextureManager);
    WScriptJsrt::InstallObjectsOnObject(jsTextureManager, "loadFromFile", LoadFromFile);
}
