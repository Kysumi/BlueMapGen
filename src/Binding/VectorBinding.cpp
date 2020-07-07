//
// Created by Scott on 1/07/2020.
//

#include <src/ChakraCore/stdafx.h>
#include <SFML/Window.hpp>
#include "VectorBinding.h"

JsValueRef VectorBinding::JSVectorPrototype;

// JsNativeFunction for Pointer constructor - Vector(x, y)
JsValueRef
CALLBACK VectorBinding::JSVectorConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                                     unsigned short argumentCount,
                                                     void *callbackState) {
    Assert(isConstructCall && argumentCount == 3);
    auto *output = JS_INVALID_REFERENCE;

    int x, y;
    JsNumberToInt(arguments[1], &x);
    JsNumberToInt(arguments[2], &y);

    auto *vec = new sf::Vector2i(x, y);

    JsCreateExternalObject(vec, nullptr, &output);
    JsSetPrototype(output, JSVectorPrototype);

    return output;
}

JsValueRef CALLBACK VectorBinding::X(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                              unsigned short argumentCount,
                                              void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *vectorArg;

    if (JsGetExternalData(arguments[0], &vectorArg) == JsNoError) {
        auto *vector = static_cast<sf::Vector2i *>(vectorArg);
        JsIntToNumber(vector->x, &output);
    };

    return output;
}

JsValueRef CALLBACK VectorBinding::Y(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                              unsigned short argumentCount,
                                              void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *vectorArg;

    if (JsGetExternalData(arguments[0], &vectorArg) == JsNoError) {
        auto *vector = static_cast<sf::Vector2i *>(vectorArg);
        JsIntToNumber(vector->y, &output);
    };

    return output;
}

void VectorBinding::bind() {
    std::vector<const char *> memberNames;
    std::vector<JsNativeFunction> memberFuncs;

    memberNames.push_back("x");
    memberFuncs.push_back(X);

    memberNames.push_back("y");
    memberFuncs.push_back(Y);

    WScriptJsrt::ProjectNativeClass(L"Vector", JSVectorConstructor, JSVectorPrototype, memberNames, memberFuncs);

}