#include <vector>
#include <string>
#include <iostream>


#include "font.h"
#include "window.h"
#include "map.h"
#include "editor.h"


int main()
{
	font::init();
	w::init();
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
	Editor			editor;
					editor.setWindow						(&w::window);
					editor.setMaps							(&maps);
					editor.map_index						= 0;
					editor.console 							= false;
					editor.snap 							= false;
					editor.geom	 							= false;
					editor.deco								= true;
					editor.command_text.setFont				(font::thintel);
					editor.command_text.setCharacterSize	(48);
					editor.command_text.setFillColor		(sf::Color::White);
					editor.command_text.setPosition			(5,427);
					editor.command_text.setString			("$ ");
					editor.message_text.setFont				(font::thintel);
					editor.message_text.setCharacterSize	(48);
					editor.message_text.setFillColor		(sf::Color::White);
					editor.message_text.setPosition			(5,0);

					editor.mouse_box.setFillColor			(sf::Color(0,0,0,0));
					editor.mouse_box.setOutlineColor		(sf::Color::Green);
					editor.mouse_box.setOutlineThickness	(2.0f);
					editor.mouse_left						= false;


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
		if (w::show_fps) w::window.draw(w::frame_rate);
		if (editor.geom)
		{
			sf::RectangleShape 	rect;
								rect.setOutlineColor	(sf::Color::Red);
								rect.setOutlineThickness(1.0);
								rect.setFillColor		(sf::Color::Red);
			for (int i = maps[editor.map_index]->geometry.size() - 1; i >= 0; i--)
			{
				sf::Vector2f size((float)maps[editor.map_index]->geometry[i].width, (float)maps[editor.map_index]->geometry[i].height);
				sf::Vector2f pos((float)maps[editor.map_index]->geometry[i].left, (float)maps[editor.map_index]->geometry[i].top);
				
				rect.setPosition(pos);
				rect.setSize(size);

				w::window.draw(rect);
			}

		}
		w::window.draw(editor);
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
