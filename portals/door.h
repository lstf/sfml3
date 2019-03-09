#ifndef _DOOR_H
#define _DOOR_H

#include <fstream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "portal.h"
#include "../animation.h"
#include "../utils/ioutils.h"
#include "../utils/txmap.h"

#define DOOR_PNG_FILE "./ats/mps/doors/D.PNG"
#define DOOR_PNG_FRAMES 3

using namespace std;

class Door : public Portal {
private:
	Animation anim;
	int frame;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	bool advance_animation();
	
public:
	sf::Sprite sp;

	Door();

	virtual MapTrans* interact();

	virtual bool update();

	virtual sf::FloatRect bounds();

	virtual sf::Vector2f size();

	virtual void set_position(sf::Vector2f pos);

	virtual sf::Vector2f get_position();

	virtual vector<string>* ats_names();

	virtual void write(ofstream &out);

	virtual void read(ifstream &inp);

	~Door();
};

#endif 
