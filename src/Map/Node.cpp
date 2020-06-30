//
// Created by scott on 6/29/2020.
//

#include "Node.h"

atlas::Node::Node(sf::Vector2i position, int width) {
    this->alive = false;

    this->shape.setFillColor(sf::Color::Black);
    this->shape.setOutlineColor(sf::Color(196, 196, 196, 0));
    this->shape.setOutlineThickness(1);
    this->shape.setSize(sf::Vector2f(10, 10));

    this->x = position.x;
    this->y = position.y;

    this->size = width;
}

atlas::Node::~Node() {
}

bool atlas::Node::isAlive() {
    return this->alive;
}

int atlas::Node::getGridPositionX() {
    return this->x;
}

int atlas::Node::getGridPositionY() {
    return this->y;
}

void atlas::Node::draw(sf::RenderWindow &window) {
    int xCoord = this->x * this->size;
    int yCoord = this->y * this->size;
    this->shape.setPosition(sf::Vector2f(xCoord, yCoord));

    window.draw(this->shape);
}

void atlas::Node::setPosition(sf::Vector2f pos) {
    this->shape.setPosition(pos);
}

void atlas::Node::kill() {
    this->alive = false;
    this->shape.setFillColor(sf::Color::Black);
}

void atlas::Node::born() {
    this->alive = true;
    this->shape.setFillColor(sf::Color::White);
}

void atlas::Node::update(sf::RenderWindow &window) {
    if (this->alive) {
        this->alive = false;
        this->kill();
    } else {
        this->alive = true;
        this->born();
    }
}

sf::Vector2i atlas::Node::getGridPosition() {
    return {this->getGridPositionX(), this->getGridPositionY()};
}

float atlas::Node::getWorldPositionX() {
    return this->getGridPositionX() * this->size;
}

float atlas::Node::getWorldPositionY() {
    return this->getGridPositionY() * this->size;
}

sf::Vector2f atlas::Node::getWorldPosition() {
    return {this->getWorldPositionX(), this->getWorldPositionY()};
}
