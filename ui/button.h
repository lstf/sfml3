#ifndef _BUTTON_H
#define _BUTTON_H

#include <string>

#include <SFML/Graphics.hpp>

#include "uicolors.h"
#include "../utils/txmap.h"

using namespace std;

enum BState {
	BNONE,
	BHOVER,
	BCLICK,
	BCLICKR
};

class Button : public sf::Drawable {
private:
	sf::RectangleShape body;

	sf::Text text;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	sf::Color normal_b;
	sf::Color hover_b;
	sf::Color normal_t;
	sf::Color hover_t;
	sf::Color normal_o;
	sf::Color hover_o;
	sf::IntRect bounds;

	BState handle_input(sf::Event &event, sf::Vector2i m_pos);

	void setPosition(sf::Vector2f pos);

	void setString(string str);

	void setColors(bool hover = false);

	sf::Vector2f getPosition();

	Button(string t, sf::FloatRect r);
};

#endif
