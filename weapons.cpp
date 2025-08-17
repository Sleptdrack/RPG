#include "weapons.h"

Weapons::Weapon::Weapon(){
	gun.setOrigin({ -75,25 });
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
	ammo.setTexture(&bullet);
	ammo.setOrigin({ -75,25 });
	range.setOrigin({ range.getRadius(),range.getRadius() });
	gun.setTexture(&pistol);
	dmg = 50;
}

void Weapons::Pistol::Draw(sf::RenderWindow& rw) {
	rw.draw(gun);
	if (shoot) {
		rw.draw(ammo);
	}
}

void Weapons::Pistol::primary() {
	if (Magazine > 0) {
		--Magazine;
	}
	else {
		Magazine = MagazineSize;
	}
	shoot = true;
}

void Weapons::Pistol::move(sf::Vector2f pos){
	gun.setPosition(pos);
	center = pos;
	sf::Time t1 = timer.getElapsedTime();
	if (!shoot) {
		ammo.setPosition(pos);
		range.setPosition(pos);
	}
	else {
		ammo.setPosition(
			ammo.getPosition() + 
			sf::Vector2f({ 
				t1.asSeconds()*speed * cos(ammo.getRotation().asRadians()),
				t1.asSeconds()*speed * sin(ammo.getRotation().asRadians()) }));
		if (!range.getGlobalBounds().contains(ammo.getPosition())) {
			shoot = false;
		}
	}
	timer.restart();
}

void Weapons::Pistol::followMouse(sf::Vector2f mp){
	float a = mp.x - center.x;
	float b = mp.y - center.y;
	float ang = atan(b / a);
	if (a > 0) {
		gun.setScale({ 1,1 });
		gun.setRotation(sf::radians(ang));
		if (!shoot)ammo.setRotation(sf::radians(ang));
	}
	if (a <= 0) {
		gun.setRotation(sf::radians(ang) + sf::degrees(180));
		if(!shoot)ammo.setRotation(sf::radians(ang) + sf::degrees(180));
		gun.setScale({ 1,-1 });
	}
}
