#include <SFML/Graphics.hpp>
#include "Controller.h"

int main(){
    Controller::SettingsView::SetupTab();
    Controller::TitleView::SetupMenu();
    sf::RenderWindow window(sf::VideoMode({ Controller::Window::X, Controller::Window::Y }), "RPG");
    window.setPosition({ 0,0 });
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::Resized>()) {
                window.setView(Controller::SettingsView::View);
            }
        }
        window.clear();
        switch (Controller::Window::currentView) {
        case Controller::Window::Title:
            window.setView(Controller::TitleView::View);
            Controller::TitleView::HandleEvent(Controller::Window::currentView, window);
            Controller::TitleView::draw(window);
            break;
        case Controller::Window::Settings:
            window.setView(Controller::SettingsView::View);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                Controller::SettingsView::ActiveTab(window);
            }
            Controller::SettingsView::resolution.handleEvent(window);
            Controller::SettingsView::Draw(window);
            break;
        case Controller::Window::Game:
            break;
        }
        window.display();
    }
}