#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "weapons.h"
#include "Enemies.h"
#using <mscorlib.dll>
#include <msclr/marshal_cppstd.h>
#using <System.dll>
using namespace System::IO;
using namespace System::Globalization;
//using namespace System;
using namespace System::Runtime::Serialization;
using namespace System::Runtime::Serialization::Formatters::Binary;
using namespace System::Collections::Generic;
using namespace std;
namespace Controller {
	class Window {
	public:
		static inline sf::Clock clock;
		static inline sf::Font Font = sf::Font("./External/Font/Mistral.ttf");
		static inline int textSize = 30;
		static inline float ratio = 9.f / 16.f;
		static inline unsigned int X = 1920, Y = static_cast<unsigned int>(X * ratio);
		static inline sf::RenderWindow window;
		static inline unordered_map<sf::Keyboard::Key, string> KeyLabel;
		static inline unordered_map<sf::Mouse::Button, string> MouseLabel;
		static inline bool mousepressed = false;
		static inline bool keypressed = false;
		static inline bool primary = false;
		static inline bool secundary = false;
		static inline bool dash = false;
		static inline bool interact = false;
		enum View {
			Title = 0,
			Settings = 1,
			Game = 2
		};
		static inline View currentView = View::Title;
		static inline bool clickstate = false;
	public:
		static void Setup();
		static void HandleEvent();
		static void resize(sf::Vector2u u);
		static string keyToString(sf::Keyboard::Key key);
		static sf::Keyboard::Key StringToKey(string s);
		static string clickToString(sf::Mouse::Button click);
		static sf::Mouse::Button StringToClick(string s);
		static bool Click();
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
	class Bar {
	public:
		int maxValue = 100;
		int value = 100;
		float pad;
		sf::Vector2f bsize;
		sf::RectangleShape back, fill;
		sf::Text text = sf::Text(Window::Font);
	public:
		Bar(sf::Vector2f size = { 200.f,22.f }, float padding = 12.f);
		void clamp() { value = std::clamp(value, 0, maxValue); }
		void updateFill();
		void setMax(int m) { maxValue = std::max(1, m); clamp(); updateFill();}
		void setValue(int v) { value = v; clamp(); updateFill();}
		void setPosition(sf::Vector2f pos);
		void update();
		void draw();
	};
	class Timer {
		sf::Clock time;
		sf::Text timeText = sf::Text(Window::Font);
	public:
		Timer();
		float gettime();
		void update();
		void restart();
		void setPosition(sf::Vector2f pos);
		void draw();
	};
	class Icon {
	public:
		sf::RectangleShape box;
		sf::Text name = sf::Text(Window::Font);
	public:
		Icon(sf::Texture& s, string n);
		void move(sf::Vector2f pos);
		void draw();
	};
	/// <summary>
	/// Animation + Player
	/// </summary>
	class Animation {
	public:
		static constexpr int nFrames = 1;//numero de frames
		static constexpr float holdTime = 0.1f;
		sf::Texture texture;
		sf::IntRect frames[nFrames];
		int iFrame = 0;
		float time = 0.0f;
	public:
		Animation() = default;
		Animation(string s, sf::Vector2i pos, sf::Vector2i size);
		void Update(float dt);
		void Advance();
	};
	class DirectionalSprite {
	public:
		enum Dir { W, SW, S, SE, E, NE, N, NW };
		sf::Texture t;
		sf::Sprite sprite = sf::Sprite(t);
		int cols, rows, frameW, frameH;
		Dir current;
		unordered_map<Dir, sf::IntRect> rects;
		DirectionalSprite(sf::Texture& tex);
		void setDirection(Dir d);
		void fromVector(const sf::Vector2f& v);
		void setPosition(const sf::Vector2f& p) { sprite.setPosition(p); };
		void setScale(const sf::Vector2f& s) { sprite.setScale(s); };
		void draw() { Window::window.draw(sprite); };
	};
	public class Player {
	public:
		int Health;
		sf::Vector2f position;
		float Speed;
		float Exp;
		int Level;
		Animation a1;
		sf::Texture tex;
		DirectionalSprite ds = DirectionalSprite(tex);
		sf::RectangleShape hitbox;
		vector<std::unique_ptr<Weapons::Weapon>> weapon;
		bool armed = false;
		bool invulnerable = false;
		sf::Clock Timer;
	public:
		Player();
		sf::Vector2f getPosition() const;
		void move();
		void Draw();
		void Aim(sf::Vector2f mp);
		void Shoot();
	};
	[System::SerializableAttribute()]
	public ref struct DB {
		int resolutionIndex;
		int musicValue;
		List<System::String^>^ keyBindings;
	};
	// Views
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
		sf::Text Name = sf::Text(Window::Font);
		sf::Text Description = sf::Text(Window::Font);
		sf::RectangleShape rec;
		sf::RectangleShape acceptButton;
		sf::Text acceptLabel = sf::Text(Window::Font, "Accept");
		bool open = false;
	public:
		Mision(string name, string description);
		void centerLabel(sf::Text& txt, const sf::FloatRect rect);
		std::vector<string> wrapText(string s, float maxWidth);
		void Draw();
		void SetPos(sf::Vector2f pos);
		void SetDesPos(sf::Vector2f pos);
		bool Open(sf::Vector2f mp) const;
		bool Accept(sf::Vector2f mp) const;
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
			"Left Click",
			"Right Click",
			"Space bar",
			"E" };
		static inline vector<sf::Text> Description;
		static inline vector<sf::Text> Key;
		static inline bool changing = false;
		static inline size_t index = 0;
		static inline sf::Keyboard::Key Keycode;
		static inline sf::Mouse::Button Mousecode;
	public:
		static void Setup();
		static void ActiveTab(sf::RenderWindow& rt);
		static void changeKey();
		static void updateKey(string s);
		static void HandleEvent();
		static void DrawTab(sf::RenderTarget& rt);
		static void DrawController();
		static void Draw(sf::RenderTarget& rt);
		static void Save();
		static void Load();
	};
	class GameView : public IView {
	public:
		// Start screen
		static inline vector<Mision> misions;
		static inline sf::Text back = sf::Text(Window::Font, "X", 30);
		static inline bool click = false;
		static inline size_t selectedMision = 0;
		//Equipment scree

		//Game screen
		enum Screen {
			Start,
			Equipment,
			Game
		};
		static inline Screen scene = Screen::Start;
		static inline Timer wave;
		static inline int spawnrate = 5;
		static inline Player p = Player();
		static inline vector<Icon> action;
		static inline vector<sf::Texture> actionT = {
			sf::Texture("./External/PLACEHOLDERS/KASS-HOLO.png"),
			sf::Texture("./External/PLACEHOLDERS/user.png"),
			sf::Texture("./External/PLACEHOLDERS/user.png"),
			sf::Texture("./External/PLACEHOLDERS/user.png")
		};
		static inline Bar Healthbar;
		static inline sf::Texture mapT;
		static inline sf::RectangleShape map;
		static inline sf::View minimap = sf::View({ 1000,50 }, { 2000,2000 });
		static inline vector<unique_ptr<Weapons::Weapon>> weapon;
		static inline vector<unique_ptr<Enemies::Enemy>> enemy;
	public:
		static void Setup();
		static void UpdateAction(); 
		static void Interact();
		static void SetEnemies();
		static void MoveEnemies();
		static void fight();
		static void DrawAction();
		static void DrawWeapon();
		static void DrawEnemies();
		static void Draw();
		static void HandleEvent();
		static void UpdateView();
	};
}

