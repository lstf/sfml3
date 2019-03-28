#ifndef _ITEMUTILS_H
#define _ITEMUTILS_H

#include <fstream>
#include <string>

#include "inventory.h"
#include "keyitem.h"
#include "../utils/ioutils.h"

using namespace std;

void delete_item(Item* item);

Item* new_item(string category, string name);

string item_ent_sp(string category, string name);

void write_item(Item* item, ofstream &out);

void read_item(Item* &item, ifstream &inp);

#endif
