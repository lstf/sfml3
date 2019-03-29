#ifndef _DOOR_H
#define _DOOR_H

#include <fstream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "portal.h"
#include "../animation.h"
#include "../logger.h"
#include "../utils/ioutils.h"
#include "../utils/txmap.h"

#define DOOR_W 32
#define DOOR_H 48
#define DOOR_PNG "./ats/mps/doors/D.PNG"
#define DOOR_FRAMES 3
#define DOOR_FPS 6

using namespace std;

////////////////////////////////////////////////
//
// Entity
//
////////////////////////////////////////////////

class Door : public Portal {
public:
	sf::Sprite sp;

	Door();

	virtual MapTrans* interact();

	virtual bool update();

	virtual sf::FloatRect bounds();

	virtual void set_position(sf::Vector2f pos);

	~Door();

private:
	Animation anim;
	int frame;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
};

////////////////////////////////////////////////
//
// Spawner
//
////////////////////////////////////////////////

class DoorSpawner : public PortalSpawner {
public:
	DoorSpawner();

	#ifdef EDITOR_BUILD
	virtual sf::FloatRect bounds();

	virtual void set_pos(sf::Vector2f pos);

	virtual void write(ofstream &out);
	#endif

	virtual void read(ifstream &inp);

	~DoorSpawner();

private:
	sf::Texture tx;
};

PortalSpawner* read_door_spawner(ifstream &inp);

void new_door(DoorSpawner* spawn);

#endif 
