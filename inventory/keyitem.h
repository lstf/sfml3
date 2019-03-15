#ifndef _KEYITEM_H
#define _KEYITEM_H

#include <fstream>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include "item.h"
#include "../utils/ioutils.h"

using namespace std;

class KeyItem final : public Item {
private:

public:
	KeyItem();

	virtual void write(ofstream &out);

	virtual void read(ifstream &inp);

	~KeyItem();
};

string key_item_ent_sp(string name);

KeyItem* new_key_item(string name);

#endif
