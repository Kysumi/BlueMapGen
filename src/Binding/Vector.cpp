//
// Created by Scott on 1/07/2020.
//

#include <src/ChakraCore/stdafx.h>
#include <SFML/Window.hpp>
#include "Vector.h"

JsValueRef binding::Vector::JSVectorPrototype;

// JsNativeFunction for Pointer constructor - Vector(x, y)
JsValueRef
CALLBACK binding::Vector::JSVectorConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                          unsigned short argumentCount,
                                          void *callbackState) {
    Assert(isConstructCall && argumentCount == 3);
    auto *output = JS_INVALID_REFERENCE;

    int x, y;
    JsNumberToInt(arguments[1], &x);
    JsNumberToInt(arguments[2], &y);

    auto *grid = new sf::Vector2i(x, y);

    JsCreateExternalObject(grid, nullptr, &output);
    JsSetPrototype(output, binding::Vector::JSVectorPrototype);

    return output;
}

JsValueRef CALLBACK binding::Vector::X(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                           unsigned short argumentCount,
                                           void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *vectorArg;

    if (JsGetExternalData(arguments[0], &vectorArg) == JsNoError) {
        auto *vector = static_cast<sf::Vector2f *>(vectorArg);
        JsIntToNumber(vector->x, &output);
    };

    return output;
}

void binding::Vector::bind() {
    std::vector<const char *> memberNames;
    std::vector<JsNativeFunction> memberFuncs;

    memberNames.push_back("x");
    memberFuncs.push_back(X);

    WScriptJsrt::ProjectNativeClass(L"Vector", JSVectorConstructor, JSVectorPrototype, memberNames, memberFuncs);

}