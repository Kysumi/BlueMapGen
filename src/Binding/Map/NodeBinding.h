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

    static JsValueRef Alive(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                            void *callbackState);

    static JsValueRef
    X(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
      void *callbackState);

    static JsValueRef
    Y(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
      void *callbackState);

    static JsValueRef
    SetTexture(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
      void *callbackState);

    static JsValueRef GetBiome(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                        void *callbackState);

    static JsValueRef SetBiome(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                               void *callbackState);
};


#endif //ONGRID_NODE_H
