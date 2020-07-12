//
// Created by scott on 6/29/2020.
//

#include <src/Assets/TextureManager.h>
#include "Node.h"

Node::Node(sf::Vector2i position, int width, float xOffset, float yOffset) {
    this->alive = false;

//    this->shape.setFillColor(sf::Color::Black);
//    this->shape.setOutlineColor(sf::Color::Magenta);
//    this->shape.setOutlineThickness(1);

    this->shape.setPointCount(6);
    this->shape.setRadius((float) width - 2);
    this->shape.setRotation(30);

    this->x = position.x;
    this->y = position.y;

    this->size = width;

    auto worldPos = getWorldPosition();
    worldPos.x += xOffset;
    worldPos.y += yOffset;
    this->shape.setPosition(worldPos);
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
    window.draw(this->shape);
}

//void Node::setPosition(sf::Vector2f pos) {
//    this->shape.setPosition(pos);
//}

//void Node::kill() {
//    this->alive = false;
//    this->shape.setFillColor(sf::Color::Black);
//}
//
//void Node::born() {
//    this->alive = true;
//    this->shape.setFillColor(sf::Color::White);
//}

sf::Vector2i Node::getGridPosition() {
    return {this->getGridPositionX(), this->getGridPositionY()};
}

float Node::getWorldPositionX() {
    return this->getGridPositionX() * (sqrt(3) * this->size);
}

float Node::getWorldPositionY() {
    return this->getGridPositionY() * (this->size * 2);
}

sf::Vector2f Node::getWorldPosition() {
    return {this->getWorldPositionX(), this->getWorldPositionY()};
}

void Node::setTexture(const std::string& textureName) {
    sf::Texture *texture = TextureManager::getTexture(textureName);
//    this->sprite = sf::Sprite(*texture);

    this->shape.setTexture(texture);

}
