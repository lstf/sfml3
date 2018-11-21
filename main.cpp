#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "game.h"
#include "window.h"
#include "map.h"
#include "editor.h"
#include "player.h"

#include "dbox.h"
#include "entity.h"
#include "enemy.h"

int main()
{
	Game_State gs;
	DBox* dbox = NULL;

	Null_Entity ent;
	ent.setMainDBox(&dbox);
	
	Null_Enemy enm;

	sf::Font thintel;
	thintel.loadFromFile("./ats/fonts/thintel.ttf");

	w::init(thintel);
	//w::window.setFramerateLimit(240);
	w::window.setVerticalSyncEnabled(true);;
	w::window.setKeyRepeatEnabled(false);
	w::show_fps = true;

	gs.unpause();
	
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
    while (w::window.isOpen())
    {
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
				if (event.key.code == sf::Keyboard::P && 
					!editor.consoleActive())
				{
					if (gs.isPaused())
					{
						gs.unpause();
					}
					else
					{
						gs.pause();
					}
				}
				else if (event.key.code == sf::Keyboard::M &&
						 !editor.consoleActive())
				{
					if (dbox == NULL)
					{
						dbox = new DBox(&dbox, "ats/text/test");
					}
				}
			}


			//Send event to listening classes
			editor.handleInput(event);
			player.handleInput(event);
			maps[editor.map_index]->handleInput(event);
			if (dbox != NULL)
			{
				dbox->update(event);
			}
		}	

		if (player.interacted()) {
			for (auto it = ent.list.begin(); it != ent.list.end(); ++it) {
				if ((*it)->bounds().intersects(player.bounds())) {
					(*it)->interact();
				}
			}
		}

		if (player.weaponActive()) {
			for (auto it = enm.list.begin(); it != enm.list.end(); ++it) {
				if ((*it)->bounds().intersects(player.weaponBounds())) {
					(*it)->takeDamage();
				}
			}
		}
		
		//updates
		player.update(maps[editor.map_index]->getGeom(), w::frame_time);
		
		for (auto it = enm.list.begin(); it != enm.list.end(); ++it) {
			(*it)->update(maps[editor.map_index]->getGeom(), w::frame_time);
		}

		maps[editor.map_index]->update();
		if (maps[editor.map_index]->nextMap != 0)
		{
			int map_i = maps[editor.map_index]->nextMap;
			maps[editor.map_index]->nextMap = 0;
			player.sp.setPosition(maps[editor.map_index]->nextMap_pos);
			player.refresh();
			gs.unpause();
			editor.map_index = map_i;
			std::cout << "X: " << player.sp.getPosition().x << "Y: " << player.sp.getPosition().y << std::endl;
			std::cout << editor.map_index << std::endl;
		}

		//Drawing
        w::window.clear();

		w::window.setView(editor.getView());

		w::window.draw(*(maps[editor.map_index]));

		for (auto it = ent.list.begin(); it != ent.list.end(); ++it) {
			w::window.draw(*(*it));
		}

		for (auto it = enm.list.begin(); it != enm.list.end(); ++it) {
			w::window.draw(*(*it));
		}

		w::window.draw(player);

		w::window.draw(editor);
		if (dbox != NULL)
		{
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
