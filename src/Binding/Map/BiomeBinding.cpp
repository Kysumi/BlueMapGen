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

JsValueRef CALLBACK BiomeBinding::GetName(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
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

JsValueRef CALLBACK BiomeBinding::SetName(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
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

void BiomeBinding::bind() {
    std::vector<const char *> memberNames;
    std::vector<JsNativeFunction> memberFuncs;

    memberNames.push_back("getName");
    memberFuncs.push_back(GetName);

    memberNames.push_back("setName");
    memberFuncs.push_back(SetName);

    WScriptJsrt::ProjectNativeClass(L"Biome", JSBiomeConstructor, JSBiomePrototype, memberNames, memberFuncs);
}
