#ifndef _ENTITY_H
#define _ENTITY_H

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../player.h"
#include "../ui/dbox.h"

using namespace std;

////////////////////////////////////////////////
//
// Entity
//
////////////////////////////////////////////////

class Entity : public sf::Drawable {
private:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const = 0;

public:
	string name;

	static vector<Entity*> list;

	Entity();

	virtual sf::FloatRect bounds() = 0;

	virtual DBox* interact(Player &player) = 0;

	virtual bool update(Player &player) = 0;

	virtual void set_pos(sf::Vector2f pos) = 0;

	~Entity();
};

////////////////////////////////////////////////
//
// Spawner
//
////////////////////////////////////////////////

class EntitySpawner : public sf::Drawable {
private:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const {
		if (sp_name != "") {
			w.draw(sp, states);
		}
	};

public:
	string name;
	string sp_name;
	sf::Sprite sp;

	#ifdef EDITOR_BUILD
	virtual sf::FloatRect bounds() = 0;

	virtual void set_pos(sf::Vector2f pos) = 0;

	virtual void write(ofstream &out) = 0;
	#endif

	virtual void read(ifstream &inp) = 0;
};

#endif
