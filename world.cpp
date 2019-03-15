#include "world.h"

double World::frame_time;

map<string, map<string, int>> World::lstates;
map<string, int>* World::lstate;
map<string, int>* World::gstate;
