#include "weapons.h"

Weapons::Weapon::Weapon(){
	gun.setOrigin({ -75,25 });
}

void Weapons::Weapon::Reload(){
	for (size_t i = 0; i < MagazineSize; i++) {
		mag.emplace_back(make_unique<Ammo>());
	}
}

void Weapons::Weapon::Draw(sf::RenderWindow& rw) {
	//Controller::Window::window.draw(gun);
	if (!pickup) {
		rw.draw(gun);
	}
}
void Weapons::Weapon::move(sf::Vector2f pos) {
	gun.setPosition(pos);
	center = pos;
}
void Weapons::Weapon::followMouse(sf::Vector2f mp){
	float a = mp.x - center.x;
	float b = mp.y - center.y;
	float ang = atan(b / a);
	gun.setRotation(sf::radians(ang));
	if (a > 0) {
		gun.setScale({ 1,1 });
	}
	if (a < 0) {
		gun.setRotation(sf::radians(ang) + sf::degrees(180));
		gun.setScale({ 1,-1 });
	}
	
}

Weapons::Pistol::Pistol(){
	firerate = 300;
	timer.reset();
	timer2.reset();
	Reload();
	gun.setTexture(&pistol);
	dmg = 50;
}

void Weapons::Pistol::Draw(sf::RenderWindow& rw) {
	rw.draw(gun);
	for (auto& e : mag) {
		if (e->out) {
			e->draw(rw);
		}
	}
}

void Weapons::Pistol::primary() {
	if (!timer2.isRunning()) {
		timer2.start();
		mag.back()->out = true;
	}
	else if (timer2.getElapsedTime().asMilliseconds() > firerate) {
		timer2.restart();
		size_t i = mag.size() - 1;
		while (mag.at(i)->out == true) {
			if (mag.at(i) == mag.front()) {
				break;
			}
			else {
				i -= 1;
			}
		}
		if (i >= 0) {
			mag.at(i)->out = true;
		}
	}
	if (mag.empty()) {
		Reload();
	}
}

void Weapons::Pistol::secundary(){
	for (size_t i = 0; i < mag.size(); i++) {
		float a = 360.f / mag.size();
		if (!mag.at(i)->out) {
			mag.at(i)->setDirection(sf::degrees(i * a));
			mag.at(i)->out = true;
		}
	}
	if (mag.empty()) {
		Reload();
	}
}

void Weapons::Pistol::move(sf::Vector2f pos){
	gun.setPosition(pos);
	center = pos;
	sf::Time t1 = timer.getElapsedTime();
	//show movement bullet
	for (auto& ammo : mag) {
		if (ammo->out) {
			ammo->move(t1, speed);
			if (!ammo->range.getGlobalBounds().contains(ammo->hitbox.getPosition())){
				ammo.release();
			}
		}
		else {
			ammo->setPosition(pos);
			ammo->range.setPosition(pos);
		}
	}
	mag.erase(remove(mag.begin(), mag.end(), nullptr), mag.end());
	timer.restart();
}

void Weapons::Pistol::followMouse(sf::Vector2f mp){
	float a = mp.x - center.x;
	float b = mp.y - center.y;
	float ang = atan(b / a);
	if (a > 0) {
		gun.setScale({ 1,1 });
		gun.setRotation(sf::radians(ang));
		for (auto& ammo : mag) {
			if (!ammo->out) {
				ammo->setDirection(sf::radians(ang));
			}
		}
	}
	if (a <= 0) {
		gun.setRotation(sf::radians(ang) + sf::degrees(180));
		for (auto& ammo : mag) {
			if (!ammo->out) {
				ammo->setDirection(sf::radians(ang)+sf::degrees(180));
			}
		}
		gun.setScale({ 1,-1 });
	}
}

Weapons::Ammo::Ammo(){
	hitbox.setTexture(&bullet);
	hitbox.setOrigin({ -75,25 });
	range.setOrigin({ range.getRadius(),range.getRadius() });
}

void Weapons::Ammo::move(sf::Time t, float speed){
	if (out) {
		hitbox.setPosition(
			hitbox.getPosition() +
			sf::Vector2f({
				t.asSeconds() * speed * cos(hitbox.getRotation().asRadians()),
				t.asSeconds() * speed * sin(hitbox.getRotation().asRadians()) }));
	}
}

void Weapons::Ammo::setDirection(sf::Angle ang){
	hitbox.setRotation(ang);
}
