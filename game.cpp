#include "game.h"

Game::Game() {
	log_dbg("constructing game");

	//Map setup
	log_dbg("loading map list");
	ifstream maps_file("./ats/mps/maps.list", std::ifstream::in);
	if (!maps_file.is_open()) {
		ok = false;
		log_err("failed to open map list");
		return;
	}
	ok = true;

	string map_name;
	int map_count;
	maps_file >> map_count;
	
	for (int i = 0; i < map_count; i++) {
		maps_file >> map_name;
		map_names.push_back(map_name);
	}

	maps_file.close();

	//Frame rate display setup
	show_fps = true;
	frame_clock.restart();
	frame_rate_text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
	frame_rate_text.setString("0");
	frame_rate_text.setCharacterSize(20);
	frame_rate_text.setFillColor(sf::Color::Yellow);

	//In-game state setup
	map<string, int> null_state;
	World::lstates["null map"] = null_state;
	World::lstate = &World::lstates["null map"];
	World::gstate = new map<string, int>;

	//Game setup
	state = GAMEPLAY;
	map_current = new Map("null map");
}

void Game::frame_calc() {
	long long frame_us = frame_clock.getElapsedTime().asMicroseconds();
	World::frame_time = frame_us / 1000000.0;
	if (show_fps) {
		frame_rate_text.setString(to_string(1/World::frame_time));
	}
	frame_clock.restart();
}

void Game::handle_input(sf::Event &event) {
	if (state == GAMEPLAY) {
		player.handleInput(event);
	} else if (state == DIALOG && dbox != NULL) {
		dbox->update(event);
		if (dbox->finished) {
			if (dbox->destroy_ent) {
				delete_entity((Entity*)dbox->destroy_ent_ptr);
			}
			if (dbox->save_game) {
				//write();
			}
			delete dbox;
			dbox = NULL;
		}
	}
}

GameTrans* Game::update() {
	if (state == GAMEPLAY) {
		//entity updates 
		for (int i = Entity::list.size() - 1; i >= 0; i--) {
			if(Entity::list[i]->update(player)) {
				delete_entity(Entity::list[i]);
			}
		}

		//entity interactions
		if (player.interacted()) {
			for (auto it = Entity::list.begin();
			it != Entity::list.end(); ++it) {
				if ((*it)->bounds().intersects(player.bounds())) {
					dbox = (*it)->interact(player);
					if (dbox) {
						state = DIALOG;
					}
					break;
				}
			}
			if (state == GAMEPLAY) {
				for (auto it = Portal::list.begin();
				it != Portal::list.end(); ++it) {
					if ((*it)->bounds().intersects(player.bounds())) {
						map_trans = (*it)->interact();
						state = PORTAL;
						break;
					}
				}
			}
		}
	
		if (state == GAMEPLAY) {
			//player weapon
			if (player.weaponActive()) {
				for (auto it = Enemy::list.begin();
				it != Enemy::list.end(); ++it) {
					if ((*it)->bounds().intersects(player.weaponBounds())) {
						(*it)->takeDamage();
					}
				}
			}
	
			//removal
			for (auto it = Enemy::list.begin(); it != Enemy::list.end(); ++it) {
				if ((*it)->remove()) {
					delete *it;
					break;
				}
			}
	
			//updates
			player.update(Map::geom, World::frame_time);
			for (auto it = Enemy::list.begin(); it != Enemy::list.end(); ++it) {
				(*it)->update(Map::geom, World::frame_time);
			}
		}	
	}

	if (state == PORTAL) {
		if (map_trans->p->update()) {
			trans = new GameTrans;
			trans->name = map_trans->map_name;
			trans->position = map_trans->position;
			delete map_trans;
			map_trans = NULL;
			return trans;
		}
	}
	
	if (state == DIALOG && dbox == NULL) {
		state = GAMEPLAY;
		player.reset_input();
	}

	return NULL;
}

void Game::clear() {
	log_dbg("clearing entities");
	Entity::list.clear(); 
	log_dbg("clearing enemies");
	Enemy::list.clear(); 
	log_dbg("clearing portals");
	Portal::list.clear(); 
}

bool Game::load_map(string name, sf::Vector2f pos) {
	if (name == "") {
		log_dbg("transitioning to same map");
	} else {
		log_dbg("transitioning to map " << name);
	}
	player.setPosition(pos);
	player.reset_input();

	if (name == "") {
		return true;
	}

	clear();

	log_dbg("loading map " << name);
	if (name == "null map") {
		delete map_current;
		map_current = new Map("null map");
		World::lstate = &World::lstates["null map"];
		return true;
	}

	Map* next_map = new Map(name);
	if (!next_map->load_init_lstate()) {
		delete next_map;
		log_err("failed to load map " << name);
		return false;
	}

	if (World::lstates.find(name) != World::lstates.end()) {
		log_dbg("lstate exists");
		World::lstate = &World::lstates[name];
	} else {
		log_dbg("lstate does not exist, copying init lstate");
		World::lstates[name] = next_map->init_lstate;
		World::lstate = &World::lstates[name];
	}

	if (!next_map->load()) {
		delete next_map;
		log_err("failed to load map " << name);
		return false;
	}

	delete map_current;
	map_current = next_map;

	return true;
}

#ifdef EDITOR_BUILD
void Game::new_map(string name) {
	log_dbg("adding new map " << name);
	sf::Vector2f pos(0, 0);
	player.setPosition(pos);

	clear();

	delete map_current;
	map_current = new Map(name);
	map_names.push_back(name);
	map_current->save();
}

void Game::save_maplist() {
	log_dbg("saving map list");
	ofstream maps_file("./ats/mps/maps.list");
	if (!maps_file.is_open()) {
		log_err("cannot open maps list");
		return;
	}

	maps_file << map_names.size() << endl;
	for (auto it = map_names.begin(); it != map_names.end(); ++it) {
		maps_file << *it << endl;
	}
	maps_file.close();
}
#endif

void Game::write() {
	log_err("saving even though its not implemented");
	ofstream out("./ats/save");
	
	player.write(out);
	write_states(World::lstates, out);

	write_string(map_current->name, out);
}

void Game::read() {
	log_err("loading even though its not implemented");
	ifstream inp("./ats/save");
	if (!inp.is_open()) {
		log_err("no save game found");
		return;
	}
	player.read(inp);
	read_states(World::lstates, inp);

	string name;
	read_string(name, inp);
	load_map(name, player.sp.getPosition());
}

void Game::resetState() {
	log_dbg("resetting state to gameplay");
	state = GAMEPLAY;
}

void Game::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(*map_current);

	for (auto it = Portal::list.begin(); it != Portal::list.end(); ++it) {
		w.draw(*(*it), states);
	}

	for (auto it = Entity::list.begin(); it != Entity::list.end(); ++it) {
		w.draw(*(*it), states);
	}

	for (auto it = Enemy::list.begin(); it != Enemy::list.end(); ++it) {
		w.draw(*(*it), states);
	}

	w.draw(player, states);

	if (state == DIALOG && dbox != NULL) {
		w.draw(*dbox, states);
	}
	
	sf::View temp = w.getView();

	w.setView(Window::default_view);

	if (show_fps) {
		w.draw(frame_rate_text, states);
	}

	w.setView(temp);
}

Game::~Game() {
	log_dbg("destructing game");
}
