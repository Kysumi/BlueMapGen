#include "Setting.h"

sf::RenderWindow* Setting::window;

void Setting::setActiveWindow(sf::RenderWindow* newWindow)
{
	window = newWindow;
}

sf::RenderWindow* Setting::getActiveWindow()
{
	return window;
}
