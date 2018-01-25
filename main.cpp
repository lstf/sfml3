#include <vector>
#include <string>


#include "font.h"
#include "window.h"
#include "map.h"


struct Editor_state 
{
	std::string command;
	sf::Text	draw_text;
	bool 		console;
	bool 		snap;
	bool 		geom;
	bool		deco;
	int			map_index;

	sf::Vector2i	mouse_pos;
	sf::RectangleShape mouse_box;	

	bool			mouse_left;
};

//swaps components of u and v such that
//	u is the top right corner in the box formed by them
void orient(sf::Vector2i &u, sf::Vector2i &v)
{
	sf::Vector2i temp;
	if (v.x < u.x)
	{
		temp.x = v.x;
		v.x = u.x;
		u.x = temp.x;
	}
	if (v.y < u.y)
	{
		temp.y = v.y;
		v.y = u.y;
		u.y = temp.y;
	}
}

int main()
{
	font::init();
	w::init();
	w::window.setFramerateLimit(60);
	w::show_fps = false;
	
	//bm:
	sf::Clock frame_clock; //for frame rate calculation
	
	std::vector<Map*> maps;
	maps.push_back(new Map);
	maps[0]->setName("Debug");


	//DEBUG db:


	Editor_state	e_state;
					e_state.map_index					= 0;
					e_state.console 					= false;
					e_state.snap 						= false;
					e_state.geom	 					= false;
					e_state.deco						= true;
					e_state.draw_text.setFont			(font::thintel);
					e_state.draw_text.setCharacterSize	(48);
					e_state.draw_text.setFillColor		(sf::Color::White);
					e_state.draw_text.setPosition		(5,427);
					e_state.draw_text.setString		("$ ");
					
					e_state.mouse_box.setFillColor			(sf::Color(0,0,0,0));
					e_state.mouse_box.setOutlineColor		(sf::Color::Green);
					e_state.mouse_box.setOutlineThickness	(2.0f);
					e_state.mouse_left						= false;


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
            if (event.type == sf::Event::Closed)
                w::window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (e_state.console)
				{
					if (event.key.code == sf::Keyboard::Tilde)
					{
						e_state.console = false;
						e_state.command = "";
						event.key.code = sf::Keyboard::Equal;
					}

					//Typing - filters input and translates event key code to ascii
					if (event.key.code >= 0 && event.key.code <= 25)
					{
						char letter = event.key.code + 65;

						e_state.command += letter;
					}
					else if (event.key.code >= 26 && event.key.code <= 35)
					{
						char letter = event.key.code - 26 + 48;

						e_state.command += letter;
					}
					else if (event.key.code == sf::Keyboard::BackSpace)
					{
						if (e_state.command != "")
						{
							e_state.command.pop_back(); 
						}
					}
					else if (event.key.code == sf::Keyboard::Space)
					{
						e_state.command += ' ';
					}

					e_state.draw_text.setString("$ " + e_state.command);

					//Handle commands
					if (event.key.code == sf::Keyboard::Return)
					{
						if (e_state.command == "QUIT")
						{
							w::window.close();
						}
						else if (e_state.command == "GEOM")
						{
							e_state.geom = !e_state.geom;
							e_state.command = "";
							e_state.draw_text.setString(std::string("$ -- Geom ") + (e_state.geom ? "TRUE" : "FALSE") + " --"); 
						}
						else if (e_state.command == "SNAP")
						{
							e_state.snap = !e_state.snap;
							e_state.command = "";
							e_state.draw_text.setString(std::string("$ -- Snap ") + (e_state.snap ? "TRUE" : "FALSE") + " --"); 
						}
						else if (e_state.command == "DECO")
						{
							e_state.deco = !e_state.deco;
							e_state.command = "";
							e_state.draw_text.setString(std::string("$ -- Deco ") + (e_state.deco ? "TRUE" : "FALSE") + " --"); 
						}
						else
						{
							e_state.draw_text.setString("$ -- Unkown Command: " + e_state.command + " --");
							e_state.command = "";
						}
					}
				}
				else
				{
					if (event.key.code == sf::Keyboard::Tilde)
					{
						e_state.console = true;
					}

				}
			}
			else if (event.type == sf::Event::MouseButtonPressed &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				e_state.mouse_left = true;
				e_state.mouse_pos.x = event.mouseButton.x;
				e_state.mouse_pos.y = event.mouseButton.y;
			}
			else if (event.type == sf::Event::MouseButtonReleased &&
					 event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i r_mouse_pos(event.mouseButton.x, event.mouseButton.y);

				e_state.mouse_left = false;

				orient(e_state.mouse_pos, r_mouse_pos);
	
				maps[e_state.map_index]->addWall(e_state.mouse_pos, r_mouse_pos - e_state.mouse_pos);
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				if (e_state.mouse_left)
				{
					sf::Vector2i p_mouse_pos = e_state.mouse_pos;
					sf::Vector2i m_mouse_pos(event.mouseMove.x, event.mouseMove.y);
					sf::Vector2f size;
					sf::Vector2f pos;

					orient(p_mouse_pos, m_mouse_pos);
					pos.x = (float)(p_mouse_pos.x);
					pos.y = (float)(p_mouse_pos.y);
					size.x = (float)(m_mouse_pos.x - p_mouse_pos.x);
					size.y = (float)(m_mouse_pos.y - p_mouse_pos.y);

					e_state.mouse_box.setPosition(pos);
					e_state.mouse_box.setSize(size);
				}
			}
        }

        w::window.clear();
		if (w::show_fps) w::window.draw(w::frame_rate);
		if (e_state.geom)
		{
			//for (int i = maps[e_state.map_index]->

		}
		if (e_state.mouse_left) w::window.draw(e_state.mouse_box);
		if (e_state.console) w::window.draw(e_state.draw_text);
        w::window.display();
    }

	//r:
    return 0;
}
