#include <SFML/Graphics.hpp>
#include "Controller.h"

int main(){
    Settings::LoadFromFile("./External/Data/settings_view.txt");
    sf::RenderWindow window(sf::VideoMode({ Controller::Window::X, Controller::Window::Y }), "SFML works!");
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        Settings::draw(window);
        window.display();
    }
}