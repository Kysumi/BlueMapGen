//
// Created by scott on 6/29/2020.
//

#ifndef ONGRID_NODE_H
#define ONGRID_NODE_H

#include <SFML/Graphics.hpp>

class Node {
public:
    // For initializing
    Node(){};
    Node(int i, int j, int w);
    ~Node();
    bool alive;

    bool isAlive();
    void draw(sf::RenderWindow& window);
    void setPosition(sf::Vector2f pos);
    void born();
    void kill();
    void update(sf::RenderWindow& window);
    int getX();
    int getY();
private:
    sf::RectangleShape shape;
    int x, y;
    int cellSize;
};

#endif //ONGRID_NODE_H
