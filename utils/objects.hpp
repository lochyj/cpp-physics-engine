#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>



struct object
{
    sf::Vector2f    position;
    sf::Vector2f    old_position;
    sf::Vector2f    acceleration;
    float           radius = 10.0f;
    float           mass = 1.0f;
    float           friction_constant = 1.0f;
    sf::Color       color = sf::Color::White;

    object() = default;
    object(sf::Vector2f position_, float radius_)
        : position{position_},
          old_position{position_},
          acceleration{0.0f, 0.0f},
          radius{radius_}
    {}

    void accelerate(sf::Vector2f accelerate)
    {
        acceleration += accelerate;
    }

    void setVelocity(sf::Vector2f velocity, float deltaTime)
    {
        old_position = position - (velocity * deltaTime);
    }

    void updatePosition(float deltaTime) {
        const sf::Vector2f displacement = position - old_position;
        old_position = position;
        position      = position + displacement + acceleration * (deltaTime * deltaTime);

        acceleration = {};
    }
};

object create_random_object(float maxRadius, float minRadius, sf::Vector2f acceleration, sf::Vector2f position) {
    auto newObject = object(position, minRadius);
    return newObject;
}


struct grid
{
    float x;
    float y;
    float size = 1.0f;
    std::vector<object> objects;

    void add_objects(std::vector<object> objectsToAdd)
    {
	    for(auto& obj : objectsToAdd)
	    {
            objects.push_back(obj);
	    }
    }

    void reset()
    {
        objects.clear();
    }
};
