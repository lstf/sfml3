#ifndef SIGN_O
#define SIGN_O

#include "entity.h"
#include "txmap.h"

class Sign : public Entity {
private:
	sf::FloatRect hitbox;
	sf::Sprite sp;
	string file;
public:
	virtual sf::FloatRect bounds();
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
	virtual DBox* interact(Player &player, map<string, int> &lstate, map<string, int> &gstate); 
	virtual sf::Vector2f size();
	void place(const sf::Vector2f &p);
	Sign(string f);
};

#endif
