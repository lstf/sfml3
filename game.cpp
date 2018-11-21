#include "game.h"

bool Game_State::paused;

bool Game_State::isPaused()
{
	return paused;
}

void Game_State::pause()
{
	paused = true;
}

void Game_State::unpause()
{
	paused = false;
}

Game_State::Game_State()
{
}
