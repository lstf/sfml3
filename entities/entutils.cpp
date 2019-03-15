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

void read_ent(ifstream &inp) {
	string name;

	read_string(name, inp);
	if (name == "item") {
		read_item_ent(inp);
	} else if (name == "keylock") {
		read_keylock_ent(inp);
	} else {
		cout << "[ENTU] read requested on unkown entity " << name << endl; 
	}
}
