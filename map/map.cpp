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

void Map::add_geometry(const sf::Vector2f &_pos, const sf::Vector2f &_size) {
	geometry.push_back(sf::FloatRect(_pos, _size));
}

void Map::add_sp(string name, const sf::Vector2f _pos, int l) {
	img* im = new img;
	im->sp.setPosition(_pos);
	im->name = name;
	im->sp.setTexture(*txmap::get_tx(name));
	sp[l].push_back(im);
}

void Map::add_ent_spawn(EntitySpawner* spawn) {
	ent_spawn.push_back(spawn);
}

void Map::remove_ent_spawn(EntitySpawner* spawn) {
	bool found = false;;

	for (int i = ent_spawn.size() - 1; i >= 0; i--) {
		if (spawn == ent_spawn[i]) {
			ent_spawn.erase(ent_spawn.begin() + i);
			found = true;
		}
	}

	if (!found) {
		cout << "[MAP] request for removal of nonexistant spawner" << endl;
	}
}

bool Map::save() {
	ofstream map_file((MAP_DIR + name).c_str(), ios::out|ios::binary);

	if (!map_file.is_open()) {
		return false;
	}

	write_string(name, map_file);
	write_state(init_lstate, map_file);
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
	int length = Portal::list.size();

	write_int(length, out);
	for (int i = 0; i < length; i++) {
		Portal::list[i]->write(out);
	}
}
void Map::write_entities(ofstream &out) {
	int length = ent_spawn.size();

	write_int(length, out);
	for (int i = 0; i < length; i++) {
		ent_spawn[i]->write(out);
	}
}

bool Map::load_init_lstate() {
	ifstream map_file((MAP_DIR + name).c_str(), ios::in|ios::binary);

	if (!map_file.is_open()) {
		return false;
	}

	read_string(name, map_file);
	read_state(init_lstate, map_file);

	map_file.close();

	return true;
}

bool Map::load() {
	ifstream map_file((MAP_DIR + name).c_str(), ios::in|ios::binary);

	if (!map_file.is_open()) {
		return false;
	}

	read_string(name, map_file);
	read_state(init_lstate, map_file);
	read_sp(map_file);
	read_geometry(map_file);
	read_portals(map_file);
	read_entities(map_file);

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
void Map::read_portals(ifstream &inp) {
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

	ent_spawn.clear();
	read_int(length, inp);
	for (int i = 0; i < length; i++) {
		ent_spawn.push_back(read_spawner(inp));
		new_ent(ent_spawn.back());
	}
}

vector<sf::FloatRect>* Map::get_geom() {
	return &geometry;
}

Map::Map(string _name) {
	name = _name;

	deco = true;
	geom = false;

	background = new Background(0,1920);
}

Map::~Map() {
	
}
