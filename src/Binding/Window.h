//
// Created by Scott on 29/06/2020.
//

#ifndef ONGRID_WINDOW_H
#define ONGRID_WINDOW_H

namespace binding {
    class Window {
    public:
        static void bind();

    private:

        static JsValueRef
        Display(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                void *callbackState);

        static JsValueRef
        Clear(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
              void *callbackState);

        static JsValueRef
        ProcessEvents(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                      void *callbackState);

        static JsValueRef
        IsOpen(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
               void *callbackState);

        static JsValueRef
        Describe(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount,
                 void *callbackState);
    };
}

#endif //ONGRID_WINDOW_H
