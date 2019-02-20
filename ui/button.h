#ifndef _BUTTON_O
#define _BUTTON_O

#include <string>

#include <SFML/Graphics.hpp>

#include "../utils/txmap.h"

using namespace std;

enum BState {
	BNONE,
	BHOVER,
	BCLICK
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

	sf::Vector2f getPosition();

	Button(sf::Color n, sf::Color h, string t, sf::FloatRect r);
};

#endif
