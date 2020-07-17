//
// Created by Scott on 14/07/2020.
//

#ifndef ONGRID_BIOMEBINDING_H
#define ONGRID_BIOMEBINDING_H

#include <src/ChakraCore/stdafx.h>

class BiomeBinding {
public:
    static void bind();

    static JsValueRef JSBiomePrototype;
private:
    static JsValueRef
    CALLBACK JSBiomeConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                unsigned short argumentCount,
                                void *callbackState);

    static JsValueRef
    GetName(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
            void *callbackState);

    static JsValueRef
    SetName(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
            void *callbackState);
};


#endif //ONGRID_BIOMEBINDING_H
