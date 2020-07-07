//
// Created by Scott on 25/06/2020.
//
#include "ChakraCore/stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <src/Binding/Grid.h>
#include <src/Binding/Window.h>
#include <src/Binding/Node.h>
#include <src/Binding/Vector.h>
#include "ChakraCore/Chakra.h"
#include "WindowManager.h"

// TODO take a look into
//https://youtu.be/VguG_y05Xe8?t=452

int main()
{
    std::cout << "Starting up OnGrid..." << std::endl;
    Chakra::Init("assets/scripts");

    binding::Grid::bind();
    binding::Window::bind();
    binding::Node::bind();
    binding::Vector::bind();

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    WindowManager::setActiveWindow(&window);

    Chakra::RunScript("main.js");

    return 0;
}