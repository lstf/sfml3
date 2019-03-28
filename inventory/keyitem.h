#ifndef _KEYITEM_H
#define _KEYITEM_H

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "item.h"

using namespace std;

class KeyItem final : public Item {
private:

public:
	KeyItem();

	~KeyItem();
};

string key_item_ent_sp(string name);

KeyItem* new_key_item(string name);

#endif
