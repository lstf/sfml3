#ifndef _MAP_H
#define _MAP_H

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "background.h"
#include "../logger.h"
#include "../player.h"
#include "../enemies/enemy.h"
#include "../entities/entutils.h"
#include "../portals/portutils.h"
#include "../utils/ioutils.h"

#define MAP_DIR "./ats/mps/"
#define TEX_DIR "./ats/mps/tx/"

#define MAP_SP_LAYERS 5

using namespace std;

struct img {
	string name;
	sf::Sprite sp;
};

class Map : public sf::Drawable {
#ifdef EDITOR_BUILD
friend class Editor;
friend class Decopanel;
friend class Geompanel;
friend class Portpanel;
friend class Ilstpanel;
friend class Entpanel;
#endif
public:
	static vector<sf::FloatRect>* geom;
	map<string, int> init_lstate;
	string name;

	Map(string _name);

	#ifdef EDITOR_BUILD
	bool save();
	#endif

	bool load_init_lstate();

	bool load();

	~Map();

private:
	vector<EntitySpawner*> ent_spawn;
	vector<PortalSpawner*> port_spawn;

	bool draw_deco;
	vector<img*> sp[MAP_SP_LAYERS];
	Background* background;

	bool draw_geom;
	vector<sf::FloatRect> geometry;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	#ifdef EDITOR_BUILD
	void add_ent_spawn(EntitySpawner* spawn);

	void remove_ent_spawn(EntitySpawner* spawn);

	void add_port_spawn(PortalSpawner* spawn);

	void remove_port_spawn(PortalSpawner* spawn);

	void add_sp(string s, const sf::Vector2f p, int l);

	void add_geometry(const sf::Vector2f &p, const sf::Vector2f &s);

	void write_ent_spawn(ofstream &out);

	void write_port_spawn(ofstream &out);

	void write_sp(ofstream &out); 
	
	void write_geometry(ofstream &out);
	#endif

	void read_ent_spawn(ifstream &inp);

	void read_port_spawn(ifstream &inp);

	void read_sp(ifstream &inp);

	void read_geometry(ifstream &inp);
};

#endif
