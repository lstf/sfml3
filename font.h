#ifndef _FONT_H
#define _FONT_H
#include <string>

#include <SFML/Graphics.hpp>

namespace font
{
	sf::Font thintel;

	int init()
	{
		if (!thintel.loadFromFile("thintel.ttf")) return 1;
		return 0;
	}
}

#endif
