#ifndef _BLOOD_H
#define _BLOOD_H

#include <list>
#include <vector>

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <SFML/Graphics.hpp>

#include "../logger.h"

#define BLOOD_DROP_ACCEL 10.0
#define BLOOD_DROP_FALL_M 10.0
#define BLOOD_DROPS_MAX 256

using namespace std;

////////////////////////////////////////////////
//
// Particle
//
////////////////////////////////////////////////

class Blood_drop : public sf::Drawable {
	public:
		bool stationary;

		Blood_drop(sf::Vector2f pos, float theta, float var, float vel);

		void update(vector<sf::FloatRect>* geo, double frameTime);

	private:
		sf::RectangleShape r;
		sf::Vector2f velocity;
		int drip;

		void draw(sf::RenderTarget& w, sf::RenderStates states) const;
};

////////////////////////////////////////////////
//
// Effect
//
////////////////////////////////////////////////

class Blood : public sf::Drawable {
	public:
		Blood();

		void update(vector<sf::FloatRect>* geo, double frameTime);

		void shoot_blood(int quantity, sf::Vector2f pos, float theta,
		float var, float vel);

		~Blood();

	private:
		list<Blood_drop> bloods;
		int maxbloods;

		void draw(sf::RenderTarget& w, sf::RenderStates states) const;
};

#endif
