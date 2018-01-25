#ifndef _WINDOW_DEFINED
#define _WINDOW_DEFINED

#include <SFML/Graphics.hpp>
#include "font.h"

namespace w 
{
	//Program wide window
	static sf::RenderWindow window(sf::VideoMode(960, 480), "SFML works!");

	//window frame time, must be set by user
	static float frame_time;
	static sf::Text frame_rate;
	static bool show_fps;


	void frame_calc(float dt)
	{
		float fps = 1/dt;
		frame_time = dt;
		if (show_fps) frame_rate.setString(std::to_string(fps));
	}

	int init()
	{
		frame_rate.setFont(font::thintel);
		frame_rate.setString("0");
		frame_rate.setCharacterSize(20);
		frame_rate.setFillColor(sf::Color::Yellow);

		return 0;
	}
}

#endif
