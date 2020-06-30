//
// Created by scott on 6/30/2020.
//

#ifndef ONGRID_NODE_H_BINDING
#define ONGRID_NODE_H_BINDING

#include <ChakraCommon.h>

namespace binding {
    class Node {
    public:
        static void bind();

        static JsValueRef JSGridPrototype;

        static JsValueRef
        JSGridConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                          void *callbackState);
    };
}


#endif //ONGRID_NODE_H
