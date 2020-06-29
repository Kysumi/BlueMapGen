//
// Created by scott on 6/29/2020.
//

#include "WindowManager.h"

sf::RenderWindow* WindowManager::window;

void WindowManager::setActiveWindow(sf::RenderWindow* newWindow)
{
    window = newWindow;
}

sf::RenderWindow* WindowManager::getActiveWindow()
{
    return window;
}
