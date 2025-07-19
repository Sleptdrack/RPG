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
		static inline unsigned int X = 2560, Y = (unsigned int)X * ratio;
		enum View {
			Title = 0,
			Settings = 1,
			Game = 2
		};
		static inline View currentView = View::Title;
	public:
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
		void handleEvent(sf::RenderWindow& window);
		void draw(sf::RenderTarget& rt);
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
	class SettingsView : public IView {
	public:
		//Settings tab
		static inline array<string, 4> tabNames = { "Back", "Video","Audio", "Controller"};
		static inline std::array<sf::RectangleShape, 4> tabs;
		static inline vector<sf::Text> tabLabels;
		static inline float tabWidth = (float)Window::X / tabs.size();
		static inline float tabHeight = 40.f;
		static inline int activeTab = 1;
		//Video tab
		static inline vector<string> res = { "2560x1440","1920x1080","1280x720" };
		static inline vector<sf::Vector2u> reso = { {2560,1440},{1920,1080},{1280,720} };
		static inline Dropdown resolution = Dropdown("Resolution: ",res, {Window::X / 4.f,Window::Y / 4.f}, {200,40});
	public:
		static void SetupTab();
		static void ActiveTab(sf::RenderWindow& rt);
		static void DrawTab(sf::RenderTarget& rt);
		static void Draw(sf::RenderTarget& rt);
	};

}

