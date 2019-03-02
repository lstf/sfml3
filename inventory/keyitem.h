#ifndef _KEYITEM_H
#define _KEYITEM_H

#include <string>

#include <SFML/Graphics.hpp>

#include "inventory.h"
#include "../actors/entity.h"
#include "../utils/txmap.h"

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

	string key_name;
	string key_desc;

	virtual sf::FloatRect bounds();

	virtual DBox* interact(Player &player, map<string, int> &lstate,
	map<string, int> &gstate);

	void update(Player &player, map<string, int> &lstate,
	map<string, int> &gstate);

	virtual void set_pos(sf::Vector2f pos);

	virtual sf::Vector2f size();

	KeyItemEnt();
};

#endif
