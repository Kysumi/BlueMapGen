//
// Created by scott on 6/30/2020.
//

#include "Node.h"
#include <src/ChakraCore/stdafx.h>
#include <src/Map/Node.h>

JsValueRef binding::Node::JSGridPrototype;

JsValueRef
CALLBACK binding::Node::JSGridConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                          unsigned short argumentCount,
                                          void *callbackState) {
    Assert(isConstructCall && argumentCount == 4);
    auto *output = JS_INVALID_REFERENCE;

    int x, y, width;
    JsNumberToInt(arguments[1], &x);
    JsNumberToInt(arguments[2], &y);
    JsNumberToInt(arguments[3], &width);

    auto *node = new atlas::Node(sf::Vector2i(x, y), width);

    JsCreateExternalObject(node, nullptr, &output);
    JsSetPrototype(output, binding::Node::JSGridPrototype);

    return output;
}

void binding::Node::bind() {
    std::vector<const char *> memberNames;
    std::vector<JsNativeFunction> memberFuncs;

//    memberNames.push_back("process");
//    memberFuncs.push_back(Process);
//
//    memberNames.push_back("getNeighbours");
//    memberFuncs.push_back(GetNeighbours);
//
//    memberNames.push_back("draw");
//    memberFuncs.push_back(Draw);

    WScriptJsrt::ProjectNativeClass(L"Grid", JSGridConstructor, JSGridPrototype, memberNames, memberFuncs);
}
