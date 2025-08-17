
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <sstream>
# include <cmath>
#include <SFML/Graphics.hpp>
#using <mscorlib.dll>
#include <msclr/marshal_cppstd.h>
#using <System.dll>
using namespace System::IO;
using namespace System::Globalization;
using namespace System::Runtime::Serialization;
using namespace System::Runtime::Serialization::Formatters::Binary;
using namespace System::Collections::Generic;
using namespace std;
namespace Weapons {
	static inline sf::Texture pistol = sf::Texture("./External/PLACEHOLDERS/pistol.png");
	static inline sf::Texture bullet = sf::Texture("./External/PLACEHOLDERS/bullet.png");
	class Ammo {
	public:
		sf::RectangleShape hitbox = sf::RectangleShape({ 50,50 });
		sf::CircleShape range = sf::CircleShape(500.f);
		bool out = false;
	public:
		Ammo();//add range later
		virtual void move(sf::Time t, float speed);
		virtual void setPosition(sf::Vector2f pos) { hitbox.setPosition(pos); };
		virtual void setDirection(sf::Angle ang);
		virtual void draw(sf::RenderWindow& rw) { rw.draw(hitbox); };
	};
	class Weapon {
	public:
		sf::RectangleShape gun = sf::RectangleShape({ 50,50 });
		int MagazineSize = 10;
		vector<unique_ptr<Ammo>> mag;
		float firerate;
		float dmg;
		sf::Vector2f center;
		sf::Clock timer;
		sf::Clock timer2;
		bool pickup = false;
	public:
		Weapon();
		virtual void Reload();
		virtual void Draw(sf::RenderWindow& rw);
		virtual void move(sf::Vector2f pos);
		virtual void followMouse(sf::Vector2f mp);
		virtual void primary() {};
		virtual void secundary() {};
	};
	class Pistol : public Weapon{
	public:
		float speed = 1000.f;
		bool pickup = false;
	public:
		Pistol();
		void Draw(sf::RenderWindow& rw)override;
		void primary()override;
		void secundary()override;
		void move(sf::Vector2f pos)override;
		void followMouse(sf::Vector2f mp)override;
	};
}