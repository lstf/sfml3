#include "map.h"

vector<sf::FloatRect>* Map::geom;

Map::Map(string _name) {
	log_dbg("constructing map");

	name = _name;

	draw_deco = true;
	draw_geom = false;

	background = new Background(0,1920);

	geom = &geometry;
}

#ifdef EDITOR_BUILD
bool Map::save() {
	ofstream map_file((MAP_DIR + name).c_str(), ios::out|ios::binary);

	if (!map_file.is_open()) {
		return false;
	}

	write_string(name, map_file);
	write_state(init_lstate, map_file);
	write_ent_spawn(map_file);
	write_port_spawn(map_file);
	write_sp(map_file);
	write_geometry(map_file);

	map_file.close();
	
	return true;
}
#endif

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
	read_ent_spawn(map_file);
	read_port_spawn(map_file);
	read_sp(map_file);
	read_geometry(map_file);

	map_file.close();

	return true;
}

Map::~Map() {
	log_dbg("destructing map");
}

void Map::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	if (draw_deco) {
		w.draw(*background, states);

		for (int j = 0; j < MAP_SP_LAYERS; j++) {
			for (int i = (int)sp[j].size() - 1; i >= 0; i--) {
				w.draw(sp[j][i]->sp, states);
			}
		}
	}

	if (draw_geom) {
		sf::RectangleShape gRect;
		gRect.setOutlineThickness(-1.0);
		gRect.setOutlineColor(sf::Color::Blue);
		gRect.setFillColor(sf::Color::Transparent);

		for (int i = (int)geometry.size() - 1; i >= 0; i--) {
			sf::FloatRect r = geometry[i];
			gRect.setPosition(r.left, r.top);
			gRect.setSize(sf::Vector2f(r.width, r.height));

			w.draw(gRect, states);
		}
	}
}

#ifdef EDITOR_BUILD
void Map::add_ent_spawn(EntitySpawner* spawn) {
	log_dbg("adding entity spawner");
	ent_spawn.push_back(spawn);
}

void Map::remove_ent_spawn(EntitySpawner* spawn) {
	log_dbg("removing entity spawner");
	bool found = false;;

	for (int i = ent_spawn.size() - 1; i >= 0; i--) {
		if (spawn == ent_spawn[i]) {
			ent_spawn.erase(ent_spawn.begin() + i);
			found = true;
		}
	}

	if (!found) {
		log_err("spawner not in ent_spawn");
	}
}

void Map::add_port_spawn(PortalSpawner* spawn) {
	log_dbg("adding portal spawner");
	port_spawn.push_back(spawn);
}

void Map::remove_port_spawn(PortalSpawner* spawn) {
	log_dbg("removing portal spawner");

	bool found = false;;

	for (int i = port_spawn.size() - 1; i >= 0; i--) {
		if (spawn == port_spawn[i]) {
			port_spawn.erase(port_spawn.begin() + i);
			found = true;
		}
	}

	if (!found) {
		log_err("spawner not in port_spawn");
	}
}

void Map::add_sp(string s, const sf::Vector2f p, int l) {
	log_dbg("adding sprite " << s << ": x" << p.x << " y" << p.y);

	img* im = new img;
	im->sp.setPosition(p);
	im->name = s;
	im->sp.setTexture(*txmap::get_tx(s));

	sp[l].push_back(im);
}

void Map::add_geometry(const sf::Vector2f &p, const sf::Vector2f &s) {
	log_dbg("adding geometry rect: x" << p.x << " y" <<
	p.y << " w" << s.x << " h" << s.y);

	geometry.push_back(sf::FloatRect(p, s));
}

void Map::write_ent_spawn(ofstream &out) {
	int length = ent_spawn.size();
	log_dbg("writing entity spawners: " << length);

	write_int(length, out);
	for (int i = 0; i < length; i++) {
		ent_spawn[i]->write(out);
	}
}

void Map::write_port_spawn(ofstream &out) {
	int length = port_spawn.size();
	log_dbg("writing portal spawners: " << length);

	write_int(length, out);
	for (int i = 0; i < length; i++) {
		port_spawn[i]->write(out);
	}
}

void Map::write_sp(ofstream &out) {
	for (int i = 0; i < MAP_SP_LAYERS; i++) {
		int length = sp[i].size();
		log_dbg("writing layer " << i << " sprites: " << length);

		write_int(length, out);
		for (int j = 0; j < length; j++) {
			write_string(sp[i][j]->name, out);
			write_vec2(sp[i][j]->sp.getPosition(), out);
		}
	}
}
void Map::write_geometry(ofstream &out) {
	int length = geometry.size();
	log_dbg("writing geometry: " << length);

	write_int(length, out);
	for (int i = 0; i < length; i++) {
		write_rect(geometry[i], out);
	}
}
#endif

void Map::read_ent_spawn(ifstream &inp) {
	log_dbg("reading entity spawners");

	int length;

	ent_spawn.clear();

	read_int(length, inp);
	for (int i = 0; i < length; i++) {
		ent_spawn.push_back(read_entity_spawner(inp));
		new_entity(ent_spawn.back());
	}

	log_dbg("read: " << length);
}

void Map::read_port_spawn(ifstream &inp) {
	log_dbg("reading portal spawners");

	int length;

	port_spawn.clear();

	read_int(length, inp);
	for (int i = 0; i < length; i++) {
		port_spawn.push_back(read_portal_spawner(inp));
		new_portal(port_spawn.back());
	}

	log_dbg("read: " << length);
}

void Map::read_sp(ifstream &inp) {
	for (int i = 0; i < MAP_SP_LAYERS; i++) {
		log_dbg("reading layer " << i << " sprites");
		int length;

		sp[i].clear();

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

		log_dbg("read: " << length);
	}
}

void Map::read_geometry(ifstream &inp) {
	log_dbg("reading geometry");

	int length;
	sf::FloatRect r;

	geometry.clear();

	read_int(length, inp);
	for (int i = 0; i < length; i++) {
		read_rect(r, inp);
		geometry.push_back(r);
	}
	
	log_dbg("read: " << length);
}
