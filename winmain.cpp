#include <SFML/Graphics.hpp>
#include "Settings.h"

int main(){
    Settings S;
    S.LoadFromFile("./External/Data/test.txt");
    sf::RenderWindow window(sf::VideoMode({ 400, 400 }), "SFML works!");

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        S.draw(window);
        window.display();
    }
}