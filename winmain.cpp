#include <SFML/Graphics.hpp>
#include "Controller.h"

int main(){
    Controller::SettingsView::SetupTab();
    Controller::TitleView::SetupMenu();
    Controller::Window::View View=Controller::Window::Title;
    sf::RenderWindow window(sf::VideoMode({ Controller::Window::X, Controller::Window::Y }), "RPG");
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab)) {
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab));
            if (View == Controller::Window::Title) {
                View = Controller::Window::Settings;
            }
            else if (View == Controller::Window::Settings) {
                View = Controller::Window::Title;
            }
        }
        window.clear();
        switch (View) {
        case Controller::Window::Title:
            window.setView(Controller::TitleView::View);
            Controller::TitleView::HandleEvent(View, window);
            Controller::TitleView::draw(window);
            break;
        case Controller::Window::Settings:
            window.setView(Controller::SettingsView::View);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                Controller::SettingsView::ActiveTab(window);
            }
            Controller::SettingsView::DrawTab(window);
            break;
        case Controller::Window::Game:
            break;
        }
        window.display();
    }
}