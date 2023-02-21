#include <SFML/Graphics.hpp>
#include <iostream>

#include "utils/physics.hpp"
#include "utils/objects.hpp"
#include "utils/renderer.hpp"

const int window_width = 1000;
const int window_height = 1050;

const int iterations = 10;

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
    settings.antialiasingLevel = 10;
    window.setVerticalSyncEnabled(true);

    // Engine constants
    std::vector<object> objects;
    sf::Clock clock;

    // Create an object
    object obj;
    
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
            obj.position = {500.0f, 500.0f};
            obj.old_position = { 500.1f, 500.0f };
            obj.acceleration = { 10.5f, 0.6f };
            obj.mass = 10.0f;
            obj.radius = 15.0f;
            obj.color = sf::Color::Black;
            objects.push_back(obj);
        }

        window.clear(sf::Color::Black);

        objects = update(objects, iterations);

        draw_objects(objects, window);

        draw_sim_time(clock, window);

        clock.restart().asMilliseconds();

        window.display();
    }

    return 0;
}
