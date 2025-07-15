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
		static inline float ratio = 9.f / 16.f;
		static inline unsigned int X = 1920, Y = (unsigned int)X * ratio;
		enum View {
			Title = 0,
			Settings = 1,
			Game = 2
		};
	public:
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
		static inline int activeTab = 0;
	public:
		static void SetupTab();
		static void ActiveTab(sf::RenderWindow& rt);
		static void DrawTab(sf::RenderTarget& rt);
		static inline sf::Text text = sf::Text(Window::Font, "Settings", 30);
		static void draw(sf::RenderTarget& rt) {
			rt.draw(text);
		}
	};
}

