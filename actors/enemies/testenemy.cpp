#include "testenemy.h"

void Test_Enemy::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(sp, states);
	w.draw(blood, states);
}

void Test_Enemy::takeDamage() {
	if (dead || invFrames > 0) {
		return;
	}
	
	health -= 20;

	if (health <= 0) {
		die();
		sp.setTexture(*txmap::get_tx("ats/enemy/testdead.png"));
		sp.setTextureRect(sf::IntRect(0,0,32,27));
		sp.setPosition(sp.getPosition() + sf::Vector2f(0,21));
	}

	blood.shoot_blood(32,sp.getPosition(),0,0.3,10);
	invFrames = 4;
}

void Test_Enemy::die() {
	if (dead) {
		return;
	}
	Sign* s = new Sign("ats/text/testenemydead");
	s->place(sp.getPosition() + sf::Vector2f(0,16));
	dead = true;
}

bool Test_Enemy::remove() {
	return dead;
}

void Test_Enemy::updatef(vector<sf::FloatRect>* geo, double frameTime) {
	blood.update(geo, frameTime);
	if (invFrames > 0) {
		invFrames--;
	}
}

void Test_Enemy::place(sf::Vector2f p) {
	sp.setPosition(p);
}

sf::FloatRect Test_Enemy::bounds() {
	return sp.getGlobalBounds();
}

Test_Enemy::Test_Enemy() {
	sp.setTexture(*txmap::get_tx("ats/enemy/test.png"));
	invFrames = 0;
	health = 200;
	dead = false;
}
