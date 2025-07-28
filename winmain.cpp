#include <SFML/Graphics.hpp>
#include "Controller.h"

int main(){
    Controller::SettingsView::Setup();
    Controller::TitleView::SetupMenu();
    Controller::Window::Setup();
    Controller::GameView::Setup();
    while (Controller::Window::window.isOpen()){
        Controller::Window::HandleEvent();
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