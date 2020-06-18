#include <SFML/Graphics.hpp>
#include "WindowBinding.h"
#include <ChakraCommon.h>
#include "Binding.h"
#include "Setting.h"

JsValueRef CALLBACK JSDisplay(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
    auto* window = Setting::getActiveWindow();

    window->display();

    return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK JSClear(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
    auto* window = Setting::getActiveWindow();

    window->clear(sf::Color::Blue);

    return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK JSProcessEventQueue(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
    auto* window = Setting::getActiveWindow();

    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }
    }

    return JS_INVALID_REFERENCE;
}

JsValueRef CALLBACK JSIsOpen(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)
{
    auto* window = Setting::getActiveWindow();

    JsValueRef output = JS_INVALID_REFERENCE;
    JsBoolToBoolean(window->isOpen(), &output);

    return output;
}

void og::WindowBinding::bind()
{
    JsValueRef globalObject;
    JsGetGlobalObject(&globalObject);

    JsValueRef jsWindow;
    JsCreateObject(&jsWindow);

    Binding::setProperty(globalObject, L"Window", jsWindow);
    Binding::setCallback(jsWindow, L"clear", JSClear, nullptr);
    Binding::setCallback(jsWindow, L"display", JSDisplay, nullptr);
    Binding::setCallback(jsWindow, L"processEventQueue", JSProcessEventQueue, nullptr);
    Binding::setCallback(jsWindow, L"isOpen", JSIsOpen, nullptr);
}
