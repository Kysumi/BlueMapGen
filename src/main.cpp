//
// Created by Scott on 25/06/2020.
//
#include "ChakraCore/stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "ChakraCore/Chakra.h"

int main()
{
    std::cout << "Starting up OnGrid..." << std::endl;
    Chakra::init();
    Chakra::RunScript("assets/scripts/test.js");

    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // run the program as long as the window is open
//    while (window.isOpen())
//    {
//        // check all the window's events that were triggered since the last iteration of the loop
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            // "close requested" event: we close the window
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        // clear the window with black color
//        window.clear(sf::Color::Black);
//
//        // draw everything here...
//        // window.draw(...);
//
//        // end the current frame
//        window.display();
//    }

    return 0;
}