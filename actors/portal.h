#ifndef _PORTAL_H
#define _PORTAL_H

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

using namespace std;

class Portal;

struct MapTrans {
	Portal* p;
	std::string map_name;
	sf::Vector2f position;
};

class Portal : public sf::Drawable {
private:
public:
	string name;
	string target;
	sf::Vector2f target_pos;
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const = 0;
	static std::vector<Portal*> list;
	virtual sf::FloatRect bounds() = 0;
	virtual MapTrans* interact() = 0;
	virtual sf::Vector2f size() = 0;
	virtual void setPosition(sf::Vector2f pos) = 0;
	virtual sf::Vector2f getPosition() = 0;
	virtual bool update() = 0;
	Portal();
	~Portal();
};


class Null_Portal : public Portal{
private:
public:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const {
		(void)w;
		(void)states;
	}
	virtual sf::FloatRect bounds() {
		return sf::FloatRect(0,0,0,0);
	}

	virtual MapTrans* interact() {
		return NULL;
	}

	virtual sf::Vector2f size() {
		return sf::Vector2f(0,0);
	}

	virtual void setPosition(sf::Vector2f pos) {
		(void)pos;
	}

	virtual sf::Vector2f getPosition() {
		return sf::Vector2f(0,0);
	}

	virtual bool update() {
		return true;
	}
};

#endif
