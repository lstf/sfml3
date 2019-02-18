#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

#include "window.h"

#include "actors/player.h"
#include "map/map.h"
#include "ui/dbox.h"
#include "ui/editor.h"

enum State {
	GAMEPLAY,
	EDITOR,
	MENU,
	DIALOG,
	PORTAL,
	TRANSITION
};

int main()
{
	DBox* dbox = NULL;
	MapTrans* map_trans = NULL;

	Null_Entity ent;
	Null_Enemy enm;
	Null_Portal por;

	sf::Font thintel;
	thintel.loadFromFile("./ats/fonts/thintel.ttf");

	w::init(thintel);
	//w::window.setFramerateLimit(240);
	w::window.setVerticalSyncEnabled(true);
	w::window.setKeyRepeatEnabled(false);
	w::show_fps = true;
	
	//bm:
	sf::Clock frame_clock; //for frame rate calculation
	long long frameus;
	
	//Map setup
	std::vector<Map*> maps;
	std::string map_name;
	int 		map_count;

	Player player;

	maps.push_back(new Map("null map"));

	std::ifstream 	maps_file(std::string(MAP_DIR) + "maps.list", std::ifstream::in);
	if (!maps_file.is_open())
	{
		std::cout << std::string("(map load) Failed to open ") + MAP_DIR + "maps.list";
		return -1;
	}

	maps_file >> map_count;
	for (int i = 0; i < map_count; i++)
	{
		maps_file >> map_name;
		
		maps.push_back(new Map(map_name));
		if (!(maps[maps.size()-1]->loadMap()))
		{
			std::cout << std::string("(map load) Failed to open ") + MAP_DIR + map_name;
		}
		maps[maps.size()-1]->setPlayer(&player);
	}

	maps_file.close();



	//Editor setup
	Editor			editor(&w::window, &player, &maps, thintel);
	w::window.setView(editor.view);

	/**************************************************************************/
	//
	//	GameLoop: gl:
	//
	//
	State state = EDITOR;
	map<string, int> level_state;
	map<string, int> global_state;
    while (w::window.isOpen())
    {
		if (state == DIALOG && dbox == NULL) {
			state = GAMEPLAY;
		}
		frameus = frame_clock.getElapsedTime().asMicroseconds();
		w::frame_calc(frameus);
		frame_clock.restart();

        sf::Event event;
		
        while (w::window.pollEvent(event))
        {
			//
			//	Closed
			//
            if (event.type == sf::Event::Closed)
			{
                w::window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Tab) {
					if (state == GAMEPLAY) {
						state = EDITOR;
					} else if (state == EDITOR) {
						state = GAMEPLAY;
					}
				}
				else if (event.key.code == sf::Keyboard::M &&
						 !editor.consoleActive())
				{
					if (state == GAMEPLAY)
					{
						DTree* dtree = new DTree;
						DNode* root = newDnode(dtree, "Testing!");
						root->options[0]->text = "1...";
						DNode* opt1 = newDnode(dtree, "you picked 1");
						root->options[0]->target = opt1;
						newDoption(dtree, root, "2...");
						DNode* opt2 = newDnode(dtree, "you picked 2");
						root->options[1]->target = opt2;
						newDoption(dtree, root, "3...");
						DNode* opt3 = newDnode(dtree, "you picked 3");
						root->options[2]->target = opt3;
						newDoption(dtree, root, "repeat");
						root->options[3]->target = root;
						dbox = new DBox(dtree, &player, &level_state, &global_state);
						state = DIALOG;
					}
				}
			}

			//Send event to listening classes
			if (state == EDITOR) {
				editor.handleInput(event);
			} else if (state == GAMEPLAY) {
				player.handleInput(event);
			} else if (state == DIALOG && dbox != NULL) {
				dbox->update(event);
				if (dbox->finished) {
					delete dbox;
					dbox = NULL;
				}
			}

		}	

		if (state == GAMEPLAY) {

			//entity interactions
			if (player.interacted()) {
				for (auto it = ent.list.begin(); it != ent.list.end(); ++it) {
					if ((*it)->bounds().intersects(player.bounds())) {
						dbox = (*it)->interact(player, level_state, global_state);
						state = DIALOG;
						break;
					}
				}
				for (auto it = por.list.begin(); it != por.list.end(); ++it) {
					if ((*it)->bounds().intersects(player.bounds())) {
						map_trans = (*it)->interact();
						state = PORTAL;
						break;
					}
				}
			}
	
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
			player.update(maps[editor.map_index]->getGeom(), w::frame_time);
			for (auto it = enm.list.begin(); it != enm.list.end(); ++it) {
				(*it)->update(maps[editor.map_index]->getGeom(), w::frame_time);
			}
	
		}

		if (state == PORTAL) {
			if (map_trans->p->update()) {
				delete map_trans;
				map_trans = NULL;
				state = GAMEPLAY;
			}
		}

		if (maps[editor.map_index]->nextMap != 0)
		{
			int map_i = maps[editor.map_index]->nextMap;
			maps[editor.map_index]->nextMap = 0;
			player.sp.setPosition(maps[editor.map_index]->nextMap_pos);
			player.refresh();
			editor.map_index = map_i;
			std::cout << "X: " << player.sp.getPosition().x << "Y: " << player.sp.getPosition().y << std::endl;
			std::cout << editor.map_index << std::endl;
		}

		//Drawing
        w::window.clear();

		if (state == GAMEPLAY) {
			w::window.setView(player.view);
		} else if (state == EDITOR) {
			w::window.setView(editor.getView());
		}


		w::window.draw(*(maps[editor.map_index]));

		for (auto it = ent.list.begin(); it != ent.list.end(); ++it) {
			w::window.draw(*(*it));
		}

		for (auto it = enm.list.begin(); it != enm.list.end(); ++it) {
			w::window.draw(*(*it));
		}

		w::window.draw(player);

		if (state == EDITOR) {
			w::window.draw(editor);
		}

		if (state == DIALOG && dbox != NULL) {
			w::window.draw(*dbox);
		}
		
		if (w::show_fps) w::window.draw(w::frame_rate);
		w::window.display();
    }

	//Store maps
	std::ofstream maps_file_out(std::string(MAP_DIR) + "maps.list", std::ofstream::out);
	if (!maps_file_out.is_open())
	{
		std::cout << std::string("(save) Failed to open ") + MAP_DIR + "maps.list\n";
		return false;
	}
	
	maps_file_out << maps.size()-1 << std::endl;
	for (unsigned int i = 1; i < maps.size(); i++)
	{
		maps_file_out << maps[i]->name << std::endl;
	}

	maps_file_out.close();

	//r:
    return 0;
}
