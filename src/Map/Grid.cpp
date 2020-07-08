//
// Created by scott on 6/29/2020.
//

#include "Grid.h"

Grid::Grid(sf::Vector2i size) {
    this->size = size;
    srand(time(NULL));

    map.reserve(1024);
    map.max_load_factor(0.25);

    for (auto xAxis = 0; xAxis < size.x; xAxis++) {
        for (auto yAxis = 0; yAxis < size.y; yAxis++) {
            auto xOffSet = 0;
            auto yOffset = 0;

            if (xAxis % 2 == 0) {
                yOffset = nodeSize * 3/4;
            }

            Node node(sf::Vector2i(xAxis, yAxis), nodeSize, xOffSet, yOffset);

            /* generate secret number between 1 and 10: */
            auto number = rand() % 10 + 1;

            if (number > 7) {
                node.born();
            }

            map.insert(node);
        }
    }
}

Grid::~Grid() = default;

void Grid::Draw(sf::RenderWindow &window) {
    for (auto xAxis = 0; xAxis < size.x; xAxis++) {
        for (auto yAxis = 0; yAxis < size.y; yAxis++) {
            auto node = map.find(xAxis+yAxis);
            node.draw(window);
        }
    }
}

void Grid::Process() {
    // Replicate grid state to copy
    auto gridCopy = grid;

    for (auto xAxis = 0; xAxis < size.x; xAxis++) {
        for (auto yAxis = 0; yAxis < size.y; yAxis++) {

            auto node = gridCopy[xAxis][yAxis];
            auto count = getAliveNeighbours(sf::Vector2i(xAxis, yAxis)).size();

            if (count < 2) {
                gridCopy[xAxis][yAxis].kill();
            } else if (count > 3) {
                gridCopy[xAxis][yAxis].kill();
            } else if (count == 3) {
                gridCopy[xAxis][yAxis].born();
            }
        }
    }

    // Flip array back again
    grid = gridCopy;
}

std::vector<Node> Grid::getAliveNeighbours(sf::Vector2i position) {
    auto neighbours = getNeighbours(position);
    std::vector<Node> aliveNodes;
    aliveNodes.reserve(9); // maximum possible

    for (auto node : neighbours) {
        if (node.isAlive()) {
            aliveNodes.emplace_back(node);
        }
    }

    return aliveNodes;
}

std::vector<Node> Grid::getNeighbours(sf::Vector2i position) {
    return getNeighbours(position.x, position.y);
}

std::vector<Node> Grid::getNeighbours(int x, int y) {
    std::vector<Node> neighbours;
    neighbours.reserve(9); // Maximum possible neighbours

    for (auto xAxis = x - 1; xAxis <= x + 1; xAxis++) {
        for (auto yAxis = y - 1; yAxis <= y + 1; yAxis++) {
            if (xAxis < 0 || yAxis < 0) {
                continue;
            }

            if (xAxis > grid.size() - 1 || yAxis > grid[0].size() - 1) {
                continue;
            }

            // Its the node we are trying to find the neighbours of
            if (xAxis == x && yAxis == y) {
                continue;
            }

            neighbours.emplace_back(grid[xAxis][yAxis]);
        }
    }

    return neighbours;
}

Node *Grid::getNodeFromGridPosition(int x, int y) {
    return &grid[x][y];
}

sf::Vector2i *Grid::getSize() {
    return &size;
}
