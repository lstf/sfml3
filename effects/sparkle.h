#ifndef _SPARKLE_H
#define _SPARKLE_H

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <SFML/Graphics.hpp>

#include "../world.h"
#include "../utils/cqueue.h"

#define SPARKLE_X_WOBBLE 5
#define SPARKLE_Y_VEL -5
#define SPARKLE_P_COUNT 100

using namespace std;

class SparkleParticle : public sf::Drawable {
private:
	sf::RectangleShape r;
	sf::Vector2f velocity;

	void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	float life;

	SparkleParticle(sf::Vector2f pos);
	
	void update();

	~SparkleParticle();
};

class Sparkle : public sf::Drawable {
private:
	sf::IntRect r;
	int sparkle_counter;

	CQueue<SparkleParticle*> particles;
	
	void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	Sparkle();

	void update();

	void set_rect(sf::IntRect _r);

	void set_position(sf::Vector2f p);

	~Sparkle();
};

#endif
