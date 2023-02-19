#pragma once

#include <SFML/Graphics.hpp>

#include "objects.hpp"


void draw_objects(std::vector<object> objects, sf::RenderTarget& window) {
    auto circle = sf::CircleShape();
    circle.setPointCount(32);
    circle.setOrigin(1.0f, 1.0f);

    for (auto& obj : objects) {
        circle.setFillColor(obj.color);
        circle.setRadius(obj.radius);
        circle.setPosition(obj.position);
        window.draw(circle);
    }
}