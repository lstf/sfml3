#include "portal.h"

std::vector<Portal*> Portal::list;

Portal::Portal() {
	list.push_back(this);
}

Portal::~Portal() {
	for (auto it = list.begin(); it != list.end(); ++it) {
		if (*it == this)
		{
			list.erase(it);
			return;
		}
	}
}
