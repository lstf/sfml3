#ifndef _KEYITEM_H
#define _KEYITEM_H

#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>

#include "inventory.h"
#include "../entities/entity.h"
#include "../ui/button.h"
#include "../ui/textbox.h"
#include "../utils/ioutils.h"
#include "../utils/txmap.h"
#include "../utils/sfutils.h"

class KeyItem : public Item {
private:
public:
	KeyItem();
};

class KeyItemEnt : public Entity {
private:
	bool got;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	sf::Sprite sp;

	string levent;
	int lval;
	string gevent;
	int gval;

	string key_name;
	string key_desc;


	virtual sf::FloatRect bounds();

	virtual DBox* interact(Player &player, map<string, int> &lstate,
	map<string, int> &gstate);

	virtual bool update(Player &player, map<string, int> &lstate,
	map<string, int> &gstate);

	virtual void set_pos(sf::Vector2f pos);

	virtual sf::Vector2f size();

	virtual void write(ofstream &out);
	virtual void read(ifstream &inp);

	KeyItemEnt();
};

#define KEYENT_BG sf::Color(127,127,127)
#define KEYENT_FG sf::Color(0,0,0)

class KeyItemEntUI : public sf::Drawable {
private:
	KeyItemEnt* active_ent;

	Textbox* tb;
	Button* name_b;
	Button* levent_b;
	bool typing;
	int field;

	void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	bool handle_input(sf::Event &event, sf::Vector2i m_pos);
	void reset(KeyItemEnt* ent);
	KeyItemEntUI(int x, int y, int w, int h);
};

#endif
