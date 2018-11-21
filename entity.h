#ifndef ENTITY_O
#define ENTITY_O

#include <vector>

#include <SFML/Graphics.hpp>

#include "dbox.h"

class Entity : public sf::Drawable, public Game_State
{
private:
	static DBox** db;
public:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const = 0;
	static vector<Entity*> list;
	static void setMainDBox(DBox** d);
	static void dialogBox(string file);
	virtual sf::FloatRect bounds() = 0;
	virtual void interact() = 0;
	virtual sf::Vector2f size() = 0;
	Entity();
	~Entity();
};

class Null_Entity : public Entity {
private:
public:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const {
		(void)w;
		(void)states;
	}
	virtual sf::FloatRect bounds() {
		return sf::FloatRect(0,0,0,0);
	}

	virtual void interact() {
	}

	virtual sf::Vector2f size() {
		return sf::Vector2f(0,0);
	}
};

#endif
