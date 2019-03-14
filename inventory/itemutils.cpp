#include "itemutils.h"

void delete_item(Item* item) {
	if (item->category == "key") {
		delete (KeyItem*)item;
	}
}
