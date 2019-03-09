#ifndef _PORTAL_H
#define _PORTAL_H

#include <fstream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

using namespace std;

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

	virtual sf::Vector2f size() = 0;

	virtual void set_position(sf::Vector2f pos) = 0;

	virtual sf::Vector2f get_position() = 0;

	virtual vector<string>* ats_names() = 0;
	virtual void write(ofstream &out) = 0;
	virtual void read(ifstream &inp) = 0;

	~Portal();
};

#endif
