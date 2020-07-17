//
// Created by Scott on 29/06/2020.
//

#include <src/ChakraCore/stdafx.h>
#include <ChakraCommon.h>
#include <src/WindowManager.h>
#include <iostream>
#include "WindowBinding.h"

JsValueRef CALLBACK WindowBinding::Display(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                                    unsigned short argumentCount, void *callbackState) {
    auto *window = WindowManager::getActiveWindow();

    window->display();

    return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK WindowBinding::Clear(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                                  unsigned short argumentCount, void *callbackState) {
    auto *window = WindowManager::getActiveWindow();

    window->clear(sf::Color::White);

    return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK WindowBinding::ProcessEvents(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
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

JsValueRef CALLBACK WindowBinding::IsOpen(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                                   unsigned short argumentCount, void *callbackState) {
    auto *window = WindowManager::getActiveWindow();

    JsValueRef output = JS_INVALID_REFERENCE;
    JsBoolToBoolean(window->isOpen(), &output);

    return output;
}

// TODO decide if i should go ahead with this
JsValueRef CALLBACK WindowBinding::Describe(JsValueRef callee, bool isConstructCall, JsValueRef *arguments,
                                                     unsigned short argumentCount, void *callbackState) {

    std::cout << "Window Definition:" << std::endl;
    std::cout << "clear : Clears the window to the default color" << std::endl;
    std::cout << "display : Renders the content that has been drawn to the window" << std::endl;
    std::cout << "processEvents : Processes all of the input events, this includes key presses and mouse movement"
              << std::endl;
    std::cout << "isOpen : Returns true or false depending if the window is open" << std::endl;
    std::cout << "describe : This output" << std::endl;

    return JS_INVALID_REFERENCE;
}

void WindowBinding::bind() {
    JsValueRef globalObject;
    JsGetGlobalObject(&globalObject);

    JsValueRef jsWindow;
    JsCreateObject(&jsWindow);

    WScriptJsrt::setProperty(globalObject, L"Window", jsWindow);
    WScriptJsrt::InstallObjectsOnObject(jsWindow, "clear", Clear);
    WScriptJsrt::InstallObjectsOnObject(jsWindow, "display", Display);
    WScriptJsrt::InstallObjectsOnObject(jsWindow, "processEvents", ProcessEvents);
    WScriptJsrt::InstallObjectsOnObject(jsWindow, "isOpen", IsOpen);
    WScriptJsrt::InstallObjectsOnObject(jsWindow, "describe", Describe);
}
