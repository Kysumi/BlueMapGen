#include "Node.h"

Node::Node(int i, int j, int w)
{
    this->alive = false;
    this->shape.setFillColor(sf::Color::Black);
    this->shape.setOutlineColor(sf::Color(196, 196, 196, 0));
    this->shape.setOutlineThickness(1);
    this->shape.setSize(sf::Vector2f(10, 10));
    this->x = i;
    this->y = j;
    this->cellSize = w;
}

Node::~Node()
{
}

bool Node::isAlive()
{
    return this->alive;
}

int Node::getX()
{
	return this->x;
}

int Node::getY()
{
	return this->y;
}

void Node::draw(sf::RenderWindow& window)
{
    int xCoord = this->x * this->cellSize;
    int yCoord = this->y * this->cellSize;
    this->shape.setPosition(sf::Vector2f(xCoord, yCoord));
    window.draw(this->shape);
}

void Node::setPosition(sf::Vector2f pos)
{
    this->shape.setPosition(pos);
}

void Node::kill()
{
    this->alive = false;
    this->shape.setFillColor(sf::Color::Black);
}

void Node::born()
{
    this->alive = true;
    this->shape.setFillColor(sf::Color::White);
}

void Node::update(sf::RenderWindow& window, sf::Clock& clockActivating, sf::Time& elapsedActivated)
{
    if (elapsedActivated.asSeconds() >= 0.2) {
        clockActivating.restart();
        if (this->alive) {
            this->alive = false;
            this->kill();
        }
        else {
            this->alive = true;
            this->born();
        }
    }
}