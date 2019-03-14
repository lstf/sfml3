#ifndef _INVENTORY_H
#define _INVENTORY_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "item.h"
#include "itemutils.h"

#define INVENTORYCATEGORIES 8
#define INV_CONS 0
#define INV_VALU 1
#define INV_WEAP 2
#define INV_AMMO 3
#define INV_ARMO 4
#define INV_KEYS 5
#define INV_QUES 6
#define INV_MODS 7

using namespace std;

struct ItemQuantity {
	Item* item;
	int count;
};

struct Inventory {
private:
public:
	vector<ItemQuantity> keys;

	void addItem(Item* itm, int count);
};

#endif
