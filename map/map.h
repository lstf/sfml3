#ifndef _MAP_H
#define _MAP_H
#include <map>
#include <vector>
#include <iterator>
#include <string>
#include <fstream>
#include <unordered_set>
#include <set>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "background.h"
#include "../player.h"
#include "../enemies/enemy.h"
#include "../entities/entity.h"
#include "../entities/keylock.h"
#include "../inventory/keyitem.h"
#include "../portals/portal.h"
#include "../portals/door.h"
#include "../utils/ioutils.h"

#define MAP_DIR "./ats/mps/"
#define TEX_DIR "./ats/mps/tx/"

#define MAP_SP_LAYERS 5

using namespace std;

struct img {
	string name;
	sf::Sprite sp;
};

struct named_tx {
	void* png;
	sf::Texture* texture;
	string name;
};

class Map : public sf::Drawable {
	friend class Editor;
	friend class Decopanel;
	friend class Geompanel;
	friend class Portpanel;

private:
	Null_Enemy* enm;
	Null_Entity* ent;
	Null_Portal* por;

	map<string, int> init_lstate;
	map<string, int>* lstate;

	vector<img*> sp[MAP_SP_LAYERS];
	vector<sf::FloatRect> geometry;
	Background* background;

	bool deco;		//Draw environment sprites
	bool geom; 		//Draw geometry

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	bool add_geometry(const sf::Vector2f &_pos, const sf::Vector2f &_size);

	bool add_sp(string name, const sf::Vector2f _pos, int l);

	//save() load() helpers
	void write_sp(ofstream &out);
	void write_geometry(ofstream &out);
	void write_portals(ofstream &out);
	void write_entities(ofstream &out);
	void read_sp(ifstream &inp);
	void read_geometry(ifstream &inp);
	void read_doors(ifstream &inp);
	void read_entities(ifstream &inp);


public:
	string name;

	Map(string _name, Null_Enemy* _enm, Null_Entity* _ent,
	Null_Portal* _por);

	vector<sf::FloatRect>* get_geom();

	void set_lstate(map<string, int>* _lstate);

	bool save();

	bool load();

	~Map();
};

#endif
