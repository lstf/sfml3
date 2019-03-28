#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <string>
#include <float.h>

#include <SFML/Graphics.hpp>

#include "logger.h"
#include "utils/txmap.h"

#define MAX_ANIMATIONS 10

using namespace std;

class Animation {
private:
	Png		png;

	int	x;
	int	y;

	sf::Clock	clock;
	float		time;
	float		frame_time[MAX_ANIMATIONS];
	int 		frame_count[MAX_ANIMATIONS];

	void refresh_tx();

public:
	sf::Texture tx;

	int w;
	int h;

	Animation();

	void set_fps(float fps, int a = 0);

	void set_frame_count(int n, int a = 0);

	void set_png(string name);

	void set_animation(int a);

	void restart();

	bool advance();

	~Animation();
};

#endif 
