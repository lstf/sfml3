#ifndef _ITEMUTILS_H
#define _ITEMUTILS_H

#include "inventory.h"
#include "keyitem.h"

void delete_item(Item* item);

Item* new_item(string category, string name);

string item_ent_sp(string category, string name);

#endif
