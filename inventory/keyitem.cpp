#include "keyitem.h"

string key_item_ent_sp(string name) {
	if (name == "test key") {
		return "./ats/entities/keyent.png";
	} else {
		cout << "[ITEM] request for ent sp from nonexistant key " << name <<
		endl;
	}
	return "";
}

KeyItem* new_key_item(string name) {
	KeyItem* ret = NULL;
	if (name == "test key") {
		ret = new KeyItem();
		ret->name = "test key";
		ret->description = "a key that unlocks a door in test";
	} else {
		cout << "[ITEM] request for nonexistant key " << name << endl;
	}
	return ret;
}

KeyItem::KeyItem() {
	category = "key";
}

KeyItem::~KeyItem() {
}
