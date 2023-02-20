#include <SFML/Graphics.hpp>
#include <iostream>

#include "utils/physics.hpp"
#include "utils/objects.hpp"
#include "utils/renderer.hpp"

const int window_width = 1000;
const int window_height = 1000;

const int iterations = 9;

sf::Color generate_random_color() {
    auto color = sf::Color::White;
    return color;
}

int main(int argc, char* argv[])
{
    // Window constants
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Physics Engine", sf::Style::Close | sf::Style::Titlebar | sf::Style::None, settings);

    // Settings
    settings.antialiasingLevel = 2;
    window.setVerticalSyncEnabled(true);

    // Engine constants
    std::vector<object> objects;
    sf::Clock clock;

    // Create an object
    object obj;
    obj.position = { 200.0f, 500.0f };
    obj.acceleration = { 0.0f, 0.0f };
    obj.mass = 10.0f;
    obj.radius = 20.0f;
    obj.color = sf::Color::White;
    objects.push_back(obj);
    objects.push_back(obj);
    
    
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            obj.position = {250.0f, 250.0f};
            obj.acceleration = { 0.0f, 0.0f };
            obj.mass = 100.0f;
            obj.radius = 20.0f;
            obj.color = sf::Color::White;
            objects.push_back(obj);
        }

        window.clear(sf::Color::Black);

        objects = update(objects, iterations);

        draw_objects(objects, window);

        window.display();
    }

    return 0;
}
