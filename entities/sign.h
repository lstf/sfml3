#ifndef _SIGN_H
#define _SIGN_H

#include "fstream"

#include "entity.h"
#include "../utils/ioutils.h"
#include "../utils/txmap.h"

class Sign : public Entity {
private:
	sf::Sprite sp;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	virtual sf::FloatRect bounds();
	virtual DBox* interact(Player &player); 
	virtual bool update(Player &player); 
	virtual sf::Vector2f size();
	virtual void set_pos(sf::Vector2f pos);
	virtual void write(ofstream &out);
	virtual void read(ifstream &inp);
	Sign();
};

#endif
