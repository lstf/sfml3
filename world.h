#ifndef _WORLD_H
#define _WORLD_H

#include <map>
#include <string>

using namespace std;

struct World {
	static double frame_time;

	static map<string, map<string, int>> lstates;
	static map<string, int>* lstate;
	static map<string, int>* gstate;
};

#endif
