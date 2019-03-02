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
	return true;
}

void Game::frame_calc() {
	long long frame_us = frame_clock.getElapsedTime().asMicroseconds();
	frame_time = frame_us / 1000000.0;
	if (show_fps) {
		frame_rate_text.setString(to_string(1/frame_time));
	}
	frame_clock.restart();
}

void Game::handle_input(sf::Event &event) {
	if (state == GAMEPLAY) {
		player.handleInput(event);
	} else if (state == DIALOG && dbox != NULL) {
		dbox->update(event);
		if (dbox->finished) {
			delete dbox;
			dbox = NULL;
		}
	}
}

GameTrans* Game::update() {
	if (state == GAMEPLAY) {
		//entity interactions
		if (player.interacted()) {
			for (auto it = ent.list.begin(); it != ent.list.end(); ++it) {
				if ((*it)->bounds().intersects(player.bounds())) {
					dbox = (*it)->interact(player, level_state, global_state);
					if (dbox) {
						state = DIALOG;
					}
					break;
				}
			}
			if (state == GAMEPLAY) {
				for (auto it = por.list.begin(); it != por.list.end(); ++it) {
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
				for (auto it = enm.list.begin(); it != enm.list.end(); ++it) {
					if ((*it)->bounds().intersects(player.weaponBounds())) {
						(*it)->takeDamage();
					}
				}
			}
	
			//removal
			for (auto it = enm.list.begin(); it != enm.list.end(); ++it) {
				if ((*it)->remove()) {
					delete *it;
					break;
				}
			}
	
			//updates
			player.update(map_current->getGeom(), frame_time);
			for (auto it = enm.list.begin(); it != enm.list.end(); ++it) {
				(*it)->update(map_current->getGeom(), frame_time);
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
	ent.list.erase(ent.list.begin()+1, ent.list.end()); 
	enm.list.erase(enm.list.begin()+1, enm.list.end()); 
	por.list.erase(por.list.begin()+1, por.list.end()); 
}

bool Game::load_map(string name) {
	cout << "[GAME] loading map " << name << endl;
	if (name == "null map") {
		delete map_current;
		map_current = new Map("null map");
		return true;
	} else if (name == "") {
		return true;
	}
	Map* next_map = new Map(name);
	if (!next_map->loadMap()) {
		cout << "[GAME] failed to load map " << name;
		delete next_map;
		return false;
	}
	delete map_current;
	map_current = next_map;
	return true;
}
void Game::new_map(string name) {
	clear();
	delete map_current;
	map_current = new Map(name);
	map_current->saveMap();
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

void Game::resetState() {
	state = GAMEPLAY;
}

void Game::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(*map_current);


	for (auto it = ent.list.begin(); it != ent.list.end(); ++it) {
		w.draw(*(*it), states);
	}

	for (auto it = enm.list.begin(); it != enm.list.end(); ++it) {
		w.draw(*(*it), states);
	}

	w.draw(player, states);

	if (state == DIALOG && dbox != NULL) {
		w.draw(*dbox, states);
	}
	
	sf::View temp = w.getView();

	w.setView(w.getDefaultView());

	if (show_fps) {
		w.draw(frame_rate_text, states);
	}

	w.setView(temp);
}
