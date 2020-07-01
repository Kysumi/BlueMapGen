//
// Created by scott on 6/29/2020.
//

#include "Grid.h"

atlas::Grid::Grid(sf::Vector2i size)
{
    this->size = size;

    grid = std::vector<std::vector<Node>>(size.x, std::vector<Node>(size.y));

    srand(time(NULL));

    for (auto xAxis = 0; xAxis < size.x; xAxis++) {
        for (auto yAxis = 0; yAxis < size.y; yAxis++) {

            Node node(sf::Vector2i(xAxis, yAxis), nodeSize);

            /* generate secret number between 1 and 10: */
            auto number = rand() % 10 + 1;

            if (number > 7)
            {
                node.born();
            }

            grid[xAxis][yAxis] = node;
        }
    }
}

atlas::Grid::~Grid() = default;

void atlas::Grid::Draw(sf::RenderWindow& window)
{
    for (auto xAxis = 0; xAxis < size.x; xAxis++) {
        for (auto yAxis = 0; yAxis < size.y; yAxis++) {
            auto node = grid[xAxis][yAxis];
            node.draw(window);
        }
    }
}

void atlas::Grid::Process()
{
    // Replicate grid state to copy
    auto gridCopy = grid;

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

std::vector<atlas::Node> atlas::Grid::getAliveNeighbours(sf::Vector2i position)
{
    auto neighbours = getNeighbours(position);
    std::vector<Node> aliveNodes;
    aliveNodes.reserve(9); // maximum possible

    for (auto node : neighbours)
    {
        if (node.isAlive())
        {
            aliveNodes.emplace_back(node);
        }
    }

    return aliveNodes;
}

std::vector<atlas::Node> atlas::Grid::getNeighbours(sf::Vector2i position)
{
    return getNeighbours(position.x, position.y);
}

std::vector<atlas::Node> atlas::Grid::getNeighbours(int x, int y)
{
    std::vector<Node> neighbours;
    neighbours.reserve(9); // Maximum possible neighbours

    for (auto xAxis = x - 1; xAxis <= x + 1; xAxis++)
    {
        for (auto yAxis = y - 1; yAxis <= y + 1; yAxis++)
        {
            if (xAxis < 0 || yAxis < 0)
            {
                continue;
            }

            if (xAxis > grid.size() - 1 || yAxis > grid[0].size() - 1)
            {
                continue;
            }

            // Its the node we are trying to find the neighbours of
            if (xAxis == x && yAxis == y)
            {
                continue;
            }

            neighbours.emplace_back(grid[xAxis][yAxis]);
        }
    }

    return neighbours;
}

atlas::Node *atlas::Grid::getNodeFromGridPosition(int x, int y) {
    return &grid[x][y];
}

sf::Vector2i* atlas::Grid::getSize() {
    return &size;
}
