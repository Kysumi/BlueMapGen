//
// Created by scott on 6/29/2020.
//

#ifndef ONGRID_GRID_H
#define ONGRID_GRID_H

#include <SFML/System.hpp>
#include "Node.h"

namespace atlas {

    class Grid {
    public:
        Grid(sf::Vector2i size);
        ~Grid();

        void Process();
        void Draw(sf::RenderWindow &window);

        std::vector<Node> getNeighbours(sf::Vector2i position);
        std::vector<Node> getAliveNeighbours(sf::Vector2i position);
        std::vector<Node> getNeighbours(int x, int y);

    private:
        const int nodeSize = 12;
        sf::Vector2i size;
        std::vector<std::vector<Node>> grid;
    };
}

#endif //ONGRID_GRID_H
