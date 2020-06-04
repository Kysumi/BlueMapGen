#include "Grid.h"

Grid::Grid(sf::Vector2i size)
{
	this->size = size;

	gridCopy = std::vector<std::vector<Node>>(size.x, std::vector<Node>(size.y));
	grid = std::vector<std::vector<Node>>(size.x, std::vector<Node>(size.y));

	for (auto xAxis = 0; xAxis < size.x; xAxis++) {		
		for (auto yAxis = 0; yAxis < size.y; yAxis++) {
			
			Node node(xAxis, yAxis, nodeSize);

			srand(time(NULL));

			/* generate secret number between 1 and 10: */
			auto number = rand() % 10 + 1;

			if (number > 5)
			{
				node.born();
			}
			
			grid[xAxis][yAxis] = node;
			gridCopy[xAxis][yAxis] = node;
		}
	}
}

Grid::~Grid()
{
	
}

void Grid::Draw(sf::RenderWindow& window)
{
	for (auto xAxis = 0; xAxis < size.x; xAxis++) {
		for (auto yAxis = 0; yAxis < size.y; yAxis++) {
			auto node = grid[xAxis][yAxis];
			node.draw(window);
		}
	}
}

void Grid::Process()
{
	// Replicate grid state to copy
	gridCopy = grid;

	for (auto xAxis = 0; xAxis < size.x; xAxis++) {
		for (auto yAxis = 0; yAxis < size.y; yAxis++) {
			
			auto node = gridCopy[xAxis][yAxis];
			auto count = getAliveNeighbours(sf::Vector2i(xAxis, yAxis)).size();

			if (count < 2) {
				gridCopy[xAxis][yAxis].kill();
			}
			else if (count > 3) {
				gridCopy[xAxis][yAxis].kill();
			}
			else if (count == 3) {
				gridCopy[xAxis][yAxis].born();
			}
		}
	}

	// Flip array back again
	grid = gridCopy;
}

std::vector<Node> Grid::getAliveNeighbours(sf::Vector2i position)
{
	auto neighbours = getNeighbours(position);
	std::vector <Node> aliveNodes;

	for (auto node : neighbours)
	{
		if (node.isAlive())
		{
			aliveNodes.push_back(node);
		}
	}

	return aliveNodes;
}

std::vector<Node> Grid::getNeighbours(sf::Vector2i position)
{
	return getNeighbours(position.x, position.y);
}

std::vector<Node> Grid::getNeighbours(int x, int y)
{
	std::vector<Node> neigbours;
	
	for (auto xAxis = x - 1; xAxis <= x + 1; xAxis++)
	{
		for (auto yAxis = y - 1; yAxis <= y + 1; yAxis++)
		{
			if (xAxis < 0 || yAxis < 0)
			{
				continue;
			}

			if (xAxis > grid.size() || yAxis > grid[0].size())
			{
				continue;
			}

			// Its the node we are trying to find the neighbours of
			if (xAxis == x && yAxis == y)
			{
				continue;
			}
			
			neigbours.push_back(grid[x][y]);
		}
	}

	return neigbours;
}