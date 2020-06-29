//
// Created by Scott on 25/06/2020.
//
#include "ChakraCore/stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <src/Binding/Grid.h>
#include "ChakraCore/Chakra.h"
#include "WindowManager.h"

int main()
{
    std::cout << "Starting up OnGrid..." << std::endl;
    Chakra::init();
    Chakra::RunScript("assets/scripts/test.js");

    binding::Grid::bind();

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    WindowManager::setActiveWindow(&window);

    return 0;
}