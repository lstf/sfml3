#ifndef _ENTUTILS_H
#define _ENTUTILS_H

#include <fstream>
#include <iostream>

#include "entity.h"
#include "itement.h"
#include "keylock.h"

using namespace std;

void delete_ent(Entity* ent);

void read_ent(ifstream &inp);

#endif
