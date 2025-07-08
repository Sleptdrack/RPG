#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;
class Settings{
public:
	sf::Font Font;
	vector<sf::Text> Word;
public:
	void LoadFromFile(string t);
	void draw(sf::RenderTarget& rt);
};

