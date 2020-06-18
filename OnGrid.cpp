#include <iostream>
#include <SFML/Graphics.hpp>

#include "Binding.h"
#include "Chakra.h"
#include "Grid.h"
#include "Setting.h"
#include "WindowBinding.h"

//https://github.com/microsoft/Chakra-Samples/blob/master/ChakraCore%20Samples/JSRT%20Hosting%20Samples/C%2B%2B/ChakraCoreHost/ChakraCoreHost.cpp
//https://github.com/microsoft/ChakraCore/wiki/JavaScript-Runtime-%28JSRT%29-Overview

// Task queue example
// https://github.com/Microsoft/Chakra-Samples/tree/master/ChakraCore%20Samples/OpenGL%20Engine/OpenGLEngine


int main()
{
	// Bind Chakra to helper functions
    Binding::BindNativeFunctions();

	// Binding functions to JS
    og::WindowBinding::bind();
    Grid::BindToJS();
	
    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "On Grid");
    window.setFramerateLimit(60);
    Setting::setActiveWindow(&window);

    // Pass over to JS
	Binding::host->runScript("scripts/process.js");
  
    Binding::host->shutDown();

    return 0;
}