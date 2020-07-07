//
// Created by scott on 6/29/2020.
//

#include <SFML/System.hpp>
#include <src/ChakraCore/stdafx.h>
#include <src/WindowManager.h>
#include <src/Binding/GridBinding.h>
#include <src/Map/Grid.h>
#include "NodeBinding.h"
#include "VectorBinding.h"

JsValueRef GridBinding::JSGridPrototype;

// JsNativeFunction for Pointer constructor - Grid(x, y)
JsValueRef
CALLBACK GridBinding::JSGridConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                        unsigned short argumentCount,
                                        void *callbackState) {
    Assert(isConstructCall && argumentCount == 3);
    auto *output = JS_INVALID_REFERENCE;

    int x, y;
    JsNumberToInt(arguments[1], &x);
    JsNumberToInt(arguments[2], &y);

    auto *grid = new Grid(sf::Vector2i(x, y));

    JsCreateExternalObject(grid, WScriptJsrt::FinalizeFree, &output);
    JsSetPrototype(output, JSGridPrototype);

    return output;
}

JsValueRef CALLBACK GridBinding::Process(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                         unsigned short argumentCount, void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *gridArg;

    if (JsGetExternalData(arguments[0], &gridArg) == JsNoError) {
        auto *grid = static_cast<Grid *>(gridArg);
        grid->Process();
    };

    return output;
}

JsValueRef
CALLBACK GridBinding::GetNeighbours(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                    unsigned short argumentCount,
                                    void *callbackState) {
    Assert(!isConstructCall && argumentCount == 3);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *gridArg;
    Grid *grid;

    // Load grid object from JSLAND
    if (JsGetExternalData(arguments[0], &gridArg) == JsNoError) {
        grid = static_cast<Grid *>(gridArg);
    } else {
        throw "AHHHHHH no grid object";
    }

    int x, y;
    JsNumberToInt(arguments[1], &x);
    JsNumberToInt(arguments[2], &y);

    auto neighbours = grid->getNeighbours(x, y);
    JsCreateExternalObject(&neighbours, nullptr, &output);

    return output;
}

JsValueRef CALLBACK GridBinding::Draw(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                      unsigned short argumentCount,
                                      void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *gridArg;

    if (JsGetExternalData(arguments[0], &gridArg) == JsNoError) {
        auto *grid = static_cast<Grid *>(gridArg);
        grid->Draw(*WindowManager::getActiveWindow());
    };

    return output;
}

JsValueRef CALLBACK GridBinding::GetNodeFromGridPosition(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                                         unsigned short argumentCount,
                                                         void *callbackState) {
    Assert(!isConstructCall && argumentCount == 3);
    JsValueRef output = JS_INVALID_REFERENCE;

    int x, y;
    JsNumberToInt(arguments[1], &x);
    JsNumberToInt(arguments[2], &y);

    void *gridArg;

    if (JsGetExternalData(arguments[0], &gridArg) == JsNoError) {
        auto *grid = static_cast<Grid *>(gridArg);
        auto node = grid->getNodeFromGridPosition(x, y);
        JsCreateExternalObject(node, nullptr, &output);
        JsSetPrototype(output, NodeBinding::JSNodePrototype);
    };

    return output;
}

JsValueRef CALLBACK GridBinding::GetSize(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                         unsigned short argumentCount,
                                         void *callbackState) {
    Assert(!isConstructCall && argumentCount == 1);
    JsValueRef output = JS_INVALID_REFERENCE;

    void *gridArg;

    if (JsGetExternalData(arguments[0], &gridArg) == JsNoError) {
        auto *grid = static_cast<Grid *>(gridArg);
        auto vector = grid->getSize();
        // WScriptJsrt::FinalizeFree
        JsCreateExternalObject(vector, nullptr, &output);
        JsSetPrototype(output, VectorBinding::JSVectorPrototype);
    };

    return output;
}

void GridBinding::bind() {
    std::vector<const char *> memberNames;
    std::vector<JsNativeFunction> memberFuncs;

    memberNames.push_back("process");
    memberFuncs.push_back(Process);

    memberNames.push_back("getNeighbours");
    memberFuncs.push_back(GetNeighbours);

    memberNames.push_back("draw");
    memberFuncs.push_back(Draw);

    memberNames.push_back("getNodeFromGridPosition");
    memberFuncs.push_back(GetNodeFromGridPosition);

    memberNames.push_back("getSize");
    memberFuncs.push_back(GetSize);

    WScriptJsrt::ProjectNativeClass(L"Grid", JSGridConstructor, JSGridPrototype, memberNames, memberFuncs);
}
