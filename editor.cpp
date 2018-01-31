#include "editor.h"

Editor::Editor(sf::RenderWindow* _w, std::vector<Map*>* _maps, sf::Font &_font)
{
	setWindow						(_w);
	setMaps							(_maps);
	map_index						= 0;
	console 							= false;
	decorating						= false;
	snap 							= false;
	geom	 							= false;
	deco								= true;
	command_text.setFont				(_font);
	command_text.setCharacterSize	(48);
	command_text.setFillColor		(sf::Color::White);
	command_text.setPosition			(5,427);
	command_text.setString			("$ ");
	message_text.setFont				(_font);
	message_text.setCharacterSize	(48);
	message_text.setFillColor		(sf::Color::White);
	message_text.setPosition			(5,0);

	mouse_box.setFillColor			(sf::Color(0,0,0,0));
	mouse_box.setOutlineColor		(sf::Color::Green);
	mouse_box.setOutlineThickness	(2.0f);
	mouse_left						= false;

	view.setSize					(960,480);
	view.setCenter					(480,240);
	mouse_middle 					= false;

	imgView							= false;
}

void Editor::orient(sf::Vector2i &u, sf::Vector2i &v)
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

void Editor::draw(sf::RenderTarget& w, sf::RenderStates states) const
{
	sf::View temp = w.getView();

	//Scrolling
	if (!decorating && mouse_left)
	{
		w.draw(mouse_box);
	}
	if (decorating)
	{
		w.draw(deco_cursor, states);
	}

	//Static
	w.setView(w.getDefaultView());
	if (console)
	{
		w.draw(command_text, states);
		w.draw(message_text, states);
	}
	if (imgView)
	{
		for (int i = (int)(*maps)[map_index]->tx.size()-1; i >= 0; i--)
		{
			sf::Sprite sp((*maps)[map_index]->tx[i]->texture);
			sp.setPosition((i%3)*48+816,(i/3));
			w.draw(sp, states);
		}
	}
	w.setView(temp);
}

void Editor::setMaps(std::vector<Map*>* _maps)
{
	maps = _maps;
}

void Editor::setWindow(sf::RenderWindow* _w)
{
	w = _w;
}

sf::Vector2i Editor::getMouseCoordinates()
{
	sf::Vector2f world_pos;
	sf::Vector2i local_pos;	

	local_pos = sf::Mouse::getPosition(*w);
	world_pos = w->mapPixelToCoords(local_pos);
	if (snap)
	{
		world_pos.x -= (int)world_pos.x % 48;
		world_pos.y -= (int)world_pos.y % 48;
	}
	return sf::Vector2i((int)world_pos.x, (int)world_pos.y);
}

bool Editor::getConsoleInput(const sf::Event &event, std::string &str)
{
	if (event.key.code >= 0 && event.key.code <= 25)
	{
		str += char(event.key.code + 65);
	}
	else if (event.key.code >= 26 && event.key.code <= 35)
	{
		str += char(event.key.code - 26 + 48);
	}
	else if (event.key.code == sf::Keyboard::BackSpace)
	{
		if (str != "")
		{
			str.pop_back(); 
		}
	}
	else if (event.key.code == sf::Keyboard::Space)
	{
		str += ' ';
	}
	else if (event.key.code == sf::Keyboard::Period)
	{
		str += '.';
	}
	else
	{
		return false;
	}

	return true;
}

int Editor::findMap(std::string mapName)
{
	for (int i = 0; i < (int)maps->size(); i++)
	{
		if (mapName == (*maps)[i]->name)
		{
			return i;
		}
	}
	return -1;
}

bool Editor::checkUnsaved(std::string &unsaved)
{
	unsaved = "-- Unsaved Maps --\n";
	for (int i = 1; i < (int)maps->size(); i++)
	{
		if ((*maps)[i]->modified)
		{
			unsaved += ((*maps)[i]->name) + "\n";
			return true;
		}
	}
	unsaved = "";
	return false;
}

void Editor::handleInput(sf::Event event)
{
	//
	//	Key Pressed
	//
	if (event.type == sf::Event::KeyPressed &&
		!mouse_left && !mouse_middle)
	{
		if (console)
		{
			if (event.key.code == sf::Keyboard::Tilde)
			{
				console = false;
				command = "";
				event.key.code = sf::Keyboard::Equal;
			}

			//Get console input
			if (getConsoleInput(event, command))
			{
				command_text.setString("$ " + command);
			}

			//Handle commands
			if (event.key.code == sf::Keyboard::Return)
			{
				if (command == "QUIT")
				{
					std::string unsaved_maps;

					if (checkUnsaved(unsaved_maps))
					{
						message_text.setString(unsaved_maps);
					}
					else
					{
						w->close();
					}
				}
				else if(command == "QUIT OVERIDE")
				{
					w->close();
				}
				else if (command == "GEOM")
				{
					geom = (*maps)[map_index]->toggleGeom();
					message_text.setString(std::string("-- Geom ") + (geom ? "TRUE" : "FALSE") + " --"); 
				}
				else if (command == "SNAP")
				{
					snap = !snap;
					message_text.setString(std::string("-- Snap ") + (snap ? "TRUE" : "FALSE") + " --"); 
				}
				else if (command == "DECO")
				{
					deco = (*maps)[map_index]->toggleDeco();
					message_text.setString(std::string("-- Deco ") + (deco ? "TRUE" : "FALSE") + " --"); 
				}
				else if (command.substr(0,8) == "NEW MAP ")
				{
					std::string map_name = command.substr(8,16);
					int map_index_new = findMap(map_name);


					if (map_index_new < 0)
					{
						maps->push_back(new Map(map_name));
						map_index = maps->size()-1;
						(*maps)[map_index]->name = map_name;

						message_text.setString(std::string("-- New Map ") + map_name + " at map_index " + std::to_string(map_index) +  " --");
					}
					else
					{
						message_text.setString("-- Map Already Exists --");
					}
				}
				else if (command.substr(0,9) == "LOAD IMG ")
				{
					std::string img_name = command.substr(9,16);

					if (!(*maps)[map_index]->loadTexture(img_name))
					{
						message_text.setString("-- Load Image Failed : " + img_name + " --");
					}
					else
					{
						message_text.setString("-- Loaded Image: " + img_name + " --");
					}
				}
				else if (command == "LS IMG")
				{
					std::string img_list;
					imgView = true;
					for (int i = 0; i < (int)(*maps)[map_index]->tx.size(); i++)
					{
						img_list += (*maps)[map_index]->tx[i]->name + "\n";
					}
					message_text.setString(img_list);
				}
				else if (command.substr(0,8) == "SET IMG ")
				{
					std::string img_set = command.substr(8,16);
					sf::Texture* tx = (*maps)[map_index]->getTexture(img_set);

					if (tx)
					{
						decorating = true;
						deco_cursor.setTexture(*tx);
						deco_name = img_set;
					}
					else 
					{
						message_text.setString("-- Set Image Failed --");
					}
				}
				else if (command == "EDIT GEOM")
				{
					decorating = false;
				}
				else if (command == "SAVE MAP")
				{
					if (!((*maps)[map_index]->saveMap()))
					{
						message_text.setString(std::string("-- Save Map Failed --"));
					}
					else
					{
						message_text.setString(std::string("-- Saved Map ") + (*maps)[map_index]->name+ " --");
					}
				}
				else if (command.substr(0,8) == "SET MAP ")
				{
					std::string map_name = command.substr(8,16);
					int map_index_change = findMap(map_name);
					
					if (map_index_change < 0)
					{
						message_text.setString("-- Map Not Found --");
					}
					else
					{
						map_index = map_index_change;
						message_text.setString("-- " + map_name + " --");
					}
					
				}
				else if (command == "LS MAP")
				{
					std::string map_names;

					for (int i = 0; i < (int)maps->size(); i++)
					{
						map_names += (*maps)[i]->name + '\n';
					}

					message_text.setString(map_names);

				}
				else if (command == "CLEAR")
				{
					message_text.setString("");
				}
				else if (command == "HELP")
				{
					std::string help_text;
					help_text += "QUIT: 			Exit editor\n";
					help_text += "CLEAR:		 	Clear message\n";
					help_text += "GEOM: 			Toggle map geometry\n";
					help_text += "DECO: 			Toggle map decorations (graphics)\n";
					help_text += "SNAP: 			Cycle snap modes\n";
					help_text += "LS MAP: 			List maps\n";
					help_text += "SET MAP <map>: 	Set working map to <map>\n";
					help_text += "NEW MAP <file>: 	Create new map\n";
					help_text += "SAVE MAP <file>: 	Save map\n";
					help_text += "LS IMG: 			List imgs in map\n";
					help_text += "LOAD IMG: 		Load img into map\n";
					help_text += "SET IMG: 			Set working img\n";

					message_text.setString(help_text);
				}
				else
				{
					message_text.setString("-- Unkown Command: " + command + " --");
				}
				command_text.setString("$ ");
				command = "";
			}
		}
		else
		{
			if (event.key.code == sf::Keyboard::Tilde)
			{
				console = true;
			}
		}
	}

	//
	// Left Mouse Down
	//
	else if (event.type == sf::Event::MouseButtonPressed &&
			 event.mouseButton.button == sf::Mouse::Left &&
			 !mouse_middle)
	{
		mouse_left = true;
		mouse_pos = getMouseCoordinates();

		sf::Vector2f position((float)mouse_pos.x, (float)mouse_pos.y);
		if (mode == EDIT || mode == GEOM)
		{
			mouse_box.setPosition(position);
			mouse_box.setSize(sf::Vector2f(0.0f, 0.0f));
		}
		else if (mode == DECO)
		{
			(*maps)[map_index]->addDeco(deco_name, position, BG);
		}
	}
	//
	// Left Mouse Up
	//
	else if (event.type == sf::Event::MouseButtonReleased &&
			 event.mouseButton.button == sf::Mouse::Left &&
			 !mouse_middle)
	{
		mouse_left = false;
		sf::Vector2i prev_mouse_pos = mouse_pos;

		mouse_pos = getMouseCoordinates();

		if (mode == GEOM)
		{
			orient(mouse_pos, prev_mouse_pos);

			(*maps)[map_index]->addWall(mouse_pos, prev_mouse_pos - mouse_pos);
		}
		else if (mode == EDIT)
		{
			orient(mouse_pos, prev_mouse_pos);
			sf::IntRect r(mouse_pos, prev_mouse_pos - mouse_pos);

			if ((*maps)[map_index]->geom)
			{
				std::vector<IntRect>* geo = (*maps)[map_index]->geometry;
				for (auto i = geo->begin(); i != geo->end() ; ++i)
				{
					if (r.intersects((*maps)[map_index]->geometry[i]))
					{

					}
				}
			}
			if ((*maps)[map_index]->deco)
			{
			}
		}
	}
	//
	// Middle Mouse down
	//
	else if (event.type == sf::Event::MouseButtonPressed &&
			 event.mouseButton.button == sf::Mouse::Right &&
			 !mouse_left)
	{
		mouse_middle = true;
		mouse_pos.x = event.mouseButton.x;
		mouse_pos.y = event.mouseButton.y;
	}
	//
	// Middle Mouse Up
	//
	else if (event.type == sf::Event::MouseButtonReleased &&
			 event.mouseButton.button == sf::Mouse::Right &&
			 !mouse_left)
	{
		mouse_middle = false;
	}
	
	//
	// Mouse Moved
	//
	else if (event.type == sf::Event::MouseMoved)
	{
		sf::Vector2i move_mouse_pos = getMouseCoordinates();
		sf::Vector2i prev_mouse_pos = mouse_pos;

		if (!decorating && mouse_left)
		{
			sf::Vector2f size;
			sf::Vector2f pos;

			orient(prev_mouse_pos, move_mouse_pos);
			pos.x = (float)(prev_mouse_pos.x);
			pos.y = (float)(prev_mouse_pos.y);
			size.x = (float)(move_mouse_pos.x - prev_mouse_pos.x);
			size.y = (float)(move_mouse_pos.y - prev_mouse_pos.y);

			mouse_box.setPosition(pos);
			mouse_box.setSize(size);
		}
		if (decorating)
		{
			deco_cursor.setPosition(sf::Vector2f((float)move_mouse_pos.x, (float)move_mouse_pos.y));
		}
		if (mouse_middle)
		{
			view.move(sf::Vector2f((float)(mouse_pos.x - event.mouseMove.x), (float)(mouse_pos.y - event.mouseMove.y)));
			mouse_pos.x = event.mouseMove.x;
			mouse_pos.y = event.mouseMove.y;
		}
	}
}
