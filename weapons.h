
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
	class Weapon {
	public:
		sf::RectangleShape gun = sf::RectangleShape({ 50,50 });
		sf::RectangleShape ammo = sf::RectangleShape({ 50,50 });
		float dmg;
		sf::Vector2f center;
		bool pickup = false;
		bool shoot = false;
	public:
		Weapon();
		virtual void Draw(sf::RenderWindow& rw);
		virtual void move(sf::Vector2f pos);
		virtual void followMouse(sf::Vector2f mp);
		virtual bool Hit() { return shoot; };
		virtual void primary() {};
		virtual void secundary() {};
	};
	class Pistol : public Weapon{
	public:
		int MagazineSize = 10;
		int Magazine = 10;
		sf::CircleShape range = sf::CircleShape(500.f);
		sf::Clock timer;
		float speed = 1000.f;
		bool pickup = false;
	public:
		Pistol();
		void Draw(sf::RenderWindow& rw)override;
		void primary()override;
		void move(sf::Vector2f pos)override;
		void followMouse(sf::Vector2f mp)override;
	};
}