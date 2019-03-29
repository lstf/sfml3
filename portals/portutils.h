#ifndef _PORTUTILS_H
#define _PORTUTILS_H

#include <string>

#include "portal.h"
#include "door.h"
#include "../logger.h"

void new_portal(PortalSpawner* spawn);

PortalSpawner* read_portal_spawner(ifstream &inp);

void delete_portal(Portal* port);

#endif
