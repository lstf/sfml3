#include "itemutils.h"

void delete_item(Item* item) {
	if (item->category == "key") {
		delete (KeyItem*)item;
	}
}

Item* new_item(string category, string name) {
	if (category == "key") {
		return (Item*)new_key_item(name);
	} else {
		cout << "[ITEM] request for item " << name << 
		" in nonexistant category " << category << endl;
	}

	return NULL;
}

string item_ent_sp(string category, string name) {
	if (category == "key") {
		return key_item_ent_sp(name);
	} else {
		cout << "[ITEM] request for item " << name << 
		" in nonexistant category " << category << endl;
	}

	return "";
}
