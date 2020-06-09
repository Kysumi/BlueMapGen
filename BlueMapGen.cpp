#include <SFML/Graphics.hpp>

#include "Binding.h"
#include "Chakra.h"
#include "Grid.h"

//https://github.com/microsoft/Chakra-Samples/blob/master/ChakraCore%20Samples/JSRT%20Hosting%20Samples/C%2B%2B/ChakraCoreHost/ChakraCoreHost.cpp
//https://github.com/microsoft/ChakraCore/wiki/JavaScript-Runtime-%28JSRT%29-Overview

// Task queue example
// https://github.com/Microsoft/Chakra-Samples/tree/master/ChakraCore%20Samples/OpenGL%20Engine/OpenGLEngine

int main()
{
	// Bind Chakra to helper functions
    Binding::BindNativeFunctions();
    Binding::host->runScript("test.js");
	
	// Create grid
    Grid grid(sf::Vector2i(32, 32));
	
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setFramerateLimit(60);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }                
        }

        auto time = clock.getElapsedTime();
    	if (time.asSeconds() > 1)
    	{
            grid.Process();
            clock.restart();
    	}

        window.clear(sf::Color::Blue);
        grid.Draw(window);
        window.display();
    }

    //Binding::host->shutDown();

    return 0;
}