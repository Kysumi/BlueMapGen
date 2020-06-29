//
// Created by Scott on 29/06/2020.
//

#include <src/ChakraCore/stdafx.h>
#include <ChakraCommon.h>
#include <src/WindowManager.h>
#include "Window.h"

JsValueRef CALLBACK binding::Window::Display(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                             unsigned short argumentCount, void *callbackState) {
    auto *window = WindowManager::getActiveWindow();

    window->display();

    return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK binding::Window::Clear(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                           unsigned short argumentCount, void *callbackState) {
    auto *window = WindowManager::getActiveWindow();

    window->clear(sf::Color::Blue);

    return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK binding::Window::ProcessEvents(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                                   unsigned short argumentCount, void *callbackState) {
    auto *window = WindowManager::getActiveWindow();

    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
    }

    return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK binding::Window::IsOpen(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                            unsigned short argumentCount, void *callbackState) {
    auto *window = WindowManager::getActiveWindow();

    JsValueRef output = JS_INVALID_REFERENCE;
    JsBoolToBoolean(window->isOpen(), &output);

    return output;
}

void binding::Window::bind() {
    JsValueRef globalObject;
    JsGetGlobalObject(&globalObject);

    JsValueRef jsWindow;
    JsCreateObject(&jsWindow);

    WScriptJsrt::setProperty(globalObject, L"Window", jsWindow);
    WScriptJsrt::InstallObjectsOnObject(jsWindow, "clear", Clear);
    WScriptJsrt::InstallObjectsOnObject(jsWindow, "display", Display);
    WScriptJsrt::InstallObjectsOnObject(jsWindow, "processEvents", ProcessEvents);
    WScriptJsrt::InstallObjectsOnObject(jsWindow, "isOpen", IsOpen);
}
