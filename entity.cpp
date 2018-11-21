#include "entity.h"

DBox** Entity::db;
vector<Entity*> Entity::list;

void Entity::setMainDBox(DBox** d) {
	db = d;
}

void Entity::dialogBox(string file) {
	*db = new DBox(db, file);	
}

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
