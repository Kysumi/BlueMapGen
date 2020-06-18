#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Node.h"
#include <ChakraCommon.h>

class Grid {
public:
	Grid(sf::Vector2i size);
	~Grid();

	void Process();
	void Draw(sf::RenderWindow& window);

	static void BindToJS();

	std::vector<Node> getNeighbours(sf::Vector2i position);
	std::vector<Node> getAliveNeighbours(sf::Vector2i position);
	std::vector<Node> getNeighbours(int x, int y);
	
	static JsValueRef JSGridPrototype;
	
private:
	const int nodeSize = 12;
	sf::Vector2i size;

	std::vector<std::vector<Node>> grid;

	std::vector<Node> getNode(int x, int y);
	std::vector<Node> getNode(sf::Vector2i position);
};