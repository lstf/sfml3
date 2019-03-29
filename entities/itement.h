#ifndef _KEYITEMENT_H
#define _KEYITEMENT_H

#include <SFML/Graphics.hpp>

#include "entity.h"
#include "../logger.h"
#include "../world.h"
#include "../effects/sparkle.h"
#include "../inventory/itemutils.h"
#include "../ui/button.h"
#include "../ui/textbox.h"
#include "../utils/ioutils.h"

using namespace std;

class ItemEntSpawner;
////////////////////////////////////////////////
//
// Entity
//
////////////////////////////////////////////////

class ItemEnt : public Entity {
public:
	ItemEnt();

	virtual sf::FloatRect bounds();

	virtual DBox* interact(Player &player);

	virtual bool update(Player &player);

	virtual void set_pos(sf::Vector2f pos);

	void set_item(string cat, string name);

	~ItemEnt();

private:
friend class ItemEntSpawner;
friend void new_itement(ItemEntSpawner* spawn);
	sf::Sprite sp;

	string levent;
	int lval;

	string item_name;
	string item_cat;

	Sparkle sparkle;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
};

////////////////////////////////////////////////
//
// Spawner
//
////////////////////////////////////////////////

class ItemEntSpawner : public EntitySpawner {
public:
	string levent;
	int lval;

	string item_name;
	string item_cat;

	ItemEntSpawner();

	#ifdef EDITOR_BUILD
	virtual sf::FloatRect bounds();

	virtual void get_sp();

	virtual void set_pos(sf::Vector2f pos);

	virtual void write(ofstream &out);
	#endif

	virtual void read(ifstream &inp);

	~ItemEntSpawner();

private:
};

EntitySpawner* read_itement_spawner(ifstream &inp);

void new_itement(ItemEntSpawner* spawn);

////////////////////////////////////////////////
//
// UI
//
////////////////////////////////////////////////

#ifdef EDITOR_BUILD
class ItemEntUI : public sf::Drawable {
public:
	ItemEntUI(int x, int y, int w, int h);

	bool handle_input(sf::Event &event, sf::Vector2i m_pos);

	void reset(ItemEntSpawner* _spawn);

	~ItemEntUI();

private:
	ItemEntSpawner* spawn;

	Textbox* tb;
	Button* category_b;
	Button* name_b;
	Button* levent_b;
	Button* get_sp_b;
	bool typing;
	int field;

	string item_name;
	string item_cat;

	void draw(sf::RenderTarget& w, sf::RenderStates states) const;
};
#endif

#endif
