#ifdef EDITOR_BUILD
#include "editor.h"

void Editor::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	if (snap) {
		sf::RectangleShape rc;
		sf::RectangleShape r;
		sf::Color color(255,0,0,32);

		rc.setSize(sf::Vector2f(1920,1080));
		rc.setOutlineThickness(2.0);
		rc.setOutlineColor(sf::Color::Red);
		rc.setFillColor(sf::Color::Transparent);

		r.setSize(sf::Vector2f(sv.x, sv.y));
		r.setOutlineThickness(-1.0);
		r.setOutlineColor(color);
		r.setFillColor(sf::Color::Transparent);

		int max_i = 960 / sv.x + 4;
		int max_j = 480 / sv.y + 4;

		sf::Vector2f offset =
		view.getCenter() - 
		sf::Vector2f(max_i * sv.x / 2, max_j * sv.y/ 2);

		offset = sf::Vector2f(
			int(offset.x) - int(offset.x)%sv.x, 
			int(offset.y) - int(offset.y)%sv.y
		);

		for (int i = 0; i < max_i; i++) {
			for (int j = 0; j < max_j; j++) {
				r.setPosition(offset +
				sf::Vector2f(i*sv.x+sv.xo,j*sv.y+sv.yo));
				w.draw(r, states);
			}
		}
		w.draw(rc, states);
	}

	w.setView(temp);
	if (mode == EDIT_DECO) {
		w.draw(*decopanel, states);
	} else if (mode == EDIT_ENT) {
		w.draw(*entpanel, states);
	} else if (mode == EDIT_GEOM) {
		w.draw(*geompanel, states);
	} else if (mode == EDIT_PORT) {
		w.draw(*portpanel, states);
	} else if (mode == EDIT_GLST) {
		w.draw(*glstpanel, states);
	} else if (mode == EDIT_ILST) {
		w.draw(*ilstpanel, states);
	} else if (mode == EDIT_MAP) {
		w.draw(*mappanel, states);
	} else if (mode == EDIT_RECT) {
		w.draw(*rectpanel, states);
	} else if (mode == EDIT_SNAP) {
		w.draw(*snappanel, states);
	}
	if (rectpanel_rects) {
		rectpanel->drawRects(w, states);
	}

	w.setView(Window::default_view);
	for (int i = 0; i < 15; i++ ) {
		w.draw(*buttons[i], states);
	}
	w.draw(selected_pan_r, states);
}

Editor::Editor(sf::RenderWindow* _w, Game* _game) {
	log_dbg("constructing editor");
	w		= _w;
	game	= _game;

	mode = EDIT_MAP;

	for (int i = 0; i < 15; i++) {
		buttons[i] = gen_button(i);
	}
	selected_pan_r = sf::RectangleShape(sf::Vector2f(4,4));
	selected_pan_r.setFillColor(sf::Color(127,127,127));
	selected_pan_r.setPosition(buttons[0]->getPosition());
	selected_pan_r.move(3,3);

	decopanel	= new Decopanel(game, &sv);
	entpanel	= new Entpanel(game, &sv);
	mappanel	= new Mappanel(game);
	geompanel	= new Geompanel(game, &sv);
	portpanel	= new Portpanel(game, &sv);
	glstpanel	= new Glstpanel(game);
	ilstpanel	= new Ilstpanel(game);

	rectpanel		= new Rectpanel(&sv);
	rectpanel_rects	= false;

	snappanel	= new Snappanel(&sv);
	snap		= false;
	sv.x		= 1;
	sv.y		= 1;
	sv.xo		= 0;
	sv.yo		= 0;
	sv_b.x		= 32;
	sv_b.y		= 32;
	sv_b.xo		= 0;
	sv_b.yo 	= 0;

	p_m_pos			= sf::Vector2i(0,0);
	mouse_middle	= false;

	view.setSize(960,480);
	view.setCenter(480,240);
	log_dbg("done");
}

void Editor::handle_input(sf::Event &event) {
	sf::Vector2i m_pos = sf::Mouse::getPosition(*w);
	sf::Vector2f w_pos = vivf(m_pos) + 
	(view.getCenter() - view.getSize() / 2.f) * Window::scale - Window::offset;
	w_pos /= Window::scale;
	m_pos = vfvi((vivf(m_pos) - Window::offset) / Window::scale);

	//ctrl + shift + left click to place player
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
	sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) &&
	(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
	sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) &&
	event.type == sf::Event::MouseButtonPressed &&
	event.mouseButton.button == sf::Mouse::Left) {
		game->player.setPosition(w_pos);
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
					mode = EDIT_SNAP;
					selected_pan_r.setPosition(buttons[i]->getPosition());
					selected_pan_r.move(3,3);
					if (!snap) {
						snap = true;
						sv = sv_b;
						snappanel->reset();
					}
				} else if (bs == BCLICKR) {
					snap = !snap;
					if (snap) {
						sv = sv_b;
					} else {
						sv_b = sv;
						sv.x = 1;
						sv.y = 1;
						sv.xo = 0;
						sv.yo = 0;
					}
					snappanel->reset();
				}
				break;
			case DECO_BUTTON:
				if (bs == BCLICK) {
					mode = EDIT_DECO;
					selected_pan_r.setPosition(buttons[i]->getPosition());
					selected_pan_r.move(3,3);
				} else if (bs == BCLICKR) {
					game->map_current->draw_deco = 
					!game->map_current->draw_deco;
				}
				break;
			case GEOM_BUTTON:
				if (bs == BCLICK) {
					mode = EDIT_GEOM;
					selected_pan_r.setPosition(buttons[i]->getPosition());
					selected_pan_r.move(3,3);
					game->map_current->draw_geom = true;
				} else if (bs == BCLICKR) {
					game->map_current->draw_geom =
					!game->map_current->draw_geom;
				}
				break;
			case POR_BUTTON:
				if (bs == BCLICK) {
					mode = EDIT_PORT;
					selected_pan_r.setPosition(buttons[i]->getPosition());
					selected_pan_r.move(3,3);
				} else if (bs == BCLICKR) {
				}
				break;
			case ENT_BUTTON:
				if (bs == BCLICK) {
					mode = EDIT_ENT;
					selected_pan_r.setPosition(buttons[i]->getPosition());
					selected_pan_r.move(3,3);
				} else if (bs == BCLICKR) {
				}
				break;
			case ENM_BUTTON:
				break;
			case RECT_BUTTON:
				if (bs == BCLICK) {
					rectpanel_rects = false;
					mode = EDIT_RECT;
					selected_pan_r.setPosition(buttons[i]->getPosition());
					selected_pan_r.move(3,3);
				} else if (bs == BCLICKR) {
					if (rectpanel_rects) {
						rectpanel_rects = false;
					} else if (mode != EDIT_RECT) {
						rectpanel_rects = true;
					}
				}
				break;
			case ILST_BUTTON:
				if (bs == BCLICK) {
					mode = EDIT_ILST;
					selected_pan_r.setPosition(buttons[i]->getPosition());
					selected_pan_r.move(3,3);
				} else if (bs == BCLICKR) {
				}
				break;
			case GLST_BUTTON:
				if (bs == BCLICK) {
					mode = EDIT_GLST;
					selected_pan_r.setPosition(buttons[i]->getPosition());
					selected_pan_r.move(3,3);
				} else if (bs == BCLICKR) {
				}
				break;
			}
			return;
		}
	}

	if (mode == EDIT_DECO) {
		decopanel->handle_input(event, m_pos, w_pos);
	} else if (mode == EDIT_GEOM) {
		geompanel->handle_input(event, w_pos);
	} else if (mode == EDIT_ENT) {
		entpanel->handle_input(event, m_pos, w_pos);
	} else if (mode == EDIT_PORT) {
		portpanel->handle_input(event, m_pos, w_pos);
	} else if (mode == EDIT_GLST) {
		glstpanel->handle_input(event, m_pos);
	} else if (mode == EDIT_ILST) {
		ilstpanel->handle_input(event, m_pos);
	} else if (mode == EDIT_MAP) {
		mappanel->handle_input(event, m_pos);
	} else if (mode == EDIT_RECT) {
		rectpanel->handle_input(event, m_pos, w_pos);
	} else if (mode == EDIT_SNAP) {
		snappanel->handle_input(event, m_pos);
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
		p_m_pos = m_pos;
	}
	// Middle Mouse Up
	else if (event.type == sf::Event::MouseButtonReleased &&
	event.mouseButton.button == sf::Mouse::Middle && !mouse_b_d) {
		mouse_middle = false;
	}
	// Mouse Moved
	else if (
	event.type == sf::Event::MouseMoved && mouse_middle && !mouse_b_d) {
		view.move(vivf(p_m_pos - m_pos));
		p_m_pos = m_pos;
	}
}

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
	if (mode == EDIT_RECT) {
		sf::View* r_view = rectpanel->update();
		if (r_view) {
			view = *r_view;
			delete r_view;
		}
	}

	sf::Vector2f view_center = view.getCenter();
	view = Window::default_view;
	view.setCenter(view_center);

	return ret;
}

void Editor::reset() {
	mode = EDIT_MAP;
	selected_pan_r.setPosition(buttons[0]->getPosition());
	selected_pan_r.move(3,3);
	decopanel->reset();
	entpanel->reset();
	geompanel->reset();
	portpanel->reset();
	glstpanel->reset();
	ilstpanel->reset();
	mappanel->reset();
	rectpanel->reset();
}

Button* Editor::gen_button(int i) {
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
	case RECT_BUTTON:
		name = "rectangles";
		break;
	case ILST_BUTTON:
		name = "init lstate";
		break;
	case GLST_BUTTON:
		name = "game lstate";
		break;
	default:
		name = "test button " + to_string(i + 1);
	}
	Button* ret = new Button(
			name,
			sf::FloatRect(i*64,0,64,16)
	);
	ret->normal_b = UIC_ED_BUTTON_BODY_HOVER;
	ret->hover_b = UIC_ED_BUTTON_BODY_NORMAL;
	ret->normal_t = UIC_ED_BUTTON_TEXT_HOVER;
	ret->hover_t = UIC_ED_BUTTON_TEXT_NORMAL;
	ret->normal_o = UIC_ED_BUTTON_OUTLINE_HOVER;
	ret->hover_o = UIC_ED_BUTTON_OUTLINE_NORMAL;
	ret->setColors();
	return ret;
}

Editor::~Editor() {
	log_dbg("destructing editor");
}

#endif
