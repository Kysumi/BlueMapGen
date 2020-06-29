//
// Created by scott on 6/29/2020.
//

#ifndef ONGRID_WINDOWMANAGER_H
#define ONGRID_WINDOWMANAGER_H

#include <SFML/Graphics.hpp>

class WindowManager {
public:
    static void setActiveWindow(sf::RenderWindow* newWindow);
    static sf::RenderWindow* getActiveWindow();

private:
    static sf::RenderWindow *window;
};


#endif //ONGRID_WINDOWMANAGER_H
