#include "blood.h"

void Blood_drop::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(r, states);
}

Blood_drop::Blood_drop(sf::Vector2f pos, float theta, float var, float vel) {
	stationary = false;
	if (var == 0) {
		var = 0.001;
	}
	theta += 1.55;
	theta += (float(rand() % int(var/2*1000))/1000) * (rand()%2==0?1:-1);
	velocity.y = vel * cos(theta) * (float(rand() % int(var*1000))/1000);
	velocity.x = vel * sin(theta) * (float(rand() % int(var*1000))/1000);

	fallA = 10;
	fallM = 10.0;
	
	r.setPosition(pos);
	r.setSize(sf::Vector2f(rand() % 2 + 1, rand() % 2 + 1));
	r.setFillColor(sf::Color::Red);
}

void Blood_drop::update(vector<sf::FloatRect>* geo, double frameTime) {
	if (!stationary) {
		velocity.y += frameTime*fallA;
		velocity.y = velocity.y > fallM ? fallM : velocity.y; 
		r.move(velocity);
		for (auto it = geo->begin(); it != geo->end(); it++) {
			if (r.getGlobalBounds().intersects(*it) &&
			(rand() % 4) == 0) {
				stationary = true;
			}
		}
	}
}

void Blood::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	for (auto it = bloods.begin(); it != bloods.end(); it++) {
		w.draw(*it, states);
	}
}


Blood::Blood() {
	maxbloods = 256;
}

void Blood::update(vector<sf::FloatRect>* geo, double frameTime) {
	for (auto it = bloods.begin(); it != bloods.end(); it++) {
		if (!(*it).stationary) {
			(*it).update(geo, frameTime);
		}
	}
}

void Blood::shoot_blood(int quantity, sf::Vector2f pos, float theta,
float var, float vel) {
	for (int i = 0; i < quantity; i++) {
		bloods.push_front(Blood_drop(pos,theta,var,vel));
	}
	for (int i = bloods.size() - maxbloods; i > 0; i--) {
		bloods.pop_back();
	}
}
