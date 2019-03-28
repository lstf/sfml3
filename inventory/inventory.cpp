#include "inventory.h"

void Inventory::addItem(Item* itm, int count) {
	if (itm->category == "key") {
		bool found = false;
		for (auto it = keys.begin(); it != keys.end(); ++it) {
			if (it->item->name == itm->name) {
				found = true;
				it->count += count;
				delete_item(itm);
				break;
			}
		}
		if (!found) {
			ItemQuantity iq;
			iq.item = itm;
			iq.count = count;
			keys.push_back(iq);
		}
		vector<int> remove_index;
		for (int i = keys.size() - 1; i >= 0; i--) {
			if (keys.at(i).count == 0) {
				remove_index.push_back(i);
			} else if (keys.at(i).count < 0) {
				remove_index.push_back(i);
				cout << "[INV] Negative item quantity detected" << endl;
			}
		}
		for (auto it = remove_index.begin(); it != remove_index.end(); ++it) {
			delete_item(keys.at(*it).item);
			keys.erase(keys.begin() + *it);
		}
	}
}

void Inventory::write(ofstream &out) {
	int length = keys.size();

	write_int(length, out);
	for (auto it = keys.begin(); it != keys.end(); it++) {
		write_item(it->item, out);
		write_int(it->count, out);
	}
}

void Inventory::read(ifstream &inp) {
	int length;
	ItemQuantity iq;

	keys.clear();
	read_int(length, inp);
	for (int i = 0; i < length; i++) {
		read_item(iq.item, inp);
		read_int(iq.count, inp);
		keys.push_back(iq);
	}
}
