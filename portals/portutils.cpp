#include "portutils.h"

void delete_portal(Portal* port) {
	string name = port->name;
	if (name == "door") {
		delete (Door*)port;
	} else {
		cout << "[PORTU] request for delete on nameless portal" << endl;
	}
}
