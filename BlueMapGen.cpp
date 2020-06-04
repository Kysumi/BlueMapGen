#include <SFML/Graphics.hpp>
#include "Chakra.h"

//https://github.com/microsoft/Chakra-Samples/blob/master/ChakraCore%20Samples/JSRT%20Hosting%20Samples/C%2B%2B/ChakraCoreHost/ChakraCoreHost.cpp
//https://github.com/microsoft/ChakraCore/wiki/JavaScript-Runtime-%28JSRT%29-Overview

// Task queue example
// https://github.com/Microsoft/Chakra-Samples/tree/master/ChakraCore%20Samples/OpenGL%20Engine/OpenGLEngine

int main()
{
    Chakra chakra;
    chakra.runScript("test.js");

    // create the window

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
	
    chakra.shutDown();

    return 0;
}