#include "enemy.h"

vector<Enemy*> Enemy::list;

void Enemy::update(vector<sf::FloatRect>* geo, double frameTime) {
	if (isPaused()) {
		return;
	}
	updatef(geo, frameTime);
}

Enemy::Enemy() {
	list.push_back(this);
}

Enemy::~Enemy() {
	for (auto it = list.begin(); it != list.end(); ++it) {
		if (*it == this)
		{
			list.erase(it);
			return;
		}
	}
}
