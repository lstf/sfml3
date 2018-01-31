#include <vector>
#include <string>
#include <iostream>

#include "window.h"
#include "map.h"
#include "editor.h"


int main()
{
	sf::Font thintel;
	thintel.loadFromFile("./ats/fonts/thintel.ttf");

	w::init(thintel);
	w::window.setFramerateLimit(60);
	w::show_fps = false;
	
	//bm:
	sf::Clock frame_clock; //for frame rate calculation
	
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

	//DEBUG db:


	//Editor setup
	Editor			editor(&w::window, &maps, thintel);
	w::window.setView(editor.view);

	/**************************************************************************/
	//
	//	GameLoop: gl:
	//
	//
    while (w::window.isOpen())
    {
		w::frame_calc(frame_clock.getElapsedTime().asSeconds());
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

		}	

        w::window.clear();

		w::window.setView(editor.view);

		w::window.draw(*(maps[editor.map_index]));
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
