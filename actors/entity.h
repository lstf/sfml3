#ifndef _ENTITY_H
#define _ENTITY_H

#include <vector>
#include <map>
#include <string>

#include <SFML/Graphics.hpp>

#include "player.h"
#include "../ui/dbox.h"

class Entity : public sf::Drawable
{
private:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const = 0;

public:
	string name;
	static vector<Entity*> list;
	virtual sf::FloatRect bounds() = 0;
	virtual DBox* interact(Player &player, map<string, int> &lstate, map<string, int> &gstate) = 0;
	virtual sf::Vector2f size() = 0;
	virtual bool update(Player &player, map<string, int> &lstate, map<string, int> &gstate) = 0;
	virtual void set_pos(sf::Vector2f pos) = 0;
	Entity();
	~Entity();
};

class Null_Entity : public Entity {
private:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const {
		(void)w;
		(void)states;
	}

public:
	virtual sf::FloatRect bounds() {
		return sf::FloatRect(0,0,0,0);
	}

	virtual DBox* interact(Player &player, map<string, int> &lstate, map<string, int> &gstate) {
		(void)player;
		(void)lstate;
		(void)gstate;
		return NULL;
	}

	virtual bool update(Player &player, map<string, int> &lstate, map<string, int> &gstate) {
		(void)player;
		(void)lstate;
		(void)gstate;
		return false;
	}

	virtual void set_pos(sf::Vector2f pos) {
		(void)pos;
	}

	virtual sf::Vector2f size() {
		return sf::Vector2f(0,0);
	}
};

#endif
