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

////////////////////////////////////////////////
//
// Entity
//
////////////////////////////////////////////////

class ItemEnt : public Entity {
private:
	bool got;
	Sparkle sparkle;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	sf::Sprite sp;

	string levent;
	int lval;

	string item_name;
	string item_cat;

	ItemEnt();

	virtual sf::FloatRect bounds();

	virtual DBox* interact(Player &player);

	virtual bool update(Player &player);

	virtual void set_pos(sf::Vector2f pos);

	void set_item(string cat, string name);

	virtual sf::Vector2f size();

	~ItemEnt();
};

////////////////////////////////////////////////
//
// Spawner
//
////////////////////////////////////////////////

class ItemEntSpawner : public EntitySpawner {
private:
public:
	string levent;
	int lval;

	string item_name;
	string item_cat;

	ItemEntSpawner();

	virtual sf::FloatRect bounds();

	virtual void get_sp();

	virtual void set_pos(sf::Vector2f pos);

	virtual void write(ofstream &out);

	virtual void read(ifstream &inp);

	~ItemEntSpawner();
};

EntitySpawner* read_itement_spawner(ifstream &inp);

void new_itement_ent(ItemEntSpawner* spawn);

////////////////////////////////////////////////
//
// UI
//
////////////////////////////////////////////////

#define KEYENT_BG sf::Color(127,127,127)
#define KEYENT_FG sf::Color(0,0,0)

class ItemEntUI : public sf::Drawable {
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

public:
	ItemEntUI(int x, int y, int w, int h);

	bool handle_input(sf::Event &event, sf::Vector2i m_pos);

	void reset(ItemEntSpawner* _spawn);

	~ItemEntUI();
};

#endif
