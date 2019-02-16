#include "entity.h"

vector<Entity*> Entity::list;

Entity::Entity() {
	list.push_back(this);
}

Entity::~Entity() {
	for (auto it = list.begin(); it != list.end(); ++it) {
		if (*it == this)
		{
			list.erase(it);
			return;
		}
	}
}
