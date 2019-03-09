#ifndef _ENEMY_H
#define _ENEMY_H

#include <fstream>
#include <vector>

#include <SFML/Graphics.hpp>

using namespace std;

class Enemy : public sf::Drawable {
private:
	int health;
	int maxhealth;
	bool dead;

public:
	string name;
	static vector<Enemy*> list;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const = 0;

	virtual sf::FloatRect bounds() = 0;

	virtual void takeDamage() = 0;

	virtual void die() = 0;

	virtual bool remove() = 0;

	virtual void updatef(vector<sf::FloatRect>* geo, double frameTime) = 0;

	void update(vector<sf::FloatRect>* geo, double frameTime);

	Enemy();

	~Enemy();
};

#endif
