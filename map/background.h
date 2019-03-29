#ifndef _BACKGROUND_H
#define _BACKGROUND_H

#include <vector>

#include <SFML/Graphics.hpp>

#include "../window.h"
#include "../logger.h"
#include "../utils/txmap.h"

using namespace std;

struct bg_layer {
	sf::Sprite sprite;
	sf::Vector2u size;
};

class Background : public sf:: Drawable {
public:
	int begin;
	int end;
	vector<bg_layer*> tx;

	Background(int b, int e);

	~Background();

private:
	void draw(sf::RenderTarget& w, sf::RenderStates states) const;
};

#endif
