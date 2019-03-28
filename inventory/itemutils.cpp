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

void write_item(Item* item, ofstream &out) {
	write_string(item->category, out);
	write_string(item->name, out);
}

void read_item(Item* &item, ifstream &inp) {
	string category;
	string name;

	read_string(category, inp);
	read_string(name, inp);

	item = new_item(category, name);
}
