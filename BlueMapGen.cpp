#include <iostream>
#include <SFML/Graphics.hpp>

#include "Binding.h"
#include "Chakra.h"
#include "Grid.h"
#include "Setting.h"

//https://github.com/microsoft/Chakra-Samples/blob/master/ChakraCore%20Samples/JSRT%20Hosting%20Samples/C%2B%2B/ChakraCoreHost/ChakraCoreHost.cpp
//https://github.com/microsoft/ChakraCore/wiki/JavaScript-Runtime-%28JSRT%29-Overview

// Task queue example
// https://github.com/Microsoft/Chakra-Samples/tree/master/ChakraCore%20Samples/OpenGL%20Engine/OpenGLEngine

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

void bindWindowFunctions()
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

int main()
{
	// Bind Chakra to helper functions
    Binding::BindNativeFunctions();

    bindWindowFunctions();
	
    Grid::BindToJS();
	
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Blue Map Generation");
    window.setFramerateLimit(60);

    sf::Clock clock;

    Setting::setActiveWindow(&window);
    Binding::host->runScript("scripts/process.js");
  
    Binding::host->shutDown();

    return 0;
}