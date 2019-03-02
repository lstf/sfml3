#ifndef _BUTTON_H
#define _BUTTON_H

#include <string>

#include <SFML/Graphics.hpp>

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
	sf::Color normal;
	sf::Color hover;

	sf::RectangleShape body;

	sf::Text text;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	sf::IntRect bounds;

	BState handle_input(sf::Event &event, sf::Vector2i m_pos);

	void setPosition(sf::Vector2f pos);

	void setString(string str);

	sf::Vector2f getPosition();

	Button(sf::Color n, sf::Color h, string t, sf::FloatRect r);
};

#endif
