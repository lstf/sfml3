#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "window.h"
#include "map.h"
#include "editor.h"
#include "player.h"

int main()
{
	sf::Font thintel;
	thintel.loadFromFile("./ats/fonts/thintel.ttf");

	w::init(thintel);
	w::window.setFramerateLimit(60);
	w::window.setKeyRepeatEnabled(false);
	w::show_fps = false;
	
	//bm:
	sf::Clock frame_clock; //for frame rate calculation
	float frameTime;
	
	//Map setup
	std::vector<Map*> maps;
	std::string map_name;
	int 		map_count;

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
	}

	maps_file.close();


	Player player;


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
		frameTime = frame_clock.getElapsedTime().asSeconds();
		w::frame_calc(frameTime);
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

			//Send event to listening classes
			editor.handleInput(event);
			player.handleInput(event);
		}	

		//updates
		player.update(maps[editor.map_index]->getGeom(), frameTime);
		maps[editor.map_index]->update(player.sp.getGlobalBounds());
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

		w::window.setView(editor.getView());

		w::window.draw(*(maps[editor.map_index]));
		w::window.draw(player);
		w::window.draw(editor);
		
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
	for (int i = 1; i < maps.size(); i++)
	{
		maps_file_out << maps[i]->name << std::endl;
	}

	maps_file_out.close();

	//r:
    return 0;
}
