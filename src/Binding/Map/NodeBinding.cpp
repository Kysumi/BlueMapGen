//
// Created by scott on 6/30/2020.
//

#include "NodeBinding.h"
#include <src/ChakraCore/stdafx.h>
#include <src/Map/Node.h>
#include <src/WindowManager.h>

JsValueRef NodeBinding::JSNodePrototype;

JsValueRef
CALLBACK NodeBinding::JSNodeConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                                 unsigned short argumentCount,
                                                 void *callbackState) {
    Assert(isConstructCall && argumentCount == 4);
    auto *output = JS_INVALID_REFERENCE;

    int x, y, width;
    JsNumberToInt(arguments[1], &x);
    JsNumberToInt(arguments[2], &y);
    JsNumberToInt(arguments[3], &width);

    auto *node = new Node(sf::Vector2i(x, y), width, 0, 0);

    JsCreateExternalObject(node, nullptr, &output);
    JsSetPrototype(output, JSNodePrototype);

    return output;
}

JsValueRef CALLBACK NodeBinding::Alive(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                              unsigned short argumentCount, void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *nodeArg;

    if (JsGetExternalData(arguments[0], &nodeArg) == JsNoError) {
        auto *node = static_cast<Node *>(nodeArg);

        JsBoolToBoolean(node->isAlive(), &output);
    };

    return output;
}

JsValueRef CALLBACK NodeBinding::X(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                       unsigned short argumentCount, void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *nodeArg;

    if (JsGetExternalData(arguments[0], &nodeArg) == JsNoError) {
        auto *node = static_cast<Node *>(nodeArg);

        JsIntToNumber(node->getGridPositionX(), &output);
    };

    return output;
}

JsValueRef CALLBACK NodeBinding::Y(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                   unsigned short argumentCount, void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *nodeArg;

    if (JsGetExternalData(arguments[0], &nodeArg) == JsNoError) {
        auto *node = static_cast<Node *>(nodeArg);

        JsIntToNumber(node->getGridPositionX(), &output);
    };

    return output;
}

JsValueRef
NodeBinding::SetTexture(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                        void *callbackState) {

    Assert(!isConstructCall && argumentCount == 2);

    JsValueRef output = JS_INVALID_REFERENCE;

    auto textureName = WScriptJsrt::JSStringToStdString(arguments[1]);

    void *nodeArg;

    if (JsGetExternalData(arguments[0], &nodeArg) == JsNoError) {
        auto *node = static_cast<Node *>(nodeArg);
        node->setTexture(textureName);
    };

    return output;
}

void NodeBinding::bind() {
    std::vector<const char *> memberNames;
    std::vector<JsNativeFunction> memberFuncs;

    memberNames.push_back("alive");
    memberFuncs.push_back(Alive);

    memberNames.push_back("x");
    memberFuncs.push_back(X);

    memberNames.push_back("y");
    memberFuncs.push_back(Y);

    memberNames.push_back("setTexture");
    memberFuncs.push_back(SetTexture);

    WScriptJsrt::ProjectNativeClass(L"Node", JSNodeConstructor, JSNodePrototype, memberNames, memberFuncs);
}
