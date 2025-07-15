#include "Controller.h"
void Controller::SettingsView::SetupTab(){
	for (size_t i = 0; i < tabs.size(); ++i) {
		tabLabels.push_back(sf::Text(Window::Font, tabNames[i], 18));
	}
	for (size_t i = 0; i < tabs.size(); ++i) {
		tabs[i].setSize({ tabWidth - 2, tabHeight - 2 });
		tabs[i].setPosition({ i * tabWidth + 1, 1 });
		tabs[i].setFillColor(sf::Color(200, 200, 200));
		tabs[i].setOutlineColor(sf::Color::Black);
		tabs[i].setOutlineThickness(1.f);
		sf::FloatRect tb = tabLabels[i].getLocalBounds();
		tabLabels[i].setPosition({
			tabs[i].getPosition().x + (tabWidth - tb.size.x) / 2 - 1,
			tabs[i].getPosition().y + (tabHeight - tb.size.y) / 2 - 5 }
			);
		tabLabels[i].setFillColor(sf::Color::Black);
	}
	int activeTab = 0;
	tabs[activeTab].setFillColor(sf::Color(100, 100, 250));
}

void Controller::SettingsView::ActiveTab(sf::RenderWindow& rt){
	sf::Vector2i mp = sf::Mouse::getPosition(rt);
	for (size_t i = 0; i < tabLabels.size(); ++i) {
		sf::Rect t = tabs[i].getGlobalBounds();
		if (tabs[i].getGlobalBounds().contains({ (float)mp.x,(float)mp.y })) {
			// reset colors
			tabs[activeTab].setFillColor(sf::Color(200, 200, 200));
			activeTab = i;
			tabs[activeTab].setFillColor(sf::Color(100, 100, 250));
			break;
		}
	}
}

void Controller::SettingsView::DrawTab(sf::RenderTarget& rt){
	for (size_t i = 0; i < tabNames.size(); ++i) {
		rt.draw(tabs[i]);
		rt.draw(tabLabels[i]);
	}
}

void Controller::TitleView::SetupMenu(){
	sf::FloatRect fr = title.getLocalBounds();
	title.setPosition({
		Window::X / 2.f - fr.size.x / 2.f,
		Window::Y / 10.f
		});
	for (size_t i = 0; i < menuNames.size(); ++i) {
		sf::Text t(Window::Font, menuNames[i], 48);
		sf::FloatRect r = t.getLocalBounds();
		t.setOrigin({ r.size.x / 2.f, r.size.y / 2.f });
		t.setPosition({
			Window::X / 2.f,
			Window::Y / 4.f + static_cast<float>(i) * 80.f
			});
		t.setFillColor(sf::Color(200, 200, 200));
		menuLabels.push_back(t);
	}
	menuLabels[selected].setFillColor(sf::Color(100, 100, 150));
}

void Controller::TitleView::draw(sf::RenderTarget& rt){
	rt.draw(title);
	for (size_t i = 0; i < menuLabels.size(); ++i) {
		rt.draw(menuLabels[i]);
	}
}

void Controller::TitleView::HandleEvent(Window::View& currentView, sf::RenderWindow& rw){
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		while (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up));
		menuLabels[selected].setFillColor(sf::Color(200, 200, 200));
		selected = (selected + menuNames.size() - 1) % menuNames.size();
		menuLabels[selected].setFillColor(sf::Color(100, 100, 150));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		while(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down));
		menuLabels[selected].setFillColor(sf::Color(200, 200, 200));
		selected = (selected + 1) % menuNames.size();
		menuLabels[selected].setFillColor(sf::Color(100, 100, 150));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
		while (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter));
		switch (selected) {
		case 0: // Start
		case 1: // Start New Game
			currentView = Window::Game;
			break;
		case 2: // Settings
			currentView = Window::Settings;
			break;
		case 3: // Exit Game
			rw.close();
			break;
		}
	}
}
