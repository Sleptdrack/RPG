#include <SFML/Graphics.hpp>
#include "Controller.h"

int main(){
    Controller::SettingsView::Setup();
    Controller::TitleView::SetupMenu();
    Controller::Window::Setup();
    Controller::GameView::Setup();
    while (Controller::Window::window.isOpen())
    {
        while (const std::optional event = Controller::Window::window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                Controller::Window::window.close();
            if (event->is<sf::Event::Resized>()) {
                Controller::Window::window.setView(Controller::SettingsView::View);
            }
            if (Controller::Window::currentView == Controller::Window::Settings) {
                if (const auto* mbr = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (Controller::SettingsView::changing) {
                        Controller::SettingsView::updateKey(Controller::Window::clickToString(mbr->button));
                    }
                    if (mbr->button == sf::Mouse::Button::Left) {
                        Controller::SettingsView::click1 = true;
                    }
                }
                if (const auto* mbr = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mbr->button == sf::Mouse::Button::Left && Controller::SettingsView::click1) {
                        Controller::SettingsView::click = true;
                        Controller::SettingsView::click1 = false;
                    }
                }
                if (const auto kp = event->getIf<sf::Event::KeyPressed>()) {
                    if (Controller::SettingsView::changing) {
                        Controller::SettingsView::updateKey(Controller::Window::keyToString(kp->code));
                    }
                }
            }
        }
        Controller::Window::window.clear();
        switch (Controller::Window::currentView) {
        case Controller::Window::Title:
            Controller::Window::window.setView(Controller::TitleView::View);
            Controller::TitleView::HandleEvent(Controller::Window::currentView, Controller::Window::window);
            Controller::TitleView::draw(Controller::Window::window);
            break;
        case Controller::Window::Settings:
            Controller::Window::window.setView(Controller::SettingsView::View);
            Controller::SettingsView::HandleEvent();
            Controller::SettingsView::Draw(Controller::Window::window);
            break;
        case Controller::Window::Game:
            Controller::Window::window.setView(Controller::GameView::View);
            Controller::GameView::HandleEvent();
            Controller::GameView::Draw();
            break;
        }
        Controller::Window::window.display();
    }
}