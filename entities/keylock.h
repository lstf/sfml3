#ifndef _KEYLOCK_H
#define _KEYLOCK_H

#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>

#include "entity.h"
#include "../logger.h"
#include "../world.h"
#include "../inventory/inventory.h"
#include "../inventory/keyitem.h"
#include "../ui/button.h"
#include "../ui/textbox.h"
#include "../utils/ioutils.h"

using namespace std;

class KeyLockSpawner;
////////////////////////////////////////////////
//
// Entity
//
////////////////////////////////////////////////

class KeyLock : public Entity {
public:
	KeyLock();

	virtual sf::FloatRect bounds();

	virtual DBox* interact(Player &player);

	virtual bool update(Player &player);

	virtual void set_pos(sf::Vector2f pos);

	~KeyLock();

private:
friend class KeyLockSpawner;
friend void new_keylock(KeyLockSpawner* spawn);
	sf::FloatRect r;

	string levent;
	int lval;

	string key_name;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
};

////////////////////////////////////////////////
//
// Spawner
//
////////////////////////////////////////////////

class KeyLockSpawner : public EntitySpawner {
public:
	sf::FloatRect r;

	string levent;
	int lval;

	string key_name;

	KeyLockSpawner();

	#ifdef EDITOR_BUILD
	virtual sf::FloatRect bounds();

	virtual void set_pos(sf::Vector2f pos);

	virtual void write(ofstream &out);
	#endif

	virtual void read(ifstream &inp);

	~KeyLockSpawner();

private:
};

EntitySpawner* read_keylock_spawner(ifstream &inp);

void new_keylock(KeyLockSpawner* spawn);

////////////////////////////////////////////////
//
// UI
//
////////////////////////////////////////////////

#ifdef EDITOR_BUILD
class KeyLockUI : public sf::Drawable {
public:
	KeyLockUI(int x, int y, int w, int h);

	bool handle_input(sf::Event &event, sf::Vector2i m_pos);

	void reset(KeyLockSpawner* _spawn);

	~KeyLockUI();

private:
	KeyLockSpawner* spawn;
	Button* w_b;
	Button* h_b;
	Button* key_name_b;
	Button* levent_b;
	Textbox* tb;
	
	bool typing;
	int field;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
};
#endif

#endif
