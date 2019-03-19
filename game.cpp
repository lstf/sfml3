#include "game.h"

bool Game::init() {
	//Map setup
	ifstream maps_file("./ats/mps/maps.list", std::ifstream::in);
	if (!maps_file.is_open()) {
		std::cout << std::string("(map load) Failed to open ") + "maps.list";
		return false;
	}

	map_current = new Map("null map");
	
	int map_count;
	string map_name;
	maps_file >> map_count;
	for (int i = 0; i < map_count; i++)
	{
		maps_file >> map_name;
		map_names.push_back(map_name);
	}
	maps_file.close();

	//Frame rate display setup
	frame_rate_text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
	frame_rate_text.setString("0");
	frame_rate_text.setCharacterSize(20);
	frame_rate_text.setFillColor(sf::Color::Yellow);

	state = GAMEPLAY;

	show_fps = true;
	frame_clock.restart();
	World::gstate = new map<string, int>;
	map<string, int> null_state;
	World::lstates["null map"] = null_state;
	World::lstate = &World::lstates["null map"];
	return true;
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
				delete_ent((Entity*)dbox->destroy_ent_ptr);
			}
			if (dbox->save_game) {
				write();
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
				delete_ent(Entity::list[i]);
			}
		}

		//entity interactions
		if (player.interacted()) {
			for (auto it = Entity::list.begin(); it != Entity::list.end(); ++it) {
				if ((*it)->bounds().intersects(player.bounds())) {
					dbox = (*it)->interact(player);
					if (dbox) {
						state = DIALOG;
					}
					break;
				}
			}
			if (state == GAMEPLAY) {
				for (auto it = Portal::list.begin(); it != Portal::list.end(); ++it) {
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
				for (auto it = Enemy::list.begin(); it != Enemy::list.end(); ++it) {
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
			player.update(map_current->get_geom(), World::frame_time);
			for (auto it = Enemy::list.begin(); it != Enemy::list.end(); ++it) {
				(*it)->update(map_current->get_geom(), World::frame_time);
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
			state = TRANSITION;
		}
	}
	
	if (state == DIALOG && dbox == NULL) {
		state = GAMEPLAY;
	}

	return NULL;
}

void Game::clear() {
	Entity::list.clear(); 
	Enemy::list.clear(); 
	Portal::list.clear(); 
}

bool Game::load_map(string name, sf::Vector2f pos) {
	cout << "[MAIN] clearing things" << endl;
	clear();
	cout << "[GAME] setting position " << pos.x << " " << pos.y << endl;
	player.setPosition(pos);
	cout << "[GAME] loading map " << name << endl;
	if (name == "null map") {
		delete map_current;
		map_current = new Map("null map");
		World::lstate = &World::lstates["null map"];
		return true;
	} else if (name == "") {
		return true;
	}
	Map* next_map = new Map(name);
	if (World::lstates.find(name) == World::lstates.end()) {
		World::lstates[name] = next_map->init_lstate;
	}
	World::lstate = &World::lstates[name];
	if (!next_map->load()) {
		cout << "[GAME] failed to load map " << name;
		delete next_map;
		return false;
	}
	if (World::lstates.find(name) == World::lstates.end()) {
	}
	delete map_current;
	map_current = next_map;
	return true;
}
void Game::new_map(string name) {
	sf::Vector2f pos(0, 0);
	cout << "[MAIN] clearing things" << endl;
	clear();
	cout << "[GAME] setting position " << pos.x << " " << pos.y << endl;
	player.setPosition(pos);
	clear();
	delete map_current;
	map_current = new Map(name);
	map_current->save();
	map_names.push_back(name);
}

void Game::save_maplist() {
	ofstream maps_file("./ats/mps/maps.list");
	if (!maps_file.is_open()) {
		cout << "[GAME] cannot open maps list" << endl;
	}
	maps_file << map_names.size() << endl;
	for (auto it = map_names.begin(); it != map_names.end(); ++it) {
		maps_file << *it << endl;
	}
	maps_file.close();
}

void Game::write() {
	ofstream out("./ats/save");
	
	player.write(out);
	write_states(World::lstates, out);

	write_string(map_current->name, out);
}

void Game::read() {
	ifstream inp("./ats/save");
	if (!inp.is_open()) {
		cout << "[GAME] no save game found" << endl;
		return;
	}
	player.read(inp);
	read_states(World::lstates, inp);

	string name;
	read_string(name, inp);
	load_map(name, player.sp.getPosition());
}

void Game::resetState() {
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
