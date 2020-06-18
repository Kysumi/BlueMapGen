#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

class Setting
{
public:
	static void setActiveWindow(sf::RenderWindow* newWindow);
	static sf::RenderWindow* getActiveWindow();

private:
	static sf::RenderWindow *window;
};

