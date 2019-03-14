#include "keyitem.h"

void KeyItem::write(ofstream &out) {
	write_string(category, out);
	write_string(name, out);
	write_string(description, out);
	write_string(sp_name, out);
}

void KeyItem::read(ifstream &inp) {
	read_string(name, inp);
	read_string(description, inp);
	read_string(sp_name, inp);
}
	
KeyItem::KeyItem() {
	category = "key";
}

KeyItem::~KeyItem() {
}
