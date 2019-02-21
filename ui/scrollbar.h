#ifndef _SCROLLBAR_O
#define _SCROLLBAR_O

#include <SFML/Graphics.hpp>

#define SCROLL_FG sf::Color(127,127,127)
#define SCROLL_BG sf::Color(0,0,0)

using namespace std;

class Scrollbar : public sf::Drawable {
private:
	sf::RectangleShape bg;
	sf::RectangleShape fg;
	bool clicked;
	float max_y;
	float y;
	sf::Vector2f p_mouse_pos;
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
public:
	float handle_input(sf::Event &event, sf::Vector2i m_pos);

	Scrollbar(int h1, int h2, int x, int y, int w, int h);
};

#endif 
