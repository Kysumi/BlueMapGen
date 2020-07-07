//
// Created by scott on 6/30/2020.
//

#ifndef ONGRID_NODE_H_BINDING
#define ONGRID_NODE_H_BINDING

#include <ChakraCommon.h>

class NodeBinding {
public:
    static void bind();

    static JsValueRef JSNodePrototype;

    static JsValueRef
    JSNodeConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                      void *callbackState);

    // TODO remove. They should not draw nodes themselves
    static JsValueRef
    Draw(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
         void *callbackState);

    static JsValueRef
    Kill(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
         void *callbackState);

    static JsValueRef
    Born(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
         void *callbackState);
};


#endif //ONGRID_NODE_H
