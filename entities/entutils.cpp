#include "entutils.h"

void delete_ent(Entity* ent) {
	string name = ent->name;
	if (name == "keylock") {
		delete (KeyLock*)ent;
	} else if (name == "key") {
		delete (KeyItemEnt*)ent;
	} else {
		cout << "[ENTU] delete requested on nameless entity" << endl; 
	}
}
