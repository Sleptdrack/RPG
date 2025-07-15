#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;
class Settings{
public:
	static inline sf::Font Font;
	static inline vector<sf::Text> Word;
public:
	static void LoadFromFile(string t);
	static void draw(sf::RenderTarget& rt);
};
namespace Controller {
	class Window {
	public:
		static inline float ratio = (float)9 / 16;
		static inline unsigned int X = 1920, Y = X * ratio;
	};
}

