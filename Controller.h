#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <SFML/Graphics.hpp>
using namespace std;
namespace Controller {
	class Window {
	public:
		static inline sf::Font Font = sf::Font("./External/Font/Mistral.ttf");
		static inline int textSize = 30;
		static inline float ratio = 9.f / 16.f;
		static inline unsigned int X = 1920, Y = (unsigned int)X * ratio;
		static inline sf::RenderWindow window;
		static inline unordered_map<sf::Keyboard::Key, string> KeyLabel;
		static inline unordered_map<sf::Mouse::Button, string> MouseLabel;
		enum View {
			Title = 0,
			Settings = 1,
			Game = 2
		};
		static inline View currentView = View::Title;
	public:
		static void Setup();
		static string keyToString(sf::Keyboard::Key key);
		static sf::Keyboard::Key StringToKey(string s);
		static string clickToString(sf::Mouse::Button click);
		static sf::Mouse::Button StringToClick(string s);
	};
	class Dropdown {
	public:
		sf::RectangleShape box;
		sf::Text currentOption = sf::Text(Window::Font,"",Window::textSize);
		sf::Text description = sf::Text(Window::Font, "", Window::textSize);
		vector<sf::Text> option;
		vector<sf::RectangleShape> optionBoxes;
		bool open = false;
		size_t currentIndex=0;
	public:
		Dropdown(string des, const vector<string>& items, sf::Vector2f position, sf::Vector2f size);
		void selectOption(int i);
		void handleEvent(bool& resize, size_t& j);
		void draw(sf::RenderTarget& rt);
	};
	class Slider {
	public:
		sf::RectangleShape bar;
		sf::RectangleShape thumb;
		sf::Text valueText = sf::Text(Window::Font,"",Window::textSize);
		sf::Text description = sf::Text(Window::Font, "", Window::textSize);
		int value;
		bool dragging;
	public:
		Slider(string s,sf::Vector2f position, sf::Vector2f size);
		void HandleEvent();
		void setValue(int v);
		void updateText();
		int getValue();
		void Draw();
	};
	class IView {
	public:
		static inline sf::View View = sf::View({ (float) Window::X / 2.f,(float) Window::Y / 2.f }, { (float)Window::X, (float)Window::Y });
	public:
		virtual ~IView() = default;
		virtual void handleEvent(const sf::Event& e) = 0;
		virtual void update(sf::Time dt) = 0;
		static void draw(sf::RenderTarget& rt) {};
	};
	class TitleView : public IView {
	public:
		static inline sf::Text title = sf::Text(Window::Font,"Title",72);
		static inline array<string, 4> menuNames = { "Start", "Start New Game","Settings", "Exit Game" };
		static inline vector<sf::Text> menuLabels;
		static inline size_t selected = 0;
	public:
		static void SetupMenu();
		static void draw(sf::RenderTarget& rt);
		static void HandleEvent(Window::View& currentView, sf::RenderWindow& rw);
	};
	class Mision {
	public:
		sf::Text Name = sf::Text(Window::Font, "", 1);
		sf::Text Description = sf::Text(Window::Font, "", 1);
	public:
		Mision(string name, string description);
		void Draw();
		void SetPos(sf::Vector2f pos);
		void Open();
	};
	class SettingsView : public IView {
	public:
		//Settings tab
		static inline array<string, 4> tabNames = { "Back", "Video","Audio", "Controller"};
		static inline std::array<sf::RectangleShape, 4> tabs;
		static inline vector<sf::Text> tabLabels;
		static inline float tabWidth = (float)Window::X / tabs.size();
		static inline float tabHeight = 40.f;
		static inline int activeTab = 1;
		static inline bool click = false;
		static inline bool click1 = false;
		//switch between tabs
		enum Tab {
			Video,
			Audio,
			Controllers
		};
		static inline Tab page = Tab::Video;
		//Video tab
		static inline vector<string> res = { "2560x1440","1920x1080","1280x720" };
		static inline vector<sf::Vector2u> reso = { {2560,1440},{1920,1080},{1280,720} };
		static inline Dropdown resolution = Dropdown("Resolution: ",res, {Window::X / 4.f,Window::Y / 4.f}, {200,40});
		//Audio tab
		static inline Slider Music1 = Slider("Music",{ Window::X / 4.f, Window::Y / 4.f }, {500,20});
		//Controller tab
		static inline vector<string> textDes = {
			"Move Forward",
			"Move Backward",
			"Move Right",
			"Move Left",
			"Primary Attack",
			"Secundary Attack",
			"Dodge",
			"Interact" };
		static inline vector<string> textKey = {
			"W",
			"S",
			"D",
			"S",
			"Left click",
			"Right click",
			"Space bar",
			"E" };
		static inline vector<sf::Text> Description;
		static inline vector<sf::Text> Key;
		static inline bool changing = false;
		static inline size_t index = 0;
	public:
		static void Setup();
		static void ActiveTab(sf::RenderWindow& rt);
		static void changeKey();
		static void updateKey(string s);
		static void HandleEvent();
		static void DrawTab(sf::RenderTarget& rt);
		static void DrawController();
		static void Draw(sf::RenderTarget& rt);
	};
	class GameView : public IView {
	public:
		// Start screen
		static inline vector<Mision> misions;
		static inline sf::Text back = sf::Text(Window::Font, "X", 30);
		//Equipment scree

		//Game screen
		enum Screen {
			Start,
			Equipment,
			Game
		};
		static inline Screen scene = Screen::Start;
	public:
		static void Setup();
		static void Draw();
		static void HandleEvent();
	};

}

