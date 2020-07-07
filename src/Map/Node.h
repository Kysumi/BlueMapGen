//
// Created by scott on 6/29/2020.
//

#ifndef ONGRID_NODE_H
#define ONGRID_NODE_H

#include <SFML/Graphics.hpp>
    class Node {
    public:
        // For initializing
        Node() {};

        Node(sf::Vector2i position, int width);

        ~Node();

        bool alive;

        bool isAlive();

        void draw(sf::RenderWindow &window);

        void setPosition(sf::Vector2f pos);

        void born();

        void kill();

        int getGridPositionX();

        int getGridPositionY();

        sf::Vector2i getGridPosition();

        float getWorldPositionX();

        float getWorldPositionY();

        sf::Vector2f getWorldPosition();

    private:
        sf::RectangleShape shape;
        int x, y;
        int size;
    };

#endif //ONGRID_NODE_H
