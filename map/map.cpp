#include "map.h"

void Map::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	if (deco) {
		w.draw(*background, states);
		for (int j = 0; j < MAP_SP_LAYERS; j++) {
			for (int i = (int)sp[j].size() - 1; i >= 0; i--) {
				w.draw(sp[j][i]->sp, states);
			}
		}
	}
	if (geom) {
		sf::RectangleShape gRect;
		sf::Vector2f gV;

		gRect.setOutlineThickness(-1.0);
		gRect.setOutlineColor(sf::Color::Blue);
		gRect.setFillColor(sf::Color::Transparent);

		for (int i = (int)geometry.size() - 1; i >= 0; i--) {
			gV.x = geometry[i].left;
			gV.y = geometry[i].top;
			gRect.setPosition(gV);
			gV.x = geometry[i].width;
			gV.y = geometry[i].height;
			gRect.setSize(gV);

			w.draw(gRect, states);
		}
	}
}

bool Map::add_geometry(const sf::Vector2f &_pos, const sf::Vector2f &_size) {
	geometry.push_back(sf::FloatRect(_pos, _size));
	return true;
}

bool Map::add_sp(string name, const sf::Vector2f _pos, int l) {
	img* im = new img;
	im->sp.setPosition(_pos);
	im->name = name;
	im->sp.setTexture(*txmap::get_tx(name));
	sp[l].push_back(im);

	return true;
}

bool Map::save() {
	ofstream map_file((MAP_DIR + name).c_str(), ios::out|ios::binary);

	if (!map_file.is_open()) {
		return false;
	}


	write_string(name, map_file);
	write_sp(map_file);
	write_geometry(map_file);
	write_portals(map_file);
	write_entities(map_file);

	map_file.close();
	
	return true;
}
void Map::write_sp(ofstream &out) {
	for (int i = 0; i < MAP_SP_LAYERS; i++) {
		int length = sp[i].size();
		write_int(length, out);
		for (int j = 0; j < length; j++) {
			write_string(sp[i][j]->name, out);
			write_vec2(sp[i][j]->sp.getPosition(), out);
		}
	}
}
void Map::write_geometry(ofstream &out) {
	int length = geometry.size();

	write_int(length, out);
	for (int i = 0; i < length; i++) {
		write_rect(geometry[i], out);
	}
}
void Map::write_portals(ofstream &out) {
	int length = por->list.size();

	write_int(length - 1, out);
	for (int i = 1; i < length; i++) {
		por->list[i]->write(out);
	}
}
void Map::write_entities(ofstream &out) {
	int length = ent->list.size();

	write_int(length - 1, out);
	for (int i = 1; i < length; i++) {
		ent->list[i]->write(out);
	}
}

bool Map::load() {
	ifstream map_file((MAP_DIR + name).c_str(), ios::in|ios::binary);

	if (!map_file.is_open()) {
		return false;
	}


	read_string(name, map_file);
	read_sp(map_file);
	read_geometry(map_file);
	read_doors(map_file);
	read_entities(map_file);

	//TODO read init_lstate;
	if (lstate == NULL) {
		lstate = &init_lstate;
	}

	map_file.close();

	return true;
}
void Map::read_sp(ifstream &inp) {
	for (int i = 0; i < MAP_SP_LAYERS; i++) {
		sp[i].clear();

		int length;
		read_int(length, inp);
		for (int j = 0; j < length; j++) {
			string name;
			sf::Vector2f pos;

			read_string(name, inp);
			read_vec2(pos, inp);

			sp[i].push_back(new img);
			sp[i].back()->sp.setTexture(*txmap::get_tx(name));
			sp[i].back()->name = name;
			sp[i].back()->sp.setPosition(pos);
		}
	}
}
void Map::read_geometry(ifstream &inp) {
	int length;
	sf::FloatRect r;

	geometry.clear();

	read_int(length, inp);
	for (int i = 0; i < length; i++) {
		read_rect(r, inp);
		geometry.push_back(r);
	}
}
void Map::read_doors(ifstream &inp) {
	int length;
	string por_name;

	read_int(length, inp);
	for (int i = 0; i < length; i++) {
		read_string(por_name, inp);
		if (por_name == "door") {
			Door* new_door = new Door;
			new_door->read(inp);
		}
	}
}
void Map::read_entities(ifstream &inp) {
	int length;
	string ent_name;

	read_int(length, inp);
	for (int i = 0; i < length; i++) {
		read_string(ent_name, inp);
		if (ent_name == "key") {
			KeyItemEnt* new_ent = new KeyItemEnt;
			new_ent->read(inp);
		} else if (ent_name == "keylock") {
			KeyLock* new_ent = new KeyLock;
			new_ent->read(inp);
		}
	}
}

vector<sf::FloatRect>* Map::get_geom() {
	return &geometry;
}

void Map::set_lstate(map<string, int>* _lstate) {
	lstate = _lstate;
}

Map::Map(string _name, Null_Enemy* _enm, Null_Entity* _ent,
Null_Portal* _por) {
	name = _name;
	enm = _enm;
	ent = _ent;
	por = _por;

	deco = true;
	geom = false;

	lstate = NULL;

	background = new Background(0,1920);
}

Map::~Map() {
	
}
