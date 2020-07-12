//
// Created by Scott on 25/06/2020.
//
#include "ChakraCore/stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <src/Binding/Map/GridBinding.h>
#include <src/Binding/WindowBinding.h>
#include <src/Binding/Map/NodeBinding.h>
#include <src/Binding/VectorBinding.h>
#include "ChakraCore/Chakra.h"
#include "WindowManager.h"

// TODO take a look into
// https://youtu.be/VguG_y05Xe8?t=452
// https://www.redblobgames.com/grids/hexagons/implementation.html <-- Excellent hexagon grid resource
// https://embeddedartistry.com/blog/2017/08/30/choosing-the-right-container-associative-containers/
int main() {
    std::cout << "Starting up OnGrid..." << std::endl;
    Chakra::Init("assets/scripts");

    GridBinding::bind();
    WindowBinding::bind();
    NodeBinding::bind();
    VectorBinding::bind();

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    WindowManager::setActiveWindow(&window);

    Chakra::RunScript("main.js");

    return 0;
}