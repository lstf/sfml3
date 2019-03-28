#ifndef _ENTUTILS_H
#define _ENTUTILS_H

#include <fstream>
#include <iostream>

#include "entity.h"
#include "itement.h"
#include "keylock.h"

using namespace std;

void delete_ent(Entity* ent);

EntitySpawner* read_spawner(ifstream &inp);

void new_ent(EntitySpawner* spawner);

#endif
