#ifndef _GAME_H
#define _GAME_H

using namespace std;

#include <SFML/Graphics.hpp>

#include "utils/txmap.h"
#include "actors/entity.h"
#include "actors/enemy.h"
#include "actors/player.h"
#include "actors/portal.h"
#include "map/map.h"
#include "ui/dbox.h"

struct GameTrans {
	string name;
	sf::Vector2f position;
};

enum State {
	GAMEPLAY,
	DIALOG,
	PORTAL,
	TRANSITION
};

class Game : public sf::Drawable {
	friend class Editor;
	friend class Decopanel;
	friend class Geompanel;
	friend class Mappanel;
private:
	sf::Clock	frame_clock;
	double		frame_time;
	sf::Text	frame_rate_text;
	bool		show_fps;

	DBox* dbox;
	MapTrans* map_trans;

	GameTrans* trans;

	Null_Entity ent;
	Null_Enemy enm;
	Null_Portal por;

	Map* map_current;
	vector<string> map_names;

	State state;

	map<string, int> level_state;
	map<string, int> global_state;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
public:
	Player player;

	bool init();

	void frame_calc();

	void handle_input(sf::Event &event);

	GameTrans* update();

	void clear();

	bool load_map(string name);

	void new_map(string name);

	void save_maplist();

	void resetState();
};

#endif
