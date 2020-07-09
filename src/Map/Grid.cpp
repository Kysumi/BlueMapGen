//
// Created by scott on 6/29/2020.
//

#include "Grid.h"

Grid::Grid(sf::Vector2i size) {
    this->size = size;
    srand(time(NULL));

    map.reserve(size.x * size.y);
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

            auto hash = getMap(yAxis, xAxis);
            map[hash] = node;
        }
    }
}

Grid::~Grid() = default;

void Grid::Draw(sf::RenderWindow &window) {
    for (auto xAxis = 0; xAxis < size.x; xAxis++) {
        for (auto yAxis = 0; yAxis < size.y; yAxis++) {
            auto node = map[getMap(xAxis, yAxis)];
            node.draw(window);
        }
    }
}

void Grid::Process() {
    // Replicate grid state to copy
    auto gridCopy = map;

    for (auto xAxis = 0; xAxis < size.x; xAxis++) {
        for (auto yAxis = 0; yAxis < size.y; yAxis++) {
            auto count = getAliveNeighbours(sf::Vector2i(xAxis, yAxis)).size();

            if (count < 2) {
                gridCopy[xAxis + yAxis].kill();
            } else if (count > 3) {
                gridCopy[xAxis + yAxis].kill();
            } else if (count == 3) {
                gridCopy[xAxis + yAxis].born();
            }
        }
    }

    // Flip array back again
    map = gridCopy;
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

            if (xAxis >= size.x - 1 || yAxis > size.y - 1) {
                continue;
            }

            // Its the node we are trying to find the neighbours of
            if (xAxis == x && yAxis == y) {
                continue;
            }
            auto hash = getMap(xAxis, yAxis);
            neighbours.emplace_back(map[hash]);
        }
    }

    return neighbours;
}

Node *Grid::getNodeFromGridPosition(int x, int y) {
    return &map[getMap(x, y)];
}

sf::Vector2i *Grid::getSize() {
    return &size;
}

int Grid::getMap(int x, int y) {

    std::hash<int> int_hash;
    size_t hashX = int_hash(x);
    size_t hashY = int_hash(y);

    return hashX ^ (hashY << 1);
}
