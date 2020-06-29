//
// Created by Scott on 25/06/2020.
//
#include "ChakraCore/stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <src/Binding/Grid.h>
#include <src/Binding/Window.h>
#include "ChakraCore/Chakra.h"
#include "WindowManager.h"

int main()
{
    std::cout << "Starting up OnGrid..." << std::endl;
    Chakra::init();

    binding::Grid::bind();
    binding::Window::bind();

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    WindowManager::setActiveWindow(&window);

    Chakra::RunScript("assets/scripts/test.js");

    return 0;
}