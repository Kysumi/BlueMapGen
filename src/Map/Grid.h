//
// Created by scott on 6/29/2020.
//

#ifndef ONGRID_GRID_H
#define ONGRID_GRID_H

#include <SFML/System.hpp>
#include "Node.h"
#include <unordered_map>

class Grid {
public:
    Grid(sf::Vector2i size);

    ~Grid();

    void Process();

    void Draw(sf::RenderWindow &window);

    static int getMap(int x, int y);

    sf::Vector2i *getSize();

    std::vector<Node> getNeighbours(sf::Vector2i position);

    std::vector<Node> getAliveNeighbours(sf::Vector2i position);

    std::vector<Node> getNeighbours(int x, int y);

    Node *getNodeFromGridPosition(int x, int y);

private:
    const int nodeSize = 5;
    sf::Vector2i size;
    std::unordered_map<int, Node> map;
};

#endif //ONGRID_GRID_H
