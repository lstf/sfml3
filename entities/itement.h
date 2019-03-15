#ifndef _KEYITEMENT_H
#define _KEYITEMENT_H

#include <SFML/Graphics.hpp>

#include "entity.h"
#include "../world.h"
#include "../effects/sparkle.h"
#include "../inventory/itemutils.h"
#include "../ui/button.h"
#include "../ui/textbox.h"
#include "../utils/ioutils.h"

class ItemEnt : public Entity {
private:
	bool got;
	Sparkle sparkle;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	sf::Sprite sp;

	string levent;
	int lval;
	string gevent;
	int gval;

	string item_name;
	string item_cat;

	ItemEnt();

	virtual sf::FloatRect bounds();

	virtual DBox* interact(Player &player);

	virtual bool update(Player &player);

	virtual void set_pos(sf::Vector2f pos);

	void set_item(string cat, string name);

	virtual sf::Vector2f size();

	virtual void write(ofstream &out);
	virtual void read(ifstream &inp);
};

void read_item_ent(ifstream &inp);

#define KEYENT_BG sf::Color(127,127,127)
#define KEYENT_FG sf::Color(0,0,0)

class ItemEntUI : public sf::Drawable {
private:
	ItemEnt* active_ent;

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
	bool handle_input(sf::Event &event, sf::Vector2i m_pos);
	void reset(ItemEnt* ent);
	ItemEntUI(int x, int y, int w, int h);
};

#endif
