//
// Created by scott on 6/29/2020.
//

#include "Node.h"

Node::Node(sf::Vector2i position, int width) {
    this->alive = false;

    this->shape.setFillColor(sf::Color::Black);
    this->shape.setOutlineColor(sf::Color(196, 196, 196, 0));
    this->shape.setOutlineThickness(1);
    this->shape.setSize(sf::Vector2f(10, 10));

    this->x = position.x;
    this->y = position.y;

    this->size = width;
}

Node::~Node() {
}

bool Node::isAlive() {
    return this->alive;
}

int Node::getGridPositionX() {
    return this->x;
}

int Node::getGridPositionY() {
    return this->y;
}

void Node::draw(sf::RenderWindow &window) {
    this->shape.setPosition(getWorldPosition());
    window.draw(this->shape);
}

void Node::setPosition(sf::Vector2f pos) {
    this->shape.setPosition(pos);
}

void Node::kill() {
    this->alive = false;
    this->shape.setFillColor(sf::Color::Black);
}

void Node::born() {
    this->alive = true;
    this->shape.setFillColor(sf::Color::White);
}

sf::Vector2i Node::getGridPosition() {
    return {this->getGridPositionX(), this->getGridPositionY()};
}

float Node::getWorldPositionX() {
    return this->getGridPositionX() * this->size;
}

float Node::getWorldPositionY() {
    return this->getGridPositionY() * this->size;
}

sf::Vector2f Node::getWorldPosition() {
    return {this->getWorldPositionX(), this->getWorldPositionY()};
}
