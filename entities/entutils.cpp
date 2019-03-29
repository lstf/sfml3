#include "entutils.h"

void new_entity(EntitySpawner* spawn) {
	string name = spawn->name;
	if (name == "item") {
		new_itement((ItemEntSpawner*)spawn);
	} else if (name == "keylock") {
		new_keylock((KeyLockSpawner*)spawn);
	} else {
		log_err("unknown name '" << name << "'");
	}
}

EntitySpawner* read_entity_spawner(ifstream &inp) {
	string name;
	read_string(name, inp);
	if (name == "item") {
		return read_itement_spawner(inp);
	} else if (name == "keylock") {
		return read_keylock_spawner(inp);
	} else {
		log_err("unknown name '" << name << "'");
	}

	return NULL;
}

void delete_entity(Entity* ent) {
	string name = ent->name;
	if (name == "item") {
		delete (ItemEnt*)ent;
	} else if (name == "keylock") {
		delete (KeyLock*)ent;
	} else {
		log_err("unknown name '" << name << "'");
	}
}
