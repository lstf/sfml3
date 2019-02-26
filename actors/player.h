#ifndef _PLAYER_H
#define _PLAYER_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../utils/txmap.h"

#define PLAY_DIR "./ats/player/"
#define PLAY_FPS 12.0

const int L = 0;
const int M = 1;
const int R = 2;
const int T = 0;
const int D = 2;

enum Animation
{
	RIGHT,
	LEFT
};

enum States
{
	STANDING,
	WALKING_LEFT,
	WALKING_RIGHT
};

struct CollisionPoint
{
	sf::Vector2f p;
	bool c;
	int i;
};

struct CollisionPoints
{
	CollisionPoint top[3];
	CollisionPoint down[3];
	CollisionPoint right[3];
	CollisionPoint left[3];
};

struct CollisionBoxes
{
	sf::FloatRect down;
	sf::FloatRect right;
};

struct InputBools
{
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool jump = false;
};

struct SpriteSheet
{
	void* 		png;
	int 		pngSize;
	int 		x;
	int 		y;
	int 		frameCount;
	sf::Clock	clock;
	float		time;
	Animation	dir;
	float		fps;
};


class Weapon : public sf::Drawable
{
private:
	SpriteSheet sp_sheet;
	sf::Sprite sp;
	sf::Texture tx;

	

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
	
public:
	bool active;

	void advanceAnimation();

	void attack();

	void setDirection(Animation _a);

	void setPosition(sf::Vector2f _pos);

	sf::FloatRect bounds();

	Weapon();

};

struct ColDirs {
	bool up;
	bool down;
	bool left;
	bool right;
};

class Player : public sf::Drawable
{ 
private:
	ColDirs coldirs;

	std::vector<sf::FloatRect>* geometry;
	Png png;
	sf::Clock animationClock;
	float animationTime;
	int animationCol;
	int animationFrameCount;
	int animationRow;
	sf::Texture tx;
	InputBools input;
	States state;
	bool stateModified;
	int speed;
	int fallA;
	int fallM;
	int jumph;
	sf::Vector2f velocity;
	CollisionPoints collide;
	bool fresh;
	Weapon weapon;
	bool interaction;

	void updateCollide(std::vector<sf::FloatRect>* geo);

	void setState(States _state);
	
	void setAnimation(Animation a);

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	bool collisionResolver(sf::Vector2f op, std::vector<sf::FloatRect>* geo);

public:
	sf::Sprite sp;

	sf::View view;

	sf::FloatRect bounds();

	bool interacted();

	void handleInput(sf::Event event);

	void setPosition(sf::Vector2f _pos);

	void update(std::vector<sf::FloatRect>* geo, double frameTime);
		
	void advanceAnimation();

	void refresh();

	sf::FloatRect weaponBounds();

	bool weaponActive();

	Player();
};

#endif
