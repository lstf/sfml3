#ifndef _ENTITY_H
#define _ENTITY_H

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../player.h"
#include "../ui/dbox.h"

class Entity : public sf::Drawable {
private:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const = 0;

public:
	string name;

	static vector<Entity*> list;

	virtual sf::FloatRect bounds() = 0;

	virtual DBox* interact(Player &player, map<string, int> &lstate,
	map<string, int> &gstate) = 0;

	virtual sf::Vector2f size() = 0;

	virtual bool update(Player &player, map<string, int> &lstate,
	map<string, int> &gstate) = 0;

	virtual void set_pos(sf::Vector2f pos) = 0;

	virtual void write(ofstream &out) = 0;
	virtual void read(ifstream &inp) = 0;

	Entity();

	~Entity();
};

#endif
