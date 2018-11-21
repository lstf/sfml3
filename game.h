#ifndef GAME_O
#define GAME_O

#include <fstream>
#include <string>

using namespace std;



class Game_State {
public:
	static bool isPaused();	
	static void pause();
	static void unpause();

	Game_State();
private:
	static bool paused;
	static int base_id;
	int id;
};


#endif
