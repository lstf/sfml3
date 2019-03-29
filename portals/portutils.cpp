#include "portutils.h"

void new_portal(PortalSpawner* spawn) {
	string name = spawn->name;
	if (name == "door") {
		new_door((DoorSpawner*)spawn);
	} else {
		log_err("unknown name '" << name << "'");
	}
}

PortalSpawner* read_portal_spawner(ifstream &inp) {
	string name;
	read_string(name, inp);
	if (name == "door") {
		return read_door_spawner(inp);
	} else {
		log_err("unknown name '" << name << "'");
	}

	return NULL;
}

void delete_portal(Portal* port) {
	string name = port->name;
	if (name == "door") {
		delete (Door*)port;
	} else {
		log_err("unknown name '" << name << "'");
	}
}
