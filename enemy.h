#ifndef ENEMY_O
#define ENEMY_O

#include <vector>

#include <SFML/Graphics.hpp>


using namespace std;

class Enemy : public sf::Drawable {
private:
	int health;
	int maxhealth;
	bool dead;
public:
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

class Null_Enemy : public Enemy {
private:
public:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const {
		(void)w;
		(void)states;
	}

	virtual sf::FloatRect bounds() {
		return sf::FloatRect(0,0,0,0);
	}

	virtual void takeDamage() {
	}

	virtual void die() {
	}

	virtual bool remove() {
		return false;
	}

	virtual void updatef(vector<sf::FloatRect>* geo, double frameTime) {
		(void)geo;
		(void)frameTime;
	}

	void update(vector<sf::FloatRect>* geo, double frameTime) {
		(void)geo;
		(void)frameTime;
	}
};

#endif
