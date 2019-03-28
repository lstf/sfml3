#ifndef _PLAYER_H
#define _PLAYER_H

#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#include <SFML/Graphics.hpp>

#include "animation.h"
#include "logger.h"
#include "window.h"
#include "inventory/inventory.h"
#include "inventory/keyitem.h"
#include "utils/ioutils.h"
#include "utils/sfutils.h"
#include "utils/txmap.h"

#define WEAPON_PNG "./ats/player/W.PNG"
#define PLAY_PNG "./ats/player/P.PNG"
#define PLAY_DIR "./ats/player/"
#define PLAY_FPS 12.0

enum States {
	STANDING,
	WALKING_LEFT,
	WALKING_RIGHT
};

struct InputBools {
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;
	bool jump = false;
};

class Weapon : public sf::Drawable {
private:
	Animation anim;
	int dir;
	int frame_counter;
	bool buffered_attack;

	sf::Sprite sp;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
	
public:
	Weapon();

	bool active;

	void update();

	void attack();

	void set_direction(int d);

	void set_position(sf::Vector2f _pos);

	sf::FloatRect bounds();

	~Weapon();
};

struct ColDirs {
	bool up;
	bool down;
	bool left;
	bool right;
};

class Player : public sf::Drawable { 
private:
	std::vector<sf::FloatRect>* geometry;

	Animation anim;
	ColDirs coldirs;
	InputBools input;
	States state;

	int speed;
	int fallA;
	int fallM;
	int jumph;
	sf::Vector2f velocity;
	bool fresh;
	Weapon weapon;
	bool interaction;

	float max_jump;

	void updateCollide(std::vector<sf::FloatRect>* geo);

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	void collisionResolver(sf::Vector2f op, std::vector<sf::FloatRect>* geo);

public:
	Inventory inv;
	sf::Sprite sp;

	sf::View view;

	sf::FloatRect bounds();

	Player();

	bool interacted();

	void handleInput(sf::Event event);

	void setPosition(sf::Vector2f _pos);

	void update(std::vector<sf::FloatRect>* geo, double frameTime);
		
	void advanceAnimation();

	void refresh();

	void reset_input();

	sf::FloatRect weaponBounds();

	bool weaponActive();

	void write(ofstream &out);

	void read(ifstream &inp);

	~Player();
};

#endif
