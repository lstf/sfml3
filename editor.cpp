#include "editor.h"

void Editor::draw(sf::RenderTarget& w, sf::RenderStates states) const
{
	sf::View temp = w.getView();	//Store window view

	//Scrolling
	//Green mouse_box for geometry
	if (mode == EDIT && mouse_left && !selectClicked)
	{
		w.draw(mouse_box);
	}
	//White mouse_box for editing
	else if (mode == GEOM && mouse_left)
	{
		w.draw(mouse_box);
	}
	//Texture cursor
	else if (mode == DECO)
	{
		w.draw(deco_cursor, states);
	}
	//White boxes for selection
	if (selection.size())
	{
		sf::RectangleShape r;
		sf::Vector2f v;
		r.setOutlineThickness(2.0);
		r.setOutlineColor(sf::Color::White);
		r.setFillColor(sf::Color::Transparent);
		for (int i = selection.size()-1; i >= 0; i--)
		{
			v.x = selection[i].width;
			v.y = selection[i].height;
			r.setSize(v);
			v.x = selection[i].left;
			v.y = selection[i].top;
			r.setPosition(v);
			
			w.draw(r, states);
		}
	}

	//Static
	w.setView(w.getDefaultView());	//Reset window view to default
	//Console text and message
	if (console)
	{
		w.draw(command_text, states);
		w.draw(message_text, states);
	}
	//Img pane
	if (imgView)
	{
		int length = (int)(*maps)[map_index]->tx.size();
		sf::RectangleShape r;
		r.setPosition(816-2, 2);
		r.setOutlineThickness(2.0);
		r.setOutlineColor(sf::Color::Cyan);
		r.setFillColor(sf::Color::Transparent);
		for (int i = length-1; i >= 0; i--)
		{
			sf::Sprite sp((*maps)[map_index]->tx[i]->texture);
			sp.setPosition((i%3)*48+816-2,(i/3)*48+2);
			w.draw(sp, states);
		}
		r.setSize(sf::Vector2f((float)3*48,(float)(((length/3)+1)*48)));
		w.draw(r, states);
	}
	w.setView(temp);	//Restore view
}

bool Editor::getConsoleInput(const sf::Event &event, std::string &str)
{
	//Allowed characters
	//Alphabetical
	if (event.key.code >= 0 && event.key.code <= 25)
	{
		str += char(event.key.code + 65);
	}
	//Numerical
	else if (event.key.code >= 26 && event.key.code <= 35)
	{
		str += char(event.key.code - 26 + 48);
	}
	//Backspace
	else if (event.key.code == sf::Keyboard::BackSpace)
	{
		if (str != "")
		{
			str.pop_back(); 
		}
	}
	//Space
	else if (event.key.code == sf::Keyboard::Space)
	{
		str += ' ';
	}
	//Period
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

sf::Vector2f Editor::getMouseCoordinates()
{
	sf::Vector2f world_pos;
	sf::Vector2i local_pos;	

	local_pos = sf::Mouse::getPosition(*w);			//Get screen coords
	world_pos = w->mapPixelToCoords(local_pos);		//Translate to world coords

	if (snap)										//Apply snapping
	{
		world_pos.x = (int)world_pos.x - (int)world_pos.x % 48;
		world_pos.y = (int)world_pos.y - (int)world_pos.y % 48;
	}

	return world_pos;
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

void Editor::orient(sf::Vector2f &u, sf::Vector2f &v)
{
	float temp;
	if (v.x < u.x)	//if v is left of u, swap horizontally
	{
		temp = v.x;
		v.x = u.x;
		u.x = temp;
	}
	if (v.y < u.y)	//if v is below u, swap vertically
	{
		temp = v.y;
		v.y = u.y;
		u.y = temp;
	}
}

Editor::Editor(sf::RenderWindow* _w, std::vector<Map*>* _maps, sf::Font &_font)
{
	w								= _w;
	maps							= _maps;
	map_index						= 0;
	console 						= false;
	snap 							= false;
	command_text.setFont			(_font);
	command_text.setCharacterSize	(48);
	command_text.setFillColor		(sf::Color::White);
	command_text.setPosition		(5,427);
	command_text.setString			("$ ");
	message_text.setFont			(_font);
	message_text.setCharacterSize	(32);
	message_text.setFillColor		(sf::Color::White);
	message_text.setPosition		(5,0);

	mouse_box.setFillColor			(sf::Color(0,0,0,0));
	mouse_box.setOutlineColor		(sf::Color::Green);
	mouse_box.setOutlineThickness	(2.0f);
	mouse_left						= false;

	view.setSize					(960,480);
	view.setCenter					(480,240);
	mouse_middle 					= false;

	imgView							= false;
	setMode							(EDIT);
	selectClicked 					= false;
}

void Editor::setMode(Modes _mode)
{
	mode = _mode;
	if (_mode == EDIT)
	{
		mouse_box.setOutlineColor(sf::Color::White);
	}
	else if (_mode == GEOM)
	{
		mouse_box.setOutlineColor(sf::Color::Green);
	}
}

void Editor::helpCommand()
{
	std::string help_text;
	help_text += "QUIT: 			Exit editor\n";
	help_text += "CLEAR:		 	Clear message\n";
	help_text += "GEOM: 			Toggle map geometry\n";
	help_text += "DECO: 			Toggle map decorations (graphics)\n";
	help_text += "SNAP: 			Toggle snap modes\n\n";
	help_text += "MAP +\n";
	help_text += "LS: 			List maps\n";
	help_text += "SET <map>: 	Set working map to <map>\n";
	help_text += "NEW <file>: 	Create new map\n";
	help_text += "SAVE:		 	Save map\n\n";

	help_text += "IMG +\n";
	help_text += "LS: 			List imgs in map\n";
	help_text += "SET: 			Set working img\n";
	help_text += "LOAD <file>: 	Load img into map\n";
	help_text += "PANE:			Show img pane\n";

	message_text.setString(help_text);
}

void Editor::quitCommand(const std::string &_arg)
{
	if (_arg == "")
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
	else if (command.substr(4,16) == " OVERRIDE")
	{
		w->close();
	}
}

void Editor::toggleCommand(bool &togg, const std::string &_name)
{
	std::string message;
	togg = !togg;
	message = "-- ";
	message += _name + " " + (togg ? "TRUE" : "FALSE") + " --";
	message_text.setString(message);
}

void Editor::mapCommand()
{
	if (command.substr(4,2) == "LS")
	{
		std::string map_names;

		for (int i = 0; i < (int)maps->size(); i++)
		{
			map_names += (*maps)[i]->name + '\n';
		}

		message_text.setString(map_names);
	}
	else if (command.substr(4,3) == "SET")
	{
		std::string map_name = command.substr(8,16);
		int map_index_change = findMap(map_name);

		setMode(EDIT);
			
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
	else if (command.substr(4,3) == "NEW")
	{
		std::string map_name = command.substr(8,16);
		int map_index_new = findMap(map_name);

		setMode(EDIT);

		if (map_index_new < 0)
		{
			std::string message;
			maps->push_back(new Map(map_name));
			map_index = maps->size()-1;
			(*maps)[map_index]->name = map_name;

			message = "-- New Map ";
			message += map_name+"at map_index "+std::to_string(map_index)+" --";
			message_text.setString(message);
		}
		else
		{
			message_text.setString("-- Map Already Exists --");
		}
	}
	else if (command.substr(4,16) == "SAVE")
	{
		if (!((*maps)[map_index]->saveMap()))
		{
			message_text.setString(std::string("-- Save Map Failed --"));
		}
		else
		{
			std::string message =  "-- Saved Map ";
			message += (*maps)[map_index]->name+" --";
			message_text.setString(message);
		}
	}
	else unknownCommand();
}

void Editor::imgCommand()
{
	if (command.substr(4,2) == "LS")
	{
		std::string img_list;

		for (int i = 0; i < (int)(*maps)[map_index]->tx.size(); i++)
		{
			img_list += (*maps)[map_index]->tx[i]->name + "\n";
		}

		message_text.setString(img_list);
	}
	else if (command.substr(4,3) == "SET")
	{
		std::string img_set = command.substr(8,16);
		sf::Texture* tx = (*maps)[map_index]->getTexture(img_set);

		if (tx)
		{
			sf::Vector2i v(sf::Mouse::getPosition(*w));
			mode = DECO;
			deco_cursor.setTexture(*tx);
			deco_cursor.setPosition(sf::Vector2f((float)v.x, (float)v.y));
			deco_name = img_set;
		}
		else 
		{
			message_text.setString("-- Set Image Failed --");
		}
	}
	else if (command.substr(4,4) == "LOAD")
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
	else if (command.substr(4,4) == "PANE")
	{
		imgView = !imgView;
	}
	else unknownCommand();
}

void Editor::unknownCommand()
{
	message_text.setString("-- Unkown Command: " + command + " --");
}

void Editor::handleInput(sf::Event event)
{
	//
	//	Key Pressed
	//
	if (event.type == sf::Event::KeyPressed &&
		!mouse_left && !mouse_middle)
	{
		if (event.key.code == sf::Keyboard::Escape &&
			!mouse_left && !mouse_middle)
		{
			setMode(EDIT);
		}
		if (event.key.code == sf::Keyboard::Tab &&
			!mouse_left && !mouse_middle)
		{
			setMode(GEOM);
		}
		else if (event.key.code == sf::Keyboard::Delete)
		{
			if (!selection.empty())
			{
				(*maps)[map_index]->deleteSelect();
				selection.clear();
			}
		}
		else if (event.key.code == sf::Keyboard::D &&
				 sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		{
			if (!selection.empty())
			{
				(*maps)[map_index]->duplicateSelect();
			}
		}
		else if (console)
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
				if (command == 					"HELP")
				{
					helpCommand();
				}
				else if (command.substr(0,4) == "QUIT")
				{
					quitCommand(command.substr(4,16));
				}
				else if (command == 			"CLEAR")
				{
					message_text.setString("");
				}
				else if (command == 			"GEOM")
				{
					toggleCommand((*maps)[map_index]->geom, "Geometry");
				}
				else if (command == 			"DECO")
				{
					toggleCommand((*maps)[map_index]->deco, "Decoration");
				}
				else if (command == 			"SNAP")
				{
					toggleCommand(snap, "Snap");
				}
				else if (command.substr(0,3) == "MAP")
				{
					mapCommand();
				}
				else if (command.substr(0,3) ==	"IMG")
				{
					imgCommand();
				}
				else
				{
					unknownCommand();
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

		if (mode == EDIT)
		{
			if (imgView)
			{
				sf::FloatRect r;
				r.width = 48.0;
				r.height = 48.0;
				for (int i = (int)(*maps)[map_index]->tx.size()-1; i >= 0; i--)
				{
					r.left = (i%3)*48+816-2;
					r.top = i/3*48+2;
					if (r.contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
					{
						sf::Vector2i v(getMouseCoordinates());
						mode = DECO;
						deco_cursor.setTexture((*maps)[map_index]->tx[i]->texture);
						deco_cursor.setPosition(mouse_pos);
						deco_name = (*maps)[map_index]->tx[i]->name;
						break;
					}
				}
			}

			selectClicked = false;
			for (int i = selection.size()-1; i >=0; i--)
			{
				if (selection[i].contains(mouse_pos))
				{
					selectClicked = true;
					break;
				}
			}
			if (!selectClicked)
			{
				mouse_box.setPosition(mouse_pos);
				mouse_box.setSize(sf::Vector2f(0.0f, 0.0f));
			}
		}
		else if (mode == GEOM)
		{
			mouse_box.setPosition(mouse_pos);
			mouse_box.setSize(sf::Vector2f(0.0f, 0.0f));
		}
		else if (mode == DECO)
		{
			(*maps)[map_index]->addDeco(deco_name, mouse_pos);
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
		sf::Vector2f prev_mouse_pos = mouse_pos;

		mouse_pos = getMouseCoordinates();

		if (mode == GEOM)
		{
			orient(mouse_pos, prev_mouse_pos);

			(*maps)[map_index]->addWall(mouse_pos, prev_mouse_pos - mouse_pos);
		}
		else if (mode == EDIT)
		{
			if (selectClicked)
			{
				selectClicked = false;
			}
			else 
			{
				orient(mouse_pos, prev_mouse_pos);
				sf::FloatRect r(mouse_pos, prev_mouse_pos - mouse_pos);

				if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
					  sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)))
				{
					(*maps)[map_index]->clearSelect();
					selection.clear();
				}

				(*maps)[map_index]->select(r, selection);
			}
		}
	}
	//
	// Middle Mouse down
	//
	else if (event.type == sf::Event::MouseButtonPressed &&
			 event.mouseButton.button == sf::Mouse::Middle &&
			 !mouse_left)
	{
		mouse_middle = true;
		mouse_pos.x = (float)event.mouseButton.x;
		mouse_pos.y = (float)event.mouseButton.y;
	}
	//
	// Middle Mouse Up
	//
	else if (event.type == sf::Event::MouseButtonReleased &&
			 event.mouseButton.button == sf::Mouse::Middle &&
			 !mouse_left)
	{
		mouse_middle = false;
	}
	//
	// Mouse Moved
	//
	else if (event.type == sf::Event::MouseMoved)
	{
		sf::Vector2f move_mouse_pos = getMouseCoordinates();
		sf::Vector2f prev_mouse_pos = mouse_pos;

		if (mode == EDIT && !mouse_middle)
		{
			if (selectClicked)
			{
				mouse_pos = move_mouse_pos;

				(*maps)[map_index]->moveSelect(move_mouse_pos - prev_mouse_pos);

				for (int i = selection.size()-1; i >= 0; i--)
				{
					selection[i].top += (move_mouse_pos - prev_mouse_pos).y;
					selection[i].left += (move_mouse_pos - prev_mouse_pos).x;
				}
			}
			else
			{
				orient(prev_mouse_pos, move_mouse_pos);

				mouse_box.setPosition(prev_mouse_pos);
				mouse_box.setSize(move_mouse_pos - prev_mouse_pos);
			}
		}
		if (mode == GEOM && !mouse_middle)
		{
			orient(prev_mouse_pos, move_mouse_pos);

			mouse_box.setPosition(prev_mouse_pos);
			mouse_box.setSize(move_mouse_pos - prev_mouse_pos);
		}
		else if (mode == DECO && !mouse_middle)
		{
			deco_cursor.setPosition(move_mouse_pos);
		}
		if (mouse_middle)
		{
			sf::Vector2f v((float)event.mouseMove.x, (float)event.mouseMove.y);
			view.move(mouse_pos - v);
			mouse_pos = v;
		}
	}
}
