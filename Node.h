#pragma once

#include <SFML/Graphics.hpp>

class Node {
public:
	// For initializing
	Node(){};
	Node(int i, int j, int w);
	~Node();
	bool alive;
	
	bool isAlive();
	void draw(sf::RenderWindow& window);
	void setPosition(sf::Vector2f pos);
	void born();
	void kill();
	void update(sf::RenderWindow& window, sf::Clock& clockActivating, sf::Time& elapsedActivated);
	int getX();
	int getY();
private:
	sf::RectangleShape shape;
	int x, y;
	int cellSize;
};
