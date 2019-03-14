#include "sparkle.h"

void SparkleParticle::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(r, states);
}

SparkleParticle::SparkleParticle(sf::Vector2f pos) {
	life = 1;

	r.setSize(sf::Vector2f(1, 1));
	r.setPosition(pos);
	r.setFillColor(sf::Color::White);

	velocity.y = -5;
	velocity.x = rand() % 32;
	if (rand() % 2) {
		velocity.x *= -1;
	}
}

void SparkleParticle::update() {
	if (velocity.x > 0) {
		velocity.x -= 100 * World::frame_time;
	} else if (velocity.x < 0) {
		velocity.x += 100 * World::frame_time;
	}

	r.move(velocity * (float)World::frame_time);

	life -= World::frame_time;
}

SparkleParticle::~SparkleParticle() {
	
}

void Sparkle::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	for (int i = particles.size() - 1; i >= 0; i--) {
		if (!(rand() % 3)) {
			w.draw(*particles.at(i), states);
		}
	}
}

Sparkle::Sparkle() {
	particles.set_size(SPARKLE_P_COUNT);
	sparkle_counter = 0;
}

void Sparkle::update() {
	sparkle_counter++;
	if (sparkle_counter == 3) {
		sparkle_counter = 0;
		particles.push(new SparkleParticle(sf::Vector2f(
			r.left + rand() % r.width,
			r.top + rand() % r.height
		)));
	}

	for (int i = particles.size() - 1; i >= 0; i--) {
		particles.at(i)->update();
	}

	if (particles.size() != 0 && particles.front()->life <= 0) {
		SparkleParticle* spp = particles.front();
		delete spp;
		particles.pop();
	}
}

void Sparkle::set_rect(sf::IntRect _r) {
	r = _r;
}

void Sparkle::set_position(sf::Vector2f p) {
	r.left = p.x;
	r.top = p.y;
}

Sparkle::~Sparkle() {
	cout << "deleting sparkle" << endl;
	for (int i = particles.size() - 1; i >= 0; i--) {
		delete particles.at(i);
	}
	cout << "done deleting sparkle" << endl;
}
