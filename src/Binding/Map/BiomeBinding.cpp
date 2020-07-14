//
// Created by Scott on 14/07/2020.
//

#include <src/Map/Biome.h>
#include "BiomeBinding.h"

JsValueRef BiomeBinding::JSBiomePrototype;

JsValueRef BiomeBinding::JSBiomeConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                            unsigned short argumentCount, void *callbackState) {
    Assert(isConstructCall && argumentCount == 1);
    auto *output = JS_INVALID_REFERENCE;

    auto *biome = new Biome();

    JsCreateExternalObject(biome, nullptr, &output);
    JsSetPrototype(output, JSBiomePrototype);

    return output;
}

JsValueRef CALLBACK BiomeBinding::getName(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                       unsigned short argumentCount, void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *nodeArg;

    if (JsGetExternalData(arguments[0], &nodeArg) == JsNoError) {
        auto *biome = static_cast<Biome *>(nodeArg);

        JsCreateString(biome->name.c_str(), biome->name.size(), &output);
    };

    return output;
}

JsValueRef CALLBACK BiomeBinding::setName(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                          unsigned short argumentCount, void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *nodeArg;

    if (JsGetExternalData(arguments[0], &nodeArg) == JsNoError) {
        auto *biome = static_cast<Biome *>(nodeArg);

        JsCreateString(biome->name.c_str(), biome->name.size(), &output);
    };

    return output;
}