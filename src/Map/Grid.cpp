//
// Created by scott on 6/29/2020.
//

#include "Grid.h"

Grid::Grid(sf::Vector2i size) {
    this->size = size;

    map.reserve(size.x * size.y);
    map.max_load_factor(0.25);

    for (auto xAxis = 0; xAxis < size.x; xAxis++) {
        for (auto yAxis = 0; yAxis < size.y; yAxis++) {
            auto xOffSet = 0;
            auto yOffset = 0;

            if (xAxis % 2 == 0) {
                yOffset = nodeSize * 3 / 4;
            }

            Node node(sf::Vector2i(xAxis, yAxis), nodeSize, xOffSet, yOffset);
            auto hash = getUniqueHash(yAxis, xAxis);
            map[hash] = node;
        }
    }

    // Load the new map onto the buffer
    mapBuffer = map;
}

Grid::~Grid() = default;

void Grid::Draw(sf::RenderWindow &window) {
    for (auto xAxis = 0; xAxis < size.x; xAxis++) {
        for (auto yAxis = 0; yAxis < size.y; yAxis++) {
            auto node = map[getUniqueHash(xAxis, yAxis)];
            node.draw(window);
        }
    }
}

std::vector<Node*> Grid::getNeighbours(int x, int y) {
    std::vector<Node*> neighbours;
    neighbours.reserve(9); // Maximum possible neighbours

    for (auto xAxis = x - 1; xAxis <= x + 1; xAxis++) {
        for (auto yAxis = y - 1; yAxis <= y + 1; yAxis++) {
            if (xAxis < 0 || yAxis < 0) {
                continue;
            }

            if (xAxis >= size.x || yAxis >= size.y) {
                continue;
            }

            // Its the node we are trying to find the neighbours of
            if (xAxis == x && yAxis == y) {
                continue;
            }

            auto hash = getUniqueHash(xAxis, yAxis);
            auto node = &map[hash];
            neighbours.emplace_back(node);
        }
    }

    return neighbours;
}

/**
 * BE CAREFUL!! this specifically returns the node from the buffer
 * and not the current "map" which is rendered.
 *
 * This is so that when this copy of the node is modified it does not
 * effect the current state of the "world"
 *
 * @param x
 * @param y
 * @return
 */
Node *Grid::getNodeFromGridPosition(int x, int y) {
    return &mapBuffer[getUniqueHash(x, y)];
}

/**
 * Returns the size of the grid
 *
 * @return
 */
sf::Vector2i *Grid::getSize() {
    return &size;
}

/**
 * Using the cantor pairing method as described here
 * https://pipme.github.io/2020-04-25-cantor-pairing/
 *
 * @param x
 * @param y
 *
 * @return unique number for the input params
 */
int Grid::getUniqueHash(int x, int y) {
    return (x + y) * (x + y + 1) / 2 + y;
}

/**
 * Copies the current mapBuffer instance onto the map instance
 * this should be done just before drawing the current map state
 * if any changes to nodes has happened
 */
void Grid::flipBuffer() {
    map = mapBuffer;
}
