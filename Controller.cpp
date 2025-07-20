#include "Controller.h"
void Controller::SettingsView::Setup(){
	//Tabs setup
	for (size_t i = 0; i < tabs.size(); ++i) {
		tabLabels.push_back(sf::Text(Window::Font, tabNames[i], Window::textSize));
	}
	for (size_t i = 0; i < tabs.size(); ++i) {
		tabs[i].setSize({ tabWidth - 2, tabHeight - 2 });
		tabs[i].setPosition({ i * tabWidth + 1, 1 });
		tabs[i].setFillColor(sf::Color(200, 200, 200));
		tabs[i].setOutlineColor(sf::Color::Black);
		tabs[i].setOutlineThickness(1.f);
		sf::FloatRect tb = tabLabels[i].getGlobalBounds();
		tabLabels[i].setPosition({
			tabs[i].getPosition().x + (tabWidth - tb.size.x) / 2 - 1,
			tabs[i].getPosition().y + (tabHeight - tb.size.y) / 2 - 5 }
			);
		tabLabels[i].setFillColor(sf::Color::Black);
	};
	tabs[activeTab].setFillColor(sf::Color(100, 100, 250));
	//Video Setup
	resolution.selectOption(1);//1920x1080;
}

void Controller::SettingsView::ActiveTab(sf::RenderWindow& rt){
	sf::Vector2f mp = rt.mapPixelToCoords(sf::Mouse::getPosition(rt));
	for (size_t i = 0; i < tabLabels.size(); ++i) {
		if (tabs[i].getGlobalBounds().contains(mp)) {
			// reset colors
			tabs[activeTab].setFillColor(sf::Color(200, 200, 200));
			activeTab = i;
			tabs[activeTab].setFillColor(sf::Color(100, 100, 250));
			page = Tab(activeTab-1);
			break;
		}
	}
	if (activeTab == 0) {
		Window::currentView = Window::View::Title;
		tabs[activeTab].setFillColor(sf::Color(200, 200, 200));
		activeTab = 1;
		tabs[activeTab].setFillColor(sf::Color(100, 100, 250));
	}
}

void Controller::SettingsView::HandleEvent(){
	sf::Vector2f mp = Window::window.mapPixelToCoords(sf::Mouse::getPosition(Window::window));
	bool resize = false;
	size_t t;
	//Event if there is a left click
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
		ActiveTab(Window::window);
		resolution.handleEvent(resize, t);
	}
	//Event if there is a resize
	if (resize) {
		Window::window.setSize(reso[t]);
	}
}

void Controller::SettingsView::DrawTab(sf::RenderTarget& rt){
	for (size_t i = 0; i < tabNames.size(); ++i) {
		rt.draw(tabs[i]);
		rt.draw(tabLabels[i]);
	}
}

void Controller::SettingsView::Draw(sf::RenderTarget& rt){
	DrawTab(rt);
	switch (page) {
	case Tab::Video:
		resolution.draw(rt);
		break;
	case Tab::Audio:
		break;
	case Tab::Controllers:
		break;
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
	//If arrow are used
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
	//If mouse is used
	sf::Vector2f mp = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
		for (size_t i = 0; i < menuLabels.size(); i++) {
			if (menuLabels[i].getGlobalBounds().contains(mp)) {
				menuLabels[selected].setFillColor(sf::Color(200, 200, 200));
				selected = i;
				menuLabels[selected].setFillColor(sf::Color(100, 100, 150));
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
	}
}

Controller::Dropdown::Dropdown(string des, const vector<string>& items, sf::Vector2f position, sf::Vector2f size){
	//Setup the main box
	currentIndex = 0;
	box.setPosition(position);
	box.setSize(size);
	box.setFillColor(sf::Color(200, 200, 200));
	box.setOutlineColor(sf::Color::Black);
	box.setOutlineThickness(1.f);
	//Description
	description.setPosition(position - sf::Vector2f{ 120, 0 });
	description.setFillColor(sf::Color::White);
	description.setString(des);
	//Current
	currentOption.setFillColor(sf::Color::Black);
	currentOption.setPosition(position + sf::Vector2f(5, 5));
	currentOption.setString(items[currentIndex]);

	for (size_t i = 0; i < items.size(); ++i) {
		sf::RectangleShape optBox;
		optBox.setPosition(position + sf::Vector2f(0, (i + 1) * size.y));
		optBox.setSize(size);
		optBox.setFillColor(sf::Color({ 220,220,220 }));
		optBox.setOutlineColor(sf::Color::Black);
		optBox.setOutlineThickness(1.f);
		optionBoxes.push_back(optBox);

		sf::Text txt(Window::Font, items[i], 18);
		txt.setFillColor(sf::Color::Black);
		txt.setPosition(optBox.getPosition() + sf::Vector2f(5, 5));
		option.push_back(txt);
	}
}

void Controller::Dropdown::selectOption(int i){
	currentIndex = i;
	currentOption.setString(option[currentIndex].getString());
}

void Controller::Dropdown::handleEvent(bool& resize, size_t& j){
	sf::Vector2f mp = Window::window.mapPixelToCoords(sf::Mouse::getPosition(Window::window));
	if (box.getGlobalBounds().contains(mp)) {
		open = !open;
		resize = false;
		return;
	}
	if (open) {
		for (size_t i = 0; i < optionBoxes.size(); ++i) {
			if (optionBoxes[i].getGlobalBounds().contains(mp)) {
				currentIndex = i;
				currentOption.setString(option[i].getString());
				resize = true; //returns true if the user selects something
				j = i;
				open = false;
				break;
			}
		}
	}
}

void Controller::Dropdown::draw(sf::RenderTarget& rt){
	rt.draw(box);
	rt.draw(currentOption);
	rt.draw(description);
	// if open, draw options
	if (open) {
		for (std::size_t i = 0; i < optionBoxes.size(); ++i) {
			rt.draw(optionBoxes[i]);
			rt.draw(option[i]);
		}
	}
}

void Controller::Window::Setup(){
	window.create(sf::VideoMode(), "RPG");
	window.setSize({ X,Y });
	window.setPosition({ 0,0 });
}

Controller::Mision::Mision(string name, string description){
	//Setup mision name
	Name.setCharacterSize(Window::textSize);
	Name.setString(name);
	Name.setFillColor(sf::Color::White);
	//setup description
	Description.setCharacterSize(Window::textSize);
	Description.setString(description);
	Description.setFillColor(sf::Color::White);
}

void Controller::Mision::Draw(){
	Window::window.draw(Name);
}

void Controller::Mision::SetPos(sf::Vector2f pos){
	Name.setPosition(pos);
}

void Controller::Mision::Open(){
	//Open window to show description
	//

}

void Controller::GameView::Setup(){
	back.setPosition({ 0,0 });
	back.setFillColor(sf::Color::White);
	//Load mision probably from file
	//for now 3 blankone
	Mision mision1("Mision 1", "Insertar descripcion aqui que tan larga puede ser nose");
	Mision mision2("Mision 2", "Insertar descripcion aqui que tan larga puede ser nose");
	Mision mision3("Mision 3", "Insertar descripcion aqui que tan larga puede ser nose");
	misions.push_back(mision1);
	misions.push_back(mision2);
	misions.push_back(mision3);
	//Position  mision in screen
	for (size_t i = 0; i < misions.size(); ++i) {
		misions[i].SetPos({
			Window::X / 2.f,
			Window::Y / 4.f + static_cast<float>(i) * 80.f
			});
	}

}

void Controller::GameView::Draw(){
	switch (scene) {
	case Start:
		Window::window.draw(back);
		for (size_t i = 0; i < misions.size(); ++i) {
			misions[i].Draw();
		}
		break;
	case Equipment:
		break;
	case Game:
		break;
	}
}

void Controller::GameView::HandleEvent(){
	sf::Vector2f mp = Window::window.mapPixelToCoords(sf::Mouse::getPosition(Window::window));
	switch (scene) {
	case Start:
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left));
			if (back.getGlobalBounds().contains(mp)) {
				Window::currentView = Window::View::Title;
			}
		}
		//Add method to open description
		///
		break;
	case Equipment:
		break;
	case Game:
		break;
	}
}
