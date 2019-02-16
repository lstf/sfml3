#ifndef TESTENEMY_O
#define TESTENEMY_O

#include <thread>
#include <unistd.h>

#include "enemy.h"
#include "sign.h"
#include "blood.h"
#include "txmap.h"

class Test_Enemy : public Enemy {
private:
	sf::Sprite sp;
	Blood blood;
	int invFrames;
	int health;
	bool dead;

public:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
	virtual sf::FloatRect bounds();
	virtual void takeDamage();
	virtual void die();
	virtual bool remove();
	virtual void updatef(vector<sf::FloatRect>* geo, double frameTime);
	void place(sf::Vector2f p);
	Test_Enemy();
};

#endif
