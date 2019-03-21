#ifndef _WINDOW_H
#define _WINDOW_H

#include <iostream>

#include <SFML/Graphics.hpp>

using namespace std;

sf::View letterbox(sf::View view, int w, int h);

class Window {
private:
	static int w;
	static int h;
public:
	static sf::Vector2f offset;
	static float scale;
	static sf::View default_view;

	static void init(sf::View view);

	static void set_size(int _w, int _h);
};


#endif
