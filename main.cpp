#include <SFML/Graphics.hpp>
#include <iostream>

#include "utils/physics.hpp"
#include "utils/objects.hpp"
#include "utils/renderer.hpp"

const int window_width = 1000;
const int window_height = 1003;

const int iterations = 20;

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

    sf::Clock spawnTimer;

    // Create an object
    object obj;

    obj.old_position = { 200.0f, 500.0f };
    obj.position = { 200.0f, 500.0f };
    obj.mass = 100.0f;
    obj.radius = 5.0f;
    obj.color = sf::Color::Black;
    obj.accelerate({ -10.0f, -100.0f });
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
            obj.old_position = { 700.0f, 500.0f };
            obj.position = {700.0f, 500.0f};
            obj.mass = 100.0f;
            obj.radius = 5.0f;
            obj.color = sf::Color::Black;
            obj.accelerate({ 10.0f, 100.0f });
            objects.push_back(obj);
        }

        window.clear(sf::Color::Black);

        objects = update(objects, iterations);

        draw_objects(objects, window);

        draw_sim_time(clock, window, objects.size());
        
        clock.restart();
        window.display();
    }

    return 0;
}
