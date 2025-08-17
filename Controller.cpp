#include "Controller.h"
using namespace msclr::interop;
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
	//Audio Setup

	//Controller Setup
	for (size_t i = 0; i < textDes.size(); ++i) {
		Description.push_back(sf::Text(Window::Font, textDes[i], Window::textSize));
		Key.push_back(sf::Text(Window::Font, textKey[i], Window::textSize));
	}
	for (size_t i = 0; i < textDes.size(); ++i) {
		sf::FloatRect tb = Description[i].getGlobalBounds();
		Description[i].setFillColor(sf::Color::White);
		Description[i].setPosition({
			(float)Window::X/3.f,
			i * ((float)Window::Y / textDes.size()-20) + Window::Y/10
			});
		Key[i].setFillColor(sf::Color::White);
		Key[i].setPosition({
			2.f* (float)Window::X/3.f,
			i* ((float)Window::Y / textDes.size() - 20) + Window::Y / 10
			});
	}
	Load();
}

void Controller::SettingsView::ActiveTab(sf::RenderWindow& rt){
	sf::Vector2f mp = rt.mapPixelToCoords(sf::Mouse::getPosition(rt));
	for (size_t i = 0; i < tabLabels.size(); ++i) {
		if (tabs[i].getGlobalBounds().contains(mp)) {
			// reset colors
			tabs[activeTab].setFillColor(sf::Color(200, 200, 200));
			activeTab = static_cast<int>(i);
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

void Controller::SettingsView::changeKey(){
	sf::Vector2f mp = Window::window.mapPixelToCoords(sf::Mouse::getPosition(Window::window));
	for (size_t i = 0; i < Key.size(); ++i) {
		if (Key[i].getGlobalBounds().contains(mp)) {
			changing = true;
			index = i;
			Key[index].setFillColor(sf::Color::Cyan);
		}
	}
}

void Controller::SettingsView::updateKey(string s){
	Key[index].setString(s);
	Key[index].setFillColor(sf::Color::White);
	changing = false;
	Save();
	//Update key in player
}

void Controller::SettingsView::HandleEvent(){
	bool resize = false;
	size_t t;
	//Event if there is a left click
	if (click) {
		ActiveTab(Window::window);
		if (page == Tab::Video) {
			resolution.handleEvent(resize, t);
		}
		if (page == Tab::Controllers){
			changeKey();
		}
		click = false;
	}
	if (changing) {
		if (Window::keypressed) {
			updateKey(Window::keyToString(Keycode));
			Window::keypressed = false;
		}
		if (Window::mousepressed) {
			updateKey(Window::clickToString(Mousecode));
			Window::mousepressed = false;
		}
	}
	//Event if there is a resize
	if (resize) {
		Window::resize(reso[t]);
	}
	if (page == Tab::Audio) {
		Music1.setValue(Music1.getValue());
		Music1.HandleEvent();
	}
}

void Controller::SettingsView::DrawTab(sf::RenderTarget& rt){
	for (size_t i = 0; i < tabNames.size(); ++i) {
		rt.draw(tabs[i]);
		rt.draw(tabLabels[i]);
	}
}

void Controller::SettingsView::DrawController() {
	for (size_t i = 0; i < textDes.size(); ++i) {
		Window::window.draw(Description[i]);
		Window::window.draw(Key[i]);
	}
}

void Controller::SettingsView::Draw(sf::RenderTarget& rt){
	DrawTab(rt);
	switch (page) {
	case Tab::Video:
		resolution.draw(rt);
		break;
	case Tab::Audio:
		Music1.Draw();
		break;
	case Tab::Controllers:
		DrawController();
		break;
	}
	
}

void Controller::SettingsView::Save(){
	DB^ model = gcnew DB();

	model->resolutionIndex = static_cast<int>(resolution.currentIndex);
	model->musicValue = Music1.getValue();
	int count = static_cast<int>(Key.size());
	model->keyBindings = gcnew List<System::String^>();
	for (size_t i = 0; i < Key.size(); ++i) {
		string s = Key[i].getString();
		model->keyBindings->Add(marshal_as<System::String^>(s));
	}

	System::IO::Stream^ stream = System::IO::File::Open("Settings.bin", System::IO::FileMode::Create);
	BinaryFormatter^ bFormatter = gcnew BinaryFormatter();
	bFormatter->Serialize(stream, model);
	stream->Close();
}

void Controller::SettingsView::Load(){
	if (!File::Exists("Settings.bin")) return;
	Stream^ stream = File::Open("Settings.bin", FileMode::Open);
	BinaryFormatter^ bFormatter = gcnew BinaryFormatter();
	DB^ model = (DB^)bFormatter->Deserialize(stream);
	stream->Close();

	resolution.selectOption(model->resolutionIndex);
	Window::resize(reso[model->resolutionIndex]);
	Music1.setValue(model->musicValue);
	for (int i = 0; i < textKey.size(); ++i) {
		string s = marshal_as<string>(model->keyBindings[i]);
		Key[i].setString(s);
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
					SettingsView::Save();
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
	clock.restart();
	window.create(sf::VideoMode(), "RPG");
	window.setSize(SettingsView::reso[SettingsView::resolution.currentIndex]);
	window.setPosition({ 0,0 });

	KeyLabel = {
		{sf::Keyboard::Key::A, "A"},{sf::Keyboard::Key::B, "B"},{sf::Keyboard::Key::C, "C"},
		{sf::Keyboard::Key::D, "D"},{sf::Keyboard::Key::E, "E"},{sf::Keyboard::Key::F, "F"},
		{sf::Keyboard::Key::G, "G"},{sf::Keyboard::Key::H, "H"},{sf::Keyboard::Key::I, "I"},
		{sf::Keyboard::Key::J, "J"},{sf::Keyboard::Key::K, "K"},{sf::Keyboard::Key::L, "L"},
		{sf::Keyboard::Key::M, "M"},{sf::Keyboard::Key::N, "N"},{sf::Keyboard::Key::O, "O"},
		{sf::Keyboard::Key::P, "P"},{sf::Keyboard::Key::Q, "Q"},{sf::Keyboard::Key::R, "R"},
		{sf::Keyboard::Key::S, "S"},{sf::Keyboard::Key::T, "T"},{sf::Keyboard::Key::U, "U"},
		{sf::Keyboard::Key::V, "V"},{sf::Keyboard::Key::W, "W"},{sf::Keyboard::Key::X, "X"},
		{sf::Keyboard::Key::Y, "Y"},{sf::Keyboard::Key::Z, "Z"},

		{sf::Keyboard::Key::Num0, "0"},{sf::Keyboard::Key::Num1, "1"},
		{sf::Keyboard::Key::Num2, "2"},{sf::Keyboard::Key::Num3, "3"},
		{sf::Keyboard::Key::Num4, "4"},{sf::Keyboard::Key::Num5, "5"},
		{sf::Keyboard::Key::Num6, "6"},{sf::Keyboard::Key::Num7, "7"},
		{sf::Keyboard::Key::Num8, "8"},{sf::Keyboard::Key::Num9, "9"},

		{sf::Keyboard::Key::Escape,   "Escape"},
		{sf::Keyboard::Key::LControl, "LControl"},{sf::Keyboard::Key::RControl, "RControl"},
		{sf::Keyboard::Key::LShift,   "LShift"  },{sf::Keyboard::Key::RShift,   "RShift"  },
		{sf::Keyboard::Key::LAlt,     "LAlt"    },{sf::Keyboard::Key::RAlt,     "RAlt"    },
		{sf::Keyboard::Key::Space,    "Space"},
		{sf::Keyboard::Key::Enter,    "Enter"},
		{sf::Keyboard::Key::Tab,      "Tab"},
		{sf::Keyboard::Key::Backspace,"Backspace"},

		{sf::Keyboard::Key::Left,  "Left"},
		{sf::Keyboard::Key::Right, "Right"},
		{sf::Keyboard::Key::Up,    "Up"},
		{sf::Keyboard::Key::Down,  "Down"}
	};

	MouseLabel = {
		{sf::Mouse::Button::Left, "Left Click"},{sf::Mouse::Button::Right,"Right Click"},
		{sf::Mouse::Button::Middle, "Middle Click"}
	};
}

void Controller::Window::HandleEvent(){
	sf::Mouse::Button leftb = Window::StringToClick(SettingsView::textKey[4]);
	sf::Mouse::Button rightb = Window::StringToClick(SettingsView::textKey[5]);
	bool click = false;
	Window::window.handleEvents(
		[&](const sf::Event::Closed) {
			window.close();
		},
		[&](const sf::Event::Resized) {
			window.setView(SettingsView::View);
		},
		[&](const sf::Event::MouseButtonPressed& key) {
			if (SettingsView::changing) {
				SettingsView::Mousecode = key.button;
				mousepressed = true;
			}
			if (key.button == sf::Mouse::Button::Left && !clickstate) {
				clickstate = true;
			}
			if (key.button == leftb && Window::currentView == Window::Game) {
				primary = true;
			}
			if (key.button == rightb && Window::currentView == Window::Game) {
				secundary = true;
			}
		},
		[&](const sf::Event::MouseButtonReleased& key) {
			if (key.button == sf::Mouse::Button::Left && clickstate) {
				clickstate = false;
				click = true;
			}
		},
		[&](const sf::Event::KeyPressed& key) {
			if (SettingsView::changing) {
				SettingsView::Keycode = key.code;
				keypressed = true;
			}
			if (key.code == Window::StringToKey(SettingsView::textKey[6])) {
				dash = true;
			}
			if (key.code == Window::StringToKey(SettingsView::textKey[7])) {
				interact = true;
			}
		}
	);
	if (click) {
		switch (currentView) {
		case Title:
			break;
		case Settings:
			SettingsView::click = true;
			break;
		case Game:
			GameView::click = true;
			break;
		}
	}
}

void Controller::Window::resize(sf::Vector2u u){
	window.setSize(u);
}

string Controller::Window::keyToString(sf::Keyboard::Key key){
	auto it = KeyLabel.find(key);
	return it != KeyLabel.end()? it->second : "Error";
}

sf::Keyboard::Key Controller::Window::StringToKey(string s)
{
	for (auto& it : KeyLabel) {
		if (it.second == s) {
			return it.first;
		}
	}
	return sf::Keyboard::Key::Unknown;
}

string Controller::Window::clickToString(sf::Mouse::Button click){
	auto it = MouseLabel.find(click);
	return it != MouseLabel.end() ? it->second : "Error";
}

sf::Mouse::Button Controller::Window::StringToClick(string s)
{
	for (auto& it : MouseLabel) {
		if (it.second == s) {
			return it.first;
		}
	}
	return sf::Mouse::Button::Extra1;
}

bool Controller::Window::Click(){
	bool n = false;
	Window::window.handleEvents(
		[&](const sf::Event::MouseButtonPressed& key) {
			if (key.button == sf::Mouse::Button::Left && !clickstate) {
				clickstate = true;
			}
		},
		[&](const sf::Event::MouseButtonReleased& key) {
			if (key.button == sf::Mouse::Button::Left && clickstate) {
				clickstate = false;
				n = true;
			}
		}
	);
	return n;
}

Controller::Mision::Mision(string name, string description) {
	//Setup mision name
	Name.setCharacterSize(Window::textSize);
	Name.setString(name);
	Name.setFillColor(sf::Color::White);
	//Background panel
	rec.setFillColor(sf::Color({30,30,30}));
	rec.setSize({ 400,400 });
	//setup description
	Description.setCharacterSize(Window::textSize);
	vector<string> lines = wrapText(description, rec.getSize().x - 20.f);
	ostringstream oss;
	for (auto& l : lines)oss << l << "\n";
	Description.setString(oss.str());
	Description.setFillColor(sf::Color::White);
	//Accept Button
	acceptButton.setSize({
		rec.getSize().x * 0.5f,
		40.f
		});
	acceptButton.setPosition({
		(rec.getSize().x - acceptButton.getSize().x) / 2.f,
		rec.getSize().y - acceptButton.getSize().y - 10.f
		});
	acceptButton.setFillColor(sf::Color({ 70,130,180 }));
	acceptLabel.setFillColor(sf::Color::White);
	centerLabel(acceptLabel, acceptButton.getGlobalBounds());
	SetDesPos({ Window::X / 2.f, Window::Y / 3.f });
}

void Controller::Mision::centerLabel(sf::Text& txt, const sf::FloatRect rect){
	sf::FloatRect tb = txt.getLocalBounds();
	txt.setOrigin(tb.getCenter());
	txt.setPosition(rect.getCenter());
}

std::vector<string> Controller::Mision::wrapText(string s, float maxWidth){
	istringstream words(s);
	vector<string> lines;
	string word, line;
	while (words >> word) {
		string test = line.empty() ? word : line + " " + word;
		Description.setString(test);
		if (Description.getGlobalBounds().size.x > maxWidth) {
			if (!line.empty()) {
				lines.push_back(line);
				line = word;
			}
			else {
				lines.push_back(test);
				line.clear();
			}
		}
		else {
			line = test;
		}
	}
	if (!line.empty())lines.push_back(line);
	return lines;
}

void Controller::Mision::Draw(){
	Window::window.draw(Name);
	if (open) {
		Window::window.draw(rec);
		Window::window.draw(Description);
		Window::window.draw(acceptButton);
		Window::window.draw(acceptLabel);
	}
}

void Controller::Mision::SetPos(sf::Vector2f pos){
	Name.setPosition(pos);
}

void Controller::Mision::SetDesPos(sf::Vector2f pos){
	rec.setPosition(pos);
	Description.setPosition(pos + sf::Vector2f({ 10,10 }));
	acceptButton.setPosition({
		pos.x + (rec.getSize().x * 0.5f) / 2.f,
		pos.y + rec.getSize().y - acceptButton.getSize().y - 10.f
		});
	centerLabel(acceptLabel, acceptButton.getGlobalBounds());
}


bool Controller::Mision::Open(sf::Vector2f mp) const{
	if (Name.getGlobalBounds().contains(mp)) {
		return true;
	}
	return open;

}

bool Controller::Mision::Accept(sf::Vector2f mp) const{
	if (acceptButton.getGlobalBounds().contains(mp)) {
		return true;
	}
	return false;
}

void Controller::GameView::Setup() {
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
			Window::X / 4.f,
			Window::Y / 4.f + static_cast<float>(i) * 80.f
			});
	}
	if (mapT.loadFromFile("./External/PLACEHOLDERS/testmap.png")) {
		map.setTexture(&mapT);
	};
	//Set starting point in map and proper scale, 
	//map.setTextureRect(sf::IntRect({ mapT.getSize().x / 2.f,mapT.getSize().y / 2.f }, { 500,500 }));
	//Set size and scale
	map.setSize(sf::Vector2f({(float)Window::X, (float)Window::Y }));
	map.setScale({ 8,8 });
	//Set position, center in window
	map.setOrigin({ Window::X / 2.f,Window::Y / 2.f });
	map.setPosition({ Window::X / 2.f,Window::Y / 2.f });
	//mini map
	minimap.setViewport(sf::FloatRect({ 0,0.75f }, { 0.25f,0.25f }));
	//Icons
	for (int i = 0; i < 4; i++) {
		Icon t(actionT[i], SettingsView::textKey[4 + i]);
		action.push_back(t);
	}
	//Healthbar
	Healthbar.setMax(p.Health);
	Healthbar.setValue(p.Health);
	//deploy and move weapons
	for (int i = 0; i < 4; i++) {
		//add rand and switch weapon type later
		weapon.emplace_back(make_unique<Weapons::Pistol>());
	}
	for (auto& e : weapon) {
		e->move({ (float)(rand() % Window::X),(float)(rand() % Window::Y) });
	}

}

void Controller::GameView::UpdateAction(){
	sf::Vector2f size = GameView::View.getSize();
	sf::Vector2f center = GameView::View.getCenter();
	sf::Vector2f pos = center + size * 0.5f - sf::Vector2f(600, 100);
	for (size_t i = 0; i < action.size(); i++) {
		action[i].move(pos + sf::Vector2f(static_cast<float>(i) * 150.f, 0.f));
	}
}

void Controller::GameView::Interact(){
	sf::Vector2f mp = Window::window.mapPixelToCoords(sf::Mouse::getPosition(Window::window));
	for (auto& e : weapon) {
		if (Window::interact) {
			if (p.hitbox.getGlobalBounds().findIntersection(e->gun.getGlobalBounds())) {
				e->center = p.getPosition();
				e->pickup = true;
				p.weapon.push_back(move(e));
				Window::interact = false;
				p.armed = true;
				
			}
		}
	}
	weapon.erase(remove(weapon.begin(), weapon.end(), nullptr), weapon.end());
}

void Controller::GameView::SetEnemies(){
	if (wave.gettime() >= spawnrate) {
		for (int i = 0; i < spawnrate/5; i++) {
			sf::Vector2f pos({ 
				(float)(rand() % (Window::X*2)) - Window::X + p.getPosition().x,
				(float)(rand() % (Window::Y*2)) - Window::Y + p.getPosition().y});
			enemy.emplace_back(make_unique<Enemies::DOPPIO>(Enemies::DV4, pos));
		}
		spawnrate += 5;
	}
}

void Controller::GameView::MoveEnemies(){
	sf::Vector2f vector;
	double pi = 3.14159;
	float r = static_cast<float>(pi) / 180;
	if (wave.gettime() > 5) {
		for (auto& e : enemy) {
			if (!e->timer.isRunning()) {
				e->timer.start();
			}
			if (e->vision.getGlobalBounds().findIntersection(p.hitbox.getGlobalBounds())) {
				vector = p.getPosition() - e->position;
				e->direction = atan2(vector.y, vector.x);
			}
			else if (!e->area.getGlobalBounds().contains(e->position)){
				e->center = true;
				vector = e->area.getPosition() - e->position;
				e->direction = atan2(vector.y, vector.x);
			}
			else if (e->spawn.getGlobalBounds().contains(e->position) && e->center == true) {
				float a = ((rand() / 360.f)-180.f) * r;
				e->direction = a;
				e->center = false;
			}
			e->move();
		}
	}
}

void Controller::GameView::fight(){
	for (auto& e : enemy) {
		for (auto& w : p.weapon) {
			for (auto& ammo : w->mag){
				if (e->hitbox.getGlobalBounds().findIntersection(ammo->hitbox.getGlobalBounds())) {
					e->Health -= (int)w->dmg;
					ammo.release();
				}
			}
			w->mag.erase(remove(w->mag.begin(),w->mag.end(), nullptr), w->mag.end());
			if(w->mag.empty())w->Reload();
			if (e->hitbox.getGlobalBounds().findIntersection(p.hitbox.getGlobalBounds())) {
				if (!p.invulnerable) {
					p.Health -= (int) e->dmg;
					Healthbar.setValue(p.Health);
					p.invulnerable = true;
					p.Timer.restart();
				}
				else if (p.Timer.getElapsedTime().asMilliseconds() >= 300) {
					p.Timer.reset();
					p.invulnerable = false;
				}
			}
			if (e->Health <= 0) {
				e.release();
			}
		}
	}
	enemy.erase(remove(enemy.begin(), enemy.end(), nullptr), enemy.end());
}

void Controller::GameView::DrawAction(){
	for (size_t i = 0; i < action.size(); i++) {
		action[i].draw();
	}
}

void Controller::GameView::DrawWeapon(){
	for (auto& e : weapon) {
		e->Draw(Controller::Window::window);
	}
}

void Controller::GameView::DrawEnemies(){
	for (auto& e : enemy) {
		e->draw(Controller::Window::window);
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
		Window::window.draw(map);
		p.Draw();
		Healthbar.draw();
		wave.draw();
		DrawAction();
		DrawWeapon();
		DrawEnemies();
		Window::window.setView(minimap);
		Window::window.draw(map);
		p.Draw();
		DrawEnemies();
		break;
	}
}

void Controller::GameView::HandleEvent(){
	sf::Vector2f mp = Window::window.mapPixelToCoords(sf::Mouse::getPosition(Window::window));
	switch (scene) {
	case Start:
		if (click) {
			if (back.getGlobalBounds().contains(mp)) {
				Window::currentView = Window::View::Title;
			}
			for (size_t i = 0; i < misions.size(); ++i) {
				if (misions[i].Open(mp)) {
					misions[selectedMision].Name.setFillColor(sf::Color::White);
					misions[selectedMision].open = false;
					selectedMision = i;
					misions[selectedMision].Name.setFillColor(sf::Color::Cyan);
					misions[selectedMision].open = true;
				}
			}
			if(misions[selectedMision].Accept(mp)){
				//set objective
				//later send to equiment screen
				//skiping this for now
				scene = Screen::Game;
				wave.restart();
			}
			click = false;
		}
		break;
	case Equipment:
		break;
	case Game:
		UpdateView();
		wave.update();
		UpdateAction();
		Interact();
		SetEnemies();
		MoveEnemies();
		fight();
		p.move();
		p.Aim(mp);
		p.Shoot();
		Healthbar.update();
		break;
	}
}

void Controller::GameView::UpdateView(){
	View.setCenter(p.getPosition());
	minimap.setCenter(p.getPosition());
}

Controller::Slider::Slider(string s,sf::Vector2f position, sf::Vector2f size) {
	value = 0;
	dragging = false;
	//bar setup
	bar.setSize(size);
	bar.setPosition(position);
	bar.setFillColor(sf::Color({ 200,200,200 }));
	bar.setOutlineColor(sf::Color::Black);
	bar.setOutlineThickness(1.f);
	//thumb
	thumb.setSize({ size.y,size.y });
	thumb.setFillColor({ 100,100,250 });
	setValue(0);
	//text
	valueText.setCharacterSize(18);
	valueText.setFillColor(sf::Color::White);
	valueText.setPosition(position - sf::Vector2f({ 25, 0 }));
	//Description
	description.setString(s);
	description.setFillColor(sf::Color::White);
	description.setPosition(valueText.getPosition() - sf::Vector2f({ description.getGlobalBounds().size.x + 10,15.f }));
}

void Controller::Slider::HandleEvent(){
	sf::Vector2f mp = Window::window.mapPixelToCoords(sf::Mouse::getPosition(Window::window));
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (thumb.getGlobalBounds().contains(mp)) {
			dragging = true;
		}
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		dragging = false;
	}
	if (dragging && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		float x = mp.x - thumb.getSize().x / 2.f;
		float minX = bar.getPosition().x;
		float maxX = bar.getPosition().x + bar.getSize().x - thumb.getSize().x;
		x = std::clamp(x, minX, maxX);
		thumb.setPosition({ x, bar.getPosition().y });
		updateText();
	}
}

void Controller::Slider::setValue(int v){
	v = std::clamp(v, 0, 100);
	float ratio = v / 100.f;
	float x = bar.getPosition().x + ratio * (bar.getSize().x - thumb.getSize().x);
	thumb.setPosition({ x, bar.getPosition().y });
	updateText();
}

void Controller::Slider::updateText(){
	valueText.setString(std::to_string(getValue()));
}

int Controller::Slider::getValue(){
	value = static_cast<int>((thumb.getPosition().x - bar.getPosition().x) / (bar.getSize().x - thumb.getSize().x) * 100.f + 0.5f);
	return value;
}

void Controller::Slider::Draw(){
	Window::window.draw(bar);
	Window::window.draw(thumb);
	Window::window.draw(valueText);
	Window::window.draw(description);
}

Controller::Animation::Animation(string s, sf::Vector2i pos, sf::Vector2i size){
	if (!texture.loadFromFile(s)) return;
	for (int i = 0; i < nFrames; i++) {
		frames[i] = sf::IntRect({ pos.x + i * size.x,pos.y }, size);
	}
}

void Controller::Animation::Update(float dt){
	time += dt;
	while (time >= holdTime) {
		time -= holdTime;
		Advance();
	}
}

void Controller::Animation::Advance(){
	if (++iFrame >= nFrames) {
		iFrame = 0;
	}
}

Controller::Player::Player(){
	Health = 500;
	position = sf::Vector2f({ Window::X / 2.f,Window::Y / 2.f });
	Speed = 150;
	Exp = 0;
	Level = 0;
	a1 = Animation("./External/PLACEHOLDERS/KASS-STB.png", { 0,0 },{500,{500}});
	if (tex.loadFromFile("./External/PLACEHOLDERS/KASS-STB.png")) {
		ds = DirectionalSprite(tex);
	};
	ds.setScale({ 0.1f,0.1f });
	hitbox.setSize({ 100,120 });
	hitbox.setOrigin({ 50,60 });
	Timer.reset();
}

sf::Vector2f Controller::Player::getPosition() const{
	return position;
}

void Controller::Player::move(){
	sf::Vector2f vel[4] = { {0,0}, {0,0}, {0,0}, {0,0} }, velo = { 0,0 };
	if (sf::Keyboard::isKeyPressed(Window::StringToKey(SettingsView::Key[0].getString()))){
		//asuming movement is only keys
		vel[0] = { 0,-1 };
		}
	if (sf::Keyboard::isKeyPressed(Window::StringToKey(SettingsView::Key[1].getString()))) {
		//asuming movement is only keys
		vel[1] = { 0,1 };
	}
	if (sf::Keyboard::isKeyPressed(Window::StringToKey(SettingsView::Key[2].getString()))) {
		//asuming movement is only keys
		vel[2] = { 1,0 };
	}
	if (sf::Keyboard::isKeyPressed(Window::StringToKey(SettingsView::Key[3].getString()))) {
		//asuming movement is only keys
		vel[3] = { -1,0 };
	}
	velo = vel[0] + vel[1] + vel[2] + vel[3];
	ds.fromVector(velo);
	sf::Time t1 = Window::clock.getElapsedTime();
	position += Speed * t1.asSeconds() * velo ;
	ds.setPosition(position);
	hitbox.setPosition(position);
	for (auto& e : weapon) e->move(position);
	Window::clock.restart();
}

void Controller::Player::Draw(){
	//animation
	ds.draw();
	if (armed) {
		for (auto& e : weapon)e->Draw(Controller::Window::window);
	}
}


void Controller::Player::Aim(sf::Vector2f mp){
	for (auto& e : weapon) e->followMouse(mp);
}

void Controller::Player::Shoot(){
	for (auto& e : weapon) {
		if (Window::primary) {
			e->primary();
			Window::primary = false;
		}
		if (Window::secundary) {
			e->secundary();
			Window::secundary = false;
		}
	}
}

Controller::DirectionalSprite::DirectionalSprite(sf::Texture& tex){
	sprite.setTexture(tex);
	auto ts = tex.getSize();
	cols = 4; rows = 2;
	frameW = ts.x / cols;
	frameH = ts.y / rows;
	
	static const std::array<std::pair<int, int>, 8> sheetPos = { {
		{0,0},//W
		{1,0},//SW
		{2,0},//S
		{3,0},//SE
		{0,1},//E
		{1,1},//NE
		{2,1},//N
		{3,1},//NW
		}};

	for (int i = 0; i < 8; ++i) {
		auto& [c, r] = sheetPos[i];
		rects[Dir(i)] = sf::IntRect(
			{ c * frameW,r * frameH },
			{ frameW,frameH }
		);
	}
	setDirection(Dir::S);
	sprite.setOrigin(sf::Vector2f(sprite.getTextureRect().size) * 0.5f);
}

void Controller::DirectionalSprite::setDirection(Dir d){
	sprite.setTextureRect(rects[d]);
	current = d;
}
void Controller::DirectionalSprite::fromVector(const sf::Vector2f& v){
	if (v.x < 0) {
		if (v.y < 0) current = Dir::NW;
		else if (v.y > 0) current = Dir::SW;
		else current = Dir::W;
	}
	else if (v.x > 0) {
		if (v.y < 0) current = Dir::NE;
		else if (v.y > 0) current = Dir::SE;
		else current = Dir::E;
	}
	else {
		if (v.y < 0) current = Dir::N;
		else current = Dir::S;
	}
	setDirection(current);
}

Controller::Bar::Bar(sf::Vector2f size, float padding){
	pad = padding;
	bsize = size;
	back.setSize(bsize);
	back.setFillColor(sf::Color(30, 30, 30, 200));
	back.setOutlineColor(sf::Color::White);
	back.setOutlineThickness(2.f);

	fill.setSize(bsize);
	fill.setFillColor(sf::Color(200, 60, 60));

	text.setFillColor(sf::Color::White);
	text.setCharacterSize(16);
}

void Controller::Bar::updateFill(){
	float ratio = (float)value / (float)maxValue;
	ratio = std::clamp(ratio, 0.f, 1.f);
	fill.setSize({ bsize.x * ratio,bsize.y });
}

void Controller::Bar::setPosition(sf::Vector2f pos){
	back.setPosition(pos);
	fill.setPosition(pos);
}

void Controller::Bar::update(){
	sf::Vector2f size = GameView::View.getSize();
	sf::Vector2f of = GameView::View.getCenter();
	sf::Vector2f pos = sf::Vector2f(pad,pad)-size*0.5f+of;
	setPosition(pos);

	text.setString(std::to_string(value) + "/" + to_string(maxValue));
	auto tb = text.getLocalBounds();
	text.setPosition(
		pos
		+ sf::Vector2f(
			bsize.x- tb.size.x-8.f,
			(bsize.y - tb.size.y )* 0.5f-0.f)
		//change 0 till look great
	);
	updateFill();
}

void Controller::Bar::draw() {
	Window::window.draw(back);
	Window::window.draw(fill);
	Window::window.draw(text);
}

Controller::Timer::Timer(){
	time.restart();
	timeText.setFillColor(sf::Color::White);
}
float Controller::Timer::gettime()
{
	return time.getElapsedTime().asSeconds();
}
void Controller::Timer::update() {
	int t = static_cast<int>(time.getElapsedTime().asSeconds());
	int min, sec;
	min = t / 60;
	sec = t % 60;
	timeText.setString(
		(min>=10?to_string(min):"0"+ to_string(min))
		+ ":" +
		(sec >= 10 ? to_string(sec) : "0" + to_string(sec))
	);

	sf::Vector2f size = GameView::View.getSize();
	sf::Vector2f center = GameView::View.getCenter();
	sf::Vector2f of = timeText.getLocalBounds().size;
	sf::Vector2f pos = 
		center + 
		sf::Vector2f(size.x * 0.5f,-size.y*0.5f) - 
		sf::Vector2f(of.x + 12.f,0.f);

	setPosition(pos);

}
void Controller::Timer::restart(){
	time.restart();
}
void Controller::Timer::setPosition(sf::Vector2f pos) {
	timeText.setPosition(pos);
}
void Controller::Timer::draw() {
	Window::window.draw(timeText);
}

Controller::Icon::Icon(sf::Texture& s, string n){
	box.setSize({ 50.f,50.f });
	box.setTexture(&s);
	//sprite.setScale({ 0.1f,0.1f });
	name.setFillColor(sf::Color::White);
	name.setString(n);
}

void Controller::Icon::move(sf::Vector2f pos){
	box.setPosition(pos);
	sf::Vector2f size = box.getGlobalBounds().size;
	name.setPosition(
		pos +
		sf::Vector2f(0,size.y+12.f)
	);
}
void Controller::Icon::draw() {
	Window::window.draw(box);
	Window::window.draw(name);
}
