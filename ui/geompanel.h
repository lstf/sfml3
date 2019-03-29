#ifdef EDITOR_BUILD
#ifndef _GEOMPANEL_H
#define _GEOMPANEL_H

#include <vector>

#include <SFML/Graphics.hpp>

#include "uiutils.h"
#include "../game.h"
#include "../map/map.h"

using namespace std;

class Geompanel : public sf::Drawable {
private:
	Game* game;
	Map* map;
	SnapVals* sv;

	sf::RectangleShape	select_r;
	bool				select_click;
	bool				selected_click;
	sf::Vector2f		select_p_mouse;
	vector<int>			selected;

	bool right_click;
	
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	void handle_input(
	sf::Event &event, sf::Vector2f w_pos);

	void reset();

	Geompanel(Game* _game, SnapVals* _sv);
};

#endif
#endif
