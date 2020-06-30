//
// Created by scott on 6/29/2020.
//

#ifndef ONGRID_GRID_H_BINDING
#define ONGRID_GRID_H_BINDING

#include <ChakraCommon.h>

namespace binding {
    class Grid {
    public:
        static void bind();

        static JsValueRef JSGridPrototype;

    private:
        static JsValueRef
        CALLBACK Process(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                         void *callbackState);

        static JsValueRef
        JSGridConstructor(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                          void *callbackState);

        static JsValueRef
        GetNeighbours(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                      void *callbackState);

        static JsValueRef
        Draw(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
             void *callbackState);

        static JsValueRef
        Describe(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                 void *callbackState);
    };
}

#endif //ONGRID_GRID_H
