#include "Enemies.h"
Enemies::DirectionalSprite::DirectionalSprite(sf::Texture& tex) {
	sprite.setTexture(tex);
	auto ts = tex.getSize();
	cols = 4; rows = 2;
	frameW = ts.x / cols;
	frameH = ts.y / rows;

	static const std::array<std::pair<int, int>, 8> sheetPos = { {
		{0,0},//W
		{1,0},//SW
		{2,0},//S
		{3,0},//SE
		{0,1},//E
		{1,1},//NE
		{2,1},//N
		{3,1},//NW
		} };

	for (int i = 0; i < 8; ++i) {
		auto& [c, r] = sheetPos[i];
		rects[Dir(i)] = sf::IntRect(
			{ c * frameW,r * frameH },
			{ frameW,frameH }
		);
	}
	setDirection(Dir::S);
	sprite.setOrigin(sf::Vector2f(sprite.getTextureRect().size) * 0.5f);
}
void Enemies::DirectionalSprite::setDirection(Dir d) {
	sprite.setTextureRect(rects[d]);
	current = d;
}
void Enemies::DirectionalSprite::fromVector(const sf::Vector2f& v) {
	if (v.x < 0) {
		if (v.y < 0) current = Dir::NW;
		else if (v.y > 0) current = Dir::SW;
		else current = Dir::W;
	}
	else if (v.x > 0) {
		if (v.y < 0) current = Dir::NE;
		else if (v.y > 0) current = Dir::SE;
		else current = Dir::E;
	}
	else {
		if (v.y < 0) current = Dir::N;
		else current = Dir::S;
	}
	setDirection(current);
}

void Enemies::Enemy::move() {
	sf::Vector2f vel = { cos(direction),sin(direction) };
	ds.fromVector(vel);
	sf::Time t1 = timer.getElapsedTime();
	position += Speed * t1.asSeconds() * vel;
	ds.setPosition(position);
	timer.restart();
}

Enemies::DOPPIO::DOPPIO(sf::Texture& tex, sf::Vector2f pos){
	Speed = 100;
	Health = 100;
	dmg = 10;
	hitbox.setSize({ 50,50});
	hitbox.setOrigin({ 25,25 });
	range.setOrigin(range.getRadius()*sf::Vector2f({1,1}));
	vision.setOrigin(vision.getRadius() * sf::Vector2f({ 1,1 }));
	area.setOrigin(area.getRadius() * sf::Vector2f({ 1,1 }));
	spawn.setOrigin(spawn.getRadius() * sf::Vector2f({ 1,1 }));
	position = pos;
	ds = DirectionalSprite(tex);
	ds.setScale({ 0.1f,0.1f });
	ds.setPosition(pos);
	hitbox.setPosition(position);
	range.setPosition(position);
	vision.setPosition(position);
	area.setPosition(position);
	spawn.setPosition(position);
	ds.fromVector({ 0,0 });
	direction = atan2((float)(rand() / 2 - 1),(float)(rand() / 2 - 1));
	timer.reset();
}

void Enemies::DOPPIO::move(){
	sf::Vector2f vel = { cos(direction),sin(direction) };
	ds.fromVector(vel);
	sf::Time t1 = timer.getElapsedTime();
	position += Speed * t1.asSeconds() * vel;
	ds.setPosition(position);
	hitbox.setPosition(position);
	range.setPosition(position);
	vision.setPosition(position);
	timer.restart();
}
