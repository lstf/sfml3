#ifndef _WINDOW_DEFINED
#define _WINDOW_DEFINED

#include <SFML/Graphics.hpp>

namespace w {
	//Program wide window
	static sf::RenderWindow window(sf::VideoMode(960, 480), "SFML works!");

	//window frame time, must be set by user
	static double frame_time;
	static sf::Text frame_rate;
	static bool show_fps;


	void frame_calc(long long dt) {
		frame_time = dt/1000000.0;
		if (show_fps) {
			frame_rate.setString(std::to_string(1/frame_time));
		}
	}

	int init(sf::Font &_font) {
		frame_rate.setFont(_font);
		frame_rate.setString("0");
		frame_rate.setCharacterSize(20);
		frame_rate.setFillColor(sf::Color::Yellow);

		return 0;
	}
}

#endif
