//
// Created by scott on 6/30/2020.
//

#include "Node.h"
#include <src/ChakraCore/stdafx.h>
#include <src/Map/Node.h>
#include <src/WindowManager.h>

JsValueRef binding::Node::JSNodePrototype;

JsValueRef
CALLBACK binding::Node::JSNodeConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
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
    JsSetPrototype(output, binding::Node::JSNodePrototype);

    return output;
}

JsValueRef CALLBACK binding::Node::Draw(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                           unsigned short argumentCount, void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *nodeArg;

    if (JsGetExternalData(arguments[0], &nodeArg) == JsNoError) {
        auto *node = static_cast<atlas::Node *>(nodeArg);
        node->draw(*WindowManager::getActiveWindow());
    };

    return output;
}

JsValueRef CALLBACK binding::Node::Kill(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                        unsigned short argumentCount, void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *nodeArg;

    if (JsGetExternalData(arguments[0], &nodeArg) == JsNoError) {
        auto *node = static_cast<atlas::Node *>(nodeArg);
        node->kill();
    };

    return output;
}

JsValueRef CALLBACK binding::Node::Born(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                        unsigned short argumentCount, void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *nodeArg;

    if (JsGetExternalData(arguments[0], &nodeArg) == JsNoError) {
        auto *node = static_cast<atlas::Node *>(nodeArg);
        node->born();
    };

    return output;
}

void binding::Node::bind() {
    std::vector<const char *> memberNames;
    std::vector<JsNativeFunction> memberFuncs;

    memberNames.push_back("draw");
    memberFuncs.push_back(Draw);

    memberNames.push_back("born");
    memberFuncs.push_back(Born);

    memberNames.push_back("kill");
    memberFuncs.push_back(Kill);

    WScriptJsrt::ProjectNativeClass(L"Node", JSNodeConstructor, JSNodePrototype, memberNames, memberFuncs);
}
