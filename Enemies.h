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
namespace Enemies{
	static inline sf::Texture DV4 = sf::Texture("./External/PLACEHOLDERS/DOPPIO-V04.png");
	class DirectionalSprite {
	public:
		enum Dir { W, SW, S, SE, E, NE, N, NW };
		sf::Sprite sprite = sf::Sprite(DV4);
		int cols, rows, frameW, frameH;
		Dir current;
		unordered_map<Dir, sf::IntRect> rects;
		DirectionalSprite(sf::Texture& tex);
		void setDirection(Dir d);
		void fromVector(const sf::Vector2f& v);
		void setPosition(const sf::Vector2f& p) { sprite.setPosition(p); };
		void setScale(const sf::Vector2f& s) { sprite.setScale(s); };
		void draw(sf::RenderWindow& rw) { rw.draw(sprite); };
	};
	class Enemy {
	public:
		int Health;
		sf::Vector2f position;
		float direction;
		float Speed;
		float dmg;
		bool center = false;
		sf::CircleShape vision = sf::CircleShape(300.f);
		sf::CircleShape area = sf::CircleShape(300.f);
		sf::CircleShape spawn = sf::CircleShape(50.f);
		DirectionalSprite ds = DirectionalSprite(DV4);
		sf::Clock timer;
		sf::RectangleShape hitbox;
	public:
		Enemy() {};
		virtual void draw(sf::RenderWindow& rw) {
			ds.draw(rw); };
		virtual void move();
	};
	class DOPPIO : public Enemy {
	public:
		sf::CircleShape range = sf::CircleShape(150.f);
		float atkspeed;
	public:
		DOPPIO(sf::Texture& tex,sf::Vector2f pos);
		void move()override;
	};
};

