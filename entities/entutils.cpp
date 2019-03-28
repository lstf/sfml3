#include "entutils.h"

void delete_ent(Entity* ent) {
	string name = ent->name;
	if (name == "item") {
		delete (ItemEnt*)ent;
	} else if (name == "keylock") {
		delete (KeyLock*)ent;
	} else {
		cout << "[ENTU] delete requested on unknown entity " << name << endl; 
	}
}

EntitySpawner* read_spawner(ifstream &inp) {
	string name;

	read_string(name, inp);
	if (name == "item") {
		return read_itement_spawner(inp);
	} else if (name == "keylock") {
		return read_keylock_spawner(inp);
	} else {
		cout << "[ENTU] read requested on unkown entity spanwer" << name <<
		endl; 
	}

	return NULL;
}

void new_ent(EntitySpawner* spawn) {
	if (spawn->name == "item") {
		new_itement_ent((ItemEntSpawner*)spawn);
	} else if (spawn->name == "keylock") {
		new_keylock_ent((KeyLockSpawner*)spawn);
	} else {
		cout << "[ENTU] request for new ent of unknown type " << spawn->name <<
		endl;
	}
}
