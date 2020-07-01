//
// Created by Scott on 1/07/2020.
//

#ifndef ONGRID_VECTOR_H_BINDING
#define ONGRID_VECTOR_H_BINDING

#include <ChakraCommon.h>

namespace binding {
    class Vector {
    public:
        static void bind();

        static JsValueRef JSVectorPrototype;

    private:
        static JsValueRef
        X(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
          void *callbackState);

        static JsValueRef
        JSVectorConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                            unsigned short argumentCount,
                            void *callbackState);
    };
}

#endif //ONGRID_VECTOR_H
