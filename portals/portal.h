#ifndef _PORTAL_H
#define _PORTAL_H

#include <fstream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

using namespace std;

////////////////////////////////////////////////
//
// Portal
//
////////////////////////////////////////////////

class Portal;

struct MapTrans {
	Portal*			p;
	std::string		map_name;
	sf::Vector2f	position;
};

class Portal : public sf::Drawable {
private:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const = 0;

public:
	static std::vector<Portal*> list;

	string 			name;
	string			target;
	sf::Vector2f	target_pos;

	Portal();

	virtual MapTrans* 
	interact() = 0;

	virtual bool update() = 0;

	virtual sf::FloatRect bounds() = 0;

	virtual void set_position(sf::Vector2f pos) = 0;

	~Portal();
};

////////////////////////////////////////////////
//
// Spawner
//
////////////////////////////////////////////////

class PortalSpawner : public sf::Drawable {
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

	string target;
	sf::Vector2f target_pos;

	#ifdef EDITOR_BUILD
	virtual sf::FloatRect bounds() = 0;

	virtual void set_pos(sf::Vector2f pos) = 0;

	virtual void write(ofstream &out) = 0;
	#endif

	virtual void read(ifstream &inp) = 0;
};

#endif
