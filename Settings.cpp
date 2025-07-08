#include "Settings.h"

void Settings::LoadFromFile(std::string t){
	while(!Font.openFromFile("./External/Font/Mistral.ttf"));
	string line;
	fstream myfile(t);
	if (myfile.is_open()) {
		int w = 0;
		while (getline(myfile, line)) {
			vector<int> n;
			for (int i = 0; i < line.size(); i++) {
				if (line[i] == ';') {
					n.push_back(i);
				}
			}
			if (line.substr(0, n[0]) == "Word") {
				string s = line.substr(n[0] + 1, n[1] - n[0] - 1);
				float x = std::stof(line.substr(n[1] + 1, n[2] - n[1] - 1));
				float y = stof(line.substr(n[2] + 1, n[3] - n[2] - 1));
				Word.push_back(sf::Text(Font, s, 30));
				Word[w].setPosition(sf::Vector2f(x, y));
			}
			w++;
		}
		myfile.close();
	}
	else std::cout << "Unable to open path";
}

void Settings::draw(sf::RenderTarget& rt){
	for (int i = 0; i < Word.size(); i++) {
		rt.draw(Word[i]);
	}
	
}
