#include "editor.h"

EditorTrans* Editor::update() {
	EditorTrans* ret = NULL;
	if (mode == EDIT_MAP) {
		MappanelTrans* m_t = mappanel->update();
		if (m_t) {
			ret = new EditorTrans;
			ret->name = m_t->name;
			ret->position = sf::Vector2f(0, 0);
			ret->is_new = m_t->is_new;
			delete m_t;
		}
	}
	if (mode == EDTRANS) {
		ret = trans;
	}
	return ret;
}

void Editor::reset() {
	mode = EDIT_MAP;
	decopanel->reset();
	geompanel->reset();
	mappanel->reset();
}

void Editor::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();	//Store window view
	if (snap) {
		sf::RectangleShape rc;
		sf::RectangleShape r;
		sf::Color color(255,0,0,32);

		rc.setSize(sf::Vector2f(1920,1080));
		rc.setOutlineThickness(2.0);
		rc.setOutlineColor(sf::Color::Red);
		rc.setFillColor(sf::Color::Transparent);
		r.setSize(sf::Vector2f(32,32));
		r.setOutlineThickness(1.0);
		r.setOutlineColor(color);
		r.setFillColor(sf::Color::Transparent);
		sf::Vector2f offset = view.getCenter() - sf::Vector2f(512,272);
		sf::Vector2i snap_coord((int)offset.x,(int)offset.y);
		offset -= sf::Vector2f(snap_coord.x, snap_coord.y);
		snap_coord -= sf::Vector2i(snap_coord.x%32, snap_coord.y%32);
		offset += sf::Vector2f(snap_coord.x, snap_coord.y);
		for (int i = 0; i < 32; i++) {
			for (int j = 0; j < 17; j++) {
				r.setPosition(offset + sf::Vector2f(i*32,j*32));
				w.draw(r, states);
			}
		}
		w.draw(rc, states);
	}

	//Static
	w.setView(w.getDefaultView());	//Reset window view to default
	for (int i = 0; i < 15; i++ ) {
		w.draw(*buttons[i], states);
	}
	if (textbox_input) {
		w.draw(*textbox, states);
	}
	w.draw(selected_pan_r, states);

	w.setView(temp);	//Restore view
	if (mode == EDIT_DECO) {
		w.draw(*decopanel, states);
	} else if (mode == EDIT_GEOM) {
		w.draw(*geompanel, states);
	} else if (mode == EDIT_MAP) {
		w.draw(*mappanel, states);
	}

}

sf::Vector2f Editor::getMouseCoordinates() {
	sf::Vector2f world_pos;
	sf::Vector2i local_pos;	

	local_pos = sf::Mouse::getPosition(*w);			//Get screen coords
	world_pos = w->mapPixelToCoords(local_pos);		//Translate to world coords

	if (snap)										//Apply snapping
	{
		world_pos.x = (int)world_pos.x - ((int)world_pos.x) % 32;
		world_pos.y = (int)world_pos.y - ((int)world_pos.y) % 32;
		if (world_pos.x < 0) world_pos.x -= 32;
		if (world_pos.y < 0) world_pos.y -= 32;
	}

	return world_pos;
}

Button* gen_button(int i) {
	string name;
	switch (i) {
	case MAP_BUTTON:
		name = "map";
		break;
	case SNAP_BUTTON:
		name = "snap";
		break;
	case DECO_BUTTON: 
		name = "decoration";
		break;
	case GEOM_BUTTON: 
		name = "geometry";
		break;
	case POR_BUTTON: 
		name = "portals";
		break;
	case ENT_BUTTON:
		name = "entities";
		break;
	case ENM_BUTTON:
		name = "enemies";
		break;
	default:
		name = "test button " + to_string(i + 1);
	};
	return new Button(
			sf::Color::Black,
			sf::Color::White,
			name,
			sf::FloatRect(i*64,0,64,16)
	);
}

Editor::Editor(sf::RenderWindow* _w, Game* _game) {
	game							= _game;
	w								= _w;
	console 						= false;
	snap 							= false;
	command_text.setFont			(*txmap::get_font("./ats/fonts/thintel.ttf"));
	command_text.setCharacterSize	(48);
	command_text.setFillColor		(sf::Color::White);
	command_text.setPosition		(5,427);
	command_text.setString			("$ ");
	message_text.setFont			(*txmap::get_font("./ats/fonts/thintel.ttf"));
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
	mode = EDIT_MAP;
	selectClicked 					= false;

	textbox_input = false;

	for (int i = 0; i < 15; i++) {
		buttons[i] = gen_button(i);
	}

	selected_pan_r = sf::RectangleShape(sf::Vector2f(4,4));
	selected_pan_r.setFillColor(sf::Color::White);
	selected_pan_r.setPosition(buttons[0]->getPosition());
	selected_pan_r.move(3,3);

	decopanel = new Decopanel(game);
	mappanel = new Mappanel(game);
	geompanel = new Geompanel(game);
}

sf::View Editor::getView() {
	return view;
}

void Editor::handleInput(sf::Event event) {
	sf::Vector2i m_pos = sf::Mouse::getPosition(*w);
	sf::Vector2f w_pos = w->mapPixelToCoords(m_pos);

	//ctrl + shift + left click to place player
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
	sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) &&
	(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
	sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) &&
	event.type == sf::Event::MouseButtonPressed &&
	event.mouseButton.button == sf::Mouse::Left) {
		game->player.setPosition(w_pos);
	}

	//textbox
	if (textbox_input) {
		string* str = textbox->handle_input(event);
		if (str) {
			*textbox_target = *str;
			cout << *textbox_target << endl;
			textbox_input = false;
			delete textbox;
			delete str;
		}
		return;
	}
	//buttons
	for (int i = 0; i < 15; i++) {
		BState bs = buttons[i]->handle_input(event, m_pos);
		if (bs != BNONE && bs != BHOVER) {
			switch (i) {
			case MAP_BUTTON:
				if (bs == BCLICK) {
					mode = EDIT_MAP;
					selected_pan_r.setPosition(buttons[i]->getPosition());
					selected_pan_r.move(3,3);
				}
				break;
			case SNAP_BUTTON:
				if (bs == BCLICK) {
					snap = !snap;
				}
				break;
			case DECO_BUTTON:
				if (bs == BCLICK) {
					mode = EDIT_DECO;
					selected_pan_r.setPosition(buttons[i]->getPosition());
					selected_pan_r.move(3,3);
				} else if (bs == BCLICKR) {
					game->map_current->deco = !game->map_current->deco;
				}
				break;
			case GEOM_BUTTON:
				if (bs == BCLICK) {
					mode = EDIT_GEOM;
					selected_pan_r.setPosition(buttons[i]->getPosition());
					selected_pan_r.move(3,3);
				} else if (bs == BCLICKR) {
					game->map_current->geom = !game->map_current->geom;
				}
				break;
			case POR_BUTTON:
				break;
			case ENT_BUTTON:
				break;
			case ENM_BUTTON:
				break;
			}
			return;
		}
	}

	if (mode == EDIT_DECO) {
		decopanel->handle_input(event, m_pos, w_pos, snap ? 32 : 1);
	} else if (mode == EDIT_GEOM) {
		geompanel->handle_input(event, w_pos, snap ? 32 : 1);
	} else if (mode == EDIT_MAP) {
		mappanel->handle_input(event, m_pos);
	}

	bool mouse_b_d = 
	sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
	sf::Mouse::isButtonPressed(sf::Mouse::Right);


	//
	// Middle Mouse down
	//
	if (event.type == sf::Event::MouseButtonPressed &&
	event.mouseButton.button == sf::Mouse::Middle && !mouse_b_d) {
		mouse_middle = true;
		mouse_pos.x = (float)event.mouseButton.x;
		mouse_pos.y = (float)event.mouseButton.y;
	}
	//
	// Middle Mouse Up
	//
	else if (event.type == sf::Event::MouseButtonReleased &&
	event.mouseButton.button == sf::Mouse::Middle && !mouse_b_d) {
		mouse_middle = false;
	}
	//
	// Mouse Moved
	//
	else if (
	event.type == sf::Event::MouseMoved && mouse_middle && !mouse_b_d) {
		sf::Vector2f v((float)event.mouseMove.x, (float)event.mouseMove.y);
		view.move(mouse_pos - v);
		mouse_pos = v;
	}
}
