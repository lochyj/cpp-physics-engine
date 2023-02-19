#include <SFML/Graphics.hpp>

const int window_width = 1000;
const int window_height = 1000;

int main(int argc, char* argv[])
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Physics Engine", sf::Style::Fullscreen | sf::Style::Close | sf::Style::Titlebar | sf::Style::None, settings);
    const uint32_t frame_rate = 60;
    window.setFramerateLimit(frame_rate);
    window.setVerticalSyncEnabled(true);

    return 0;
}
