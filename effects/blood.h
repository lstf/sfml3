#ifndef _BLOOD_H
#define _BLOOD_H

#include <vector>
#include <list>

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <SFML/Graphics.hpp>

using namespace std;


class Blood_drop : public sf::Drawable
{
	private:
		sf::RectangleShape r;
		sf::Vector2f velocity;
		int drip;
		int fallA;
		float fallM;

		void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	public:
		bool stationary;

		Blood_drop(sf::Vector2f pos, float theta, float var, float vel);

		void update(vector<sf::FloatRect>* geo, double frameTime);
};

class Blood : public sf::Drawable
{
	private:
		list<Blood_drop> bloods;
		int maxbloods;

		void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	public:
		Blood();

		void update(vector<sf::FloatRect>* geo, double frameTime);

		void shoot_blood(int quantity, sf::Vector2f pos, float theta, float var, float vel);
};

#endif
