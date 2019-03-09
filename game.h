#ifndef _GAME_H
#define _GAME_H

using namespace std;

#include <SFML/Graphics.hpp>

#include "utils/txmap.h"
#include "entities/entity.h"
#include "entities/entutils.h"
#include "entities/keylock.h"
#include "enemies/enemy.h"
#include "player.h"
#include "world.h"
#include "portals/portal.h"
#include "inventory/keyitem.h"
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
	friend class Entpanel;
	friend class Geompanel;
	friend class Portpanel;
	friend class Mappanel;
	friend class Ilstpanel;
	friend class Glstpanel;
private:
	sf::Clock	frame_clock;
	sf::Text	frame_rate_text;
	bool		show_fps;

	DBox* dbox;
	Entity* delete_ent_ptr;
	MapTrans* map_trans;

	GameTrans* trans;

	Map* map_current;
	vector<string> map_names;

	State state;

	map<string, map<string, int>> level_states;
	map<string, int>* level_state;
	map<string, int>* global_state;

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
