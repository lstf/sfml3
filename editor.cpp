#include "editor.h"

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
	if (!decorating && mouse_left) w.draw(mouse_box);
	if (decorating)
	{
		w.draw(deco_cursor);
	}
	if (console)
	{
		w.draw(command_text);
		w.draw(message_text);
	}
}

void Editor::setMaps(std::vector<Map*>* _maps)
{
	maps = _maps;
}

void Editor::setWindow(sf::RenderWindow* _w)
{
	w = _w;
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
	for (int i = 0; i < maps->size(); i++)
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
	for (int i = 1; i < maps->size(); i++)
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
	if (event.type == sf::Event::KeyPressed)
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
					geom = !geom;
					message_text.setString(std::string("-- Geom ") + (geom ? "TRUE" : "FALSE") + " --"); 
				}
				else if (command == "SNAP")
				{
					snap = !snap;
					message_text.setString(std::string("-- Snap ") + (snap ? "TRUE" : "FALSE") + " --"); 
				}
				else if (command == "DECO")
				{
					deco = !deco;
					message_text.setString(std::string("-- Deco ") + (deco ? "TRUE" : "FALSE") + " --"); 
				}
				else if (command.substr(0,5) == "NMAP ")
				{
					std::string map_name = command.substr(5,16);
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
				else if (command.substr(0,5) == "LIMG ")
				{
					std::string img_name = command.substr(5,16);

					if (!(*maps)[map_index]->loadTexture(img_name))
					{
						message_text.setString("-- Load Image Failed : " + img_name + " --");
					}
					else
					{
						message_text.setString("-- Loaded Image: " + img_name + " --");
					}
				}
				else if (command == "LSIM")
				{
					std::string img_list;
					for (int i = 0; i < (*maps)[map_index]->tx.size(); i++)
					{
						img_list += (*maps)[map_index]->tx[i]->name + "\n";
					}
					message_text.setString(img_list);
				}
				else if (command.substr(0,5) == "SIMG ")
				{
					std::string img_set = command.substr(5,16);
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
				else if (command == "GEDT")
				{
					decorating = false;
				}
//				else if (command.substr(0,5) == "DMAP ")
//				{
//					std::string map_name = command.substr(5,16);
//					int map_index_dup = findMap(map_name);
//					if (map_index_dup < 0)
//					{
//						message_text.setString("-- Duplicate Failed: " + map_name + " doesnt exist --");
//					}
//					else
//					{
//						maps->push_back(new Map(map_name));
//						map_index = maps->size()-1;
//						(*maps)[map_index]->name = map_name;
//
//						message_text.setString(std::string("-- New Map ") + map_name + " at map_index " + std::to_string(map_index) +  " --");
//					}
//				}
				else if (command == "SMAP")
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
				else if (command.substr(0,5) == "CMAP ")
				{
					std::string map_name = command.substr(5,16);
					int map_index_change = findMap(map_name);
					
					if (map_index_change < 0)
					{
						message_text.setString("-- Map Not Found --");
					}
					else
					{
						map_index = map_index_change;
					}
					
				}
				else if (command == "LSMP")
				{
					std::string map_names;

					for (int i = 0; i < maps->size(); i++)
					{
						map_names += (*maps)[i]->name + '\n';
					}

					message_text.setString(map_names);

				}
				else if (command == "CLRM")
				{
					message_text.setString("");
				}
				else if (command == "HELP")
				{
					std::string help_text;
					help_text += "QUIT: Exit editor\n";
					help_text += "CLRM: Clear message\n";
					help_text += "GEOM: Toggle map geometry\n";
					help_text += "DECO: Toggle map decorations (graphics)\n";
					help_text += "SNAP: Cycle snap modes\n";
					help_text += "LSMP: List maps\n";
					help_text += "NMAP <file>: Create new map\n";
					help_text += "LMAP <file>: Load map\n";
					help_text += "SMAP <file>: Save map\n";

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
			 event.mouseButton.button == sf::Mouse::Left)
	{
		mouse_left = true;
		if (decorating)
		{
			int x = event.mouseButton.x;
			int y = event.mouseButton.y;
			if (snap)
			{
				x -= x % 48;
				y -= y % 48;
			}
			sf::Vector2f position((float)x, (float)y);
			(*maps)[map_index]->addDeco(deco_name, position, BG);
		}
		else
		{
			mouse_pos.x = event.mouseButton.x;
			mouse_pos.y = event.mouseButton.y;
			mouse_box.setPosition((float)mouse_pos.x, (float)mouse_pos.y);
			mouse_box.setSize(sf::Vector2f(0.0f, 0.0f));
		}
	}

	//
	// Left Mouse Up
	//
	else if (event.type == sf::Event::MouseButtonReleased &&
			 event.mouseButton.button == sf::Mouse::Left)
	{
		mouse_left = false;
		if (!decorating)
		{
			sf::Vector2i r_mouse_pos(event.mouseButton.x, event.mouseButton.y);

			orient(mouse_pos, r_mouse_pos);

			(*maps)[map_index]->addWall(mouse_pos, r_mouse_pos - mouse_pos);
		}
	}

	//
	// Mouse Moved
	//
	else if (event.type == sf::Event::MouseMoved)
	{
		if (!decorating && mouse_left)
		{
			sf::Vector2i p_mouse_pos = mouse_pos;
			sf::Vector2i m_mouse_pos(event.mouseMove.x, event.mouseMove.y);
			sf::Vector2f size;
			sf::Vector2f pos;

			orient(p_mouse_pos, m_mouse_pos);
			pos.x = (float)(p_mouse_pos.x);
			pos.y = (float)(p_mouse_pos.y);
			size.x = (float)(m_mouse_pos.x - p_mouse_pos.x);
			size.y = (float)(m_mouse_pos.y - p_mouse_pos.y);

			mouse_box.setPosition(pos);
			mouse_box.setSize(size);
		}
		if (decorating)
		{
			deco_cursor.setPosition(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
		}
	}
}
