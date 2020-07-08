//
// Created by scott on 6/29/2020.
//

#ifndef ONGRID_GRID_H
#define ONGRID_GRID_H

#include <SFML/System.hpp>
#include "Node.h"
#include <unordered_set>

// class for hash function
class HashFunc {
public:
    // id is returned as hash function
    size_t operator()(Node& t) const
    {
        return t.getGridPositionX() + t.getGridPositionY();
    }
};

class Grid {
public:
    Grid(sf::Vector2i size);

    ~Grid();

    void Process();

    void Draw(sf::RenderWindow &window);

    sf::Vector2i *getSize();

    std::vector<Node> getNeighbours(sf::Vector2i position);

    std::vector<Node> getAliveNeighbours(sf::Vector2i position);

    std::vector<Node> getNeighbours(int x, int y);

    Node *getNodeFromGridPosition(int x, int y);

private:
    const int nodeSize = 8;
    sf::Vector2i size;
    std::unordered_set<Node, HashFunc> map;
};

#endif //ONGRID_GRID_H
