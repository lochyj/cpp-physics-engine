#include <SFML/Graphics.hpp>
#include <iostream>

#include "physics.hpp"
#include "objects.hpp"
#include "renderer.hpp""

const int window_width = 1800;
const int window_height = 1000;

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
    
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
        }



        window.clear(sf::Color::Black);

        draw_objects(objects, window);
        update(objects);

        window.display();
    }

    return 0;
}
