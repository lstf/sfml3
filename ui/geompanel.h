#ifndef _GEOMPANEL_O
#define _GEOMPANEL_O

#include <vector>

#include <SFML/Graphics.hpp>

#include "uiutils.h"
#include "../game.h"
#include "../map/map.h"

using namespace std;

class Geompanel : public sf::Drawable {
private:
	sf::RectangleShape select_r;
	bool select_click;
	bool selected_click;
	bool right_click;
	sf::Vector2f select_p_mouse;
	vector<int> selected;

	Game* game;
	Map* map;
	
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
public:
	void handle_input(
	sf::Event &event, sf::Vector2f w_pos, int snap_val);

	void reset();

	Geompanel(Game* _game);
};

#endif
