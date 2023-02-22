#pragma once

#include <SFML/Graphics.hpp>

#include "objects.hpp"


void draw_objects(std::vector<object> objects, sf::RenderTarget& window) {
    auto circle = sf::CircleShape();
    circle.setPointCount(500);
    circle.setOrigin(500.0f, 500.0f);

    circle.setFillColor(sf::Color::White);
    circle.setRadius(500.0f);
    circle.setPosition({ 503.0f, 503.0f });
    window.draw(circle);

    circle.setPointCount(15);
    circle.setOrigin(1.0f, 1.0f);

    for (auto& obj : objects) {
        circle.setFillColor(obj.color);
        circle.setRadius(obj.radius);
        circle.setPosition(obj.position);
        window.draw(circle);
    }

    
}

void draw_sim_time(sf::Clock clock, sf::RenderTarget& window, int balls) {
    auto time = clock.getElapsedTime();
    int32_t tms = time.asMilliseconds();
    std::string str = std::to_string(tms);
    sf::Font font;
    if (!font.loadFromFile("./utils/NotoSans.ttf"))
    {
        //...
    }

    sf::Text text;
    text.setFont(font);
    text.setString("Time Per Frame: " + str);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    window.draw(text);

    text.setFont(font);
    text.setString("Number of Balls: " + std::to_string(balls));
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition({ 0.0f, 24.0f });
    window.draw(text);

}