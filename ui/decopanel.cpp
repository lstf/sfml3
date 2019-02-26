#include "decopanel.h"

void Decopanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	sf::RectangleShape selected_rect(sf::Vector2f(0,0));
	selected_rect.setFillColor(sf::Color(0,0,0,0));
	selected_rect.setOutlineColor(sf::Color(255,255,255));
	selected_rect.setOutlineThickness(-1.0);
	for (int i = selected.size() - 1; i >= 0; i--) {
		selected_rect.setPosition(map->bg[selected[i]]->sp.getPosition());
		sf::FloatRect bgb = map->bg[selected[i]]->sp.getGlobalBounds();
		selected_rect.setSize(sf::Vector2f(bgb.width, bgb.height));
		w.draw(selected_rect, states);
	}

	w.setView(w.getDefaultView());
	w.draw(preview_bg, states);
	w.draw(preview_sp, states);
	w.draw(file_bg, states);
	w.draw(menu_bg, states);

	w.setView(scrollview);
	for (unsigned int i = 0; i < button_count; i++) {
		w.draw(*(buttons[i].btn), states);
	}

	w.setView(w.getDefaultView());
	w.draw(*scroll, states);

	w.setView(temp);
	if (state == DECO_ADD) {
		w.draw(active_sp, states);
	} else if (state == DECO_EDIT && select_click) {
		w.draw(select_r, states);
	}
}

void Decopanel::handle_input(
sf::Event &event, sf::Vector2i m_pos, sf::Vector2f w_pos) {
	sf::Vector2f w_pos_snap = snap(w_pos, *sv);
	bool shift = 
	sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || 
	sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

	//buttons
	for (unsigned int i = 0; i < button_count; i++) {
		BState bs = buttons[i].btn->handle_input(event, m_pos);	
		//hover to show preview
		if (bs == BHOVER) {
			sf::Texture* tx = txmap::get_tx(buttons[i].file);
			preview_sp.setTexture(*tx, true);
			sf::FloatRect bounds = preview_sp.getGlobalBounds();
			if (bounds.width > DECO_P_SIZE || bounds.height > DECO_P_SIZE) {
				sf::IntRect r;
				r.width = bounds.width;
				r.height = bounds.width;
				if (bounds.width > DECO_P_SIZE) {
					r.left = int((bounds.width - DECO_P_SIZE) / 2);
					r.width = DECO_P_SIZE;
				} else if (bounds.height > DECO_P_SIZE) {
					r.top = int((bounds.height - DECO_P_SIZE) / 2);
					r.height = DECO_P_SIZE;
				}
				preview_sp.setTextureRect(r);
			}
			bounds = preview_sp.getGlobalBounds();
			preview_sp.setPosition(
				(DECO_P_SIZE - bounds.width) / 2,
				(DECO_P_SIZE - bounds.height) / 2 + DECO_BASE_H
			);
		//click to set add state
		} else if (bs == BCLICK) {
			if (selected.size() > 0) {
				vector<img> ims;
				auto mb = map->bg.begin();
				for (auto it = selected.begin(); it != selected.end(); ++it) {
					ims.push_back(*map->bg.at(*it));
					map->bg.erase(mb + *it);
				}
				for (auto it = ims.begin(); it != ims.end(); ++it) {
					map->addDeco(buttons[i].file, it->sp.getPosition());
				}
				selected.clear();
			} else {
				sf::Texture* tx = txmap::get_tx(buttons[i].file);
				active_sp.setPosition(w_pos_snap);
				active_sp.setTexture(*tx, true);
				active_name = buttons[i].file;
				state = DECO_ADD;
			}
		}
	}

	//scroll bar
	float scroll_val = scroll->handle_input(event, m_pos);
	for (unsigned int i = 0; i < button_count; i++) {
		sf::Vector2f b_pos = buttons[i].btn->getPosition();
		b_pos.y = 
		(i/2)*DECO_B_SIZE/4+DECO_P_SIZE+DECO_BASE_H+int(scroll_val*scroll_max);
		b_pos.x = (i%2)*DECO_B_SIZE;
		buttons[i].btn->setPosition(b_pos);
	}

	//ui click filter
	if (event.type == sf::Event::MouseButtonPressed &&
	event.mouseButton.button == sf::Mouse::Left && bounds.contains(m_pos)) {
		return;
	}

	//add state
	if (state == DECO_ADD) {
		active_sp.setPosition(w_pos_snap);
		//Left click to add to map
		if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Left) {
			map->addDeco(active_name, w_pos_snap);
		}
	}

	//edit state
	if (state == DECO_EDIT) {
		//Left click
		if (event.type == sf::Event::MouseButtonPressed &&
		event.mouseButton.button == sf::Mouse::Left) {
			for (auto it = selected.begin(); it != selected.end(); ++it) {
				if (
				map->bg[*it]->sp.getGlobalBounds().contains(w_pos_snap)) {
					selected_click = true;
					return;	
				}
			}
			select_click = true;
			if (!shift) {
				selected.clear();
			}
			select_r.setPosition(w_pos_snap);
			select_r.setSize(sf::Vector2f(1, 1));
		//Left release
		} else if (event.type == sf::Event::MouseButtonReleased &&
		event.mouseButton.button == sf::Mouse::Left) {
			if (select_click) {
				select_click = false;
				sf::FloatRect sb = select_r.getGlobalBounds();
				for (int i = map->bg.size() - 1; i >= 0; i--) {
					sf::FloatRect bgb = map->bg[i]->sp.getGlobalBounds();
					if (sb.intersects(bgb)) {
						selected.push_back(i);
					}
					sort(selected.begin(), selected.end(), greater<int>());
					auto se = unique(selected.begin(), selected.end());
					selected.erase(se, selected.end());
				}
			} else if (selected_click) {
				selected_click = false;
			}
		//Mouse moved
		} else if (event.type == sf::Event::MouseMoved) {
			if (select_click) {
				select_r.setSize(w_pos_snap - select_r.getPosition());
			} else if (selected_click) {
				for (auto it = selected.begin(); it != selected.end(); ++it) {
					map->bg[*it]->sp.move(w_pos_snap - select_p_mouse);
				}
			}
			select_p_mouse = w_pos_snap;
		//Key pressed
		} else if (event.type == sf::Event::KeyPressed) {
			//Delete key
			if (event.key.code == sf::Keyboard::Delete) {
				auto mb = map->bg.begin();
				for (auto it = selected.begin(); it != selected.end(); ++it) {
					map->bg.erase(mb + *it);
				}
				selected.clear();
			//Shift d, duplicate
			} else if (event.key.code == sf::Keyboard::D && shift) {
				for (auto it = selected.begin(); it != selected.end(); ++it) {
					img* im = map->bg[*it];
					map->addDeco(im->name, im->sp.getPosition());
				}
			//Shifd b, auto geom
			} else if (event.key.code == sf::Keyboard::B && shift) {
				vector<sf::FloatRect> sb = gen_geom();
				for (auto it = sb.begin(); it != sb.end(); ++it) {
					map->geometry.push_back(*it);
				}
			}

		}
	}

	//escape
	if (event.type == sf::Event::KeyPressed &&
	event.key.code == sf::Keyboard::Escape) {
		state = DECO_EDIT;
	}
}

void Decopanel::reset() {
	select_click = false;
	selected_click = false;
	selected.clear();
	state = DECO_EDIT;
	map = game->map_current;
}

Decopanel::Decopanel(Game* _game, SnapVals* _sv) {
	game	= _game;
	map		= game->map_current;
	sv		= _sv;

	bg_setup();
	button_setup();
	scroll_setup();
	select_setup();

	state = DECO_EDIT;

	bounds = sf::IntRect(0,0,DECO_P_SIZE+16,480);
}

void Decopanel::bg_setup() {
	preview_bg = 
	sf::RectangleShape(sf::Vector2f(DECO_P_SIZE, DECO_P_SIZE));
	preview_bg.setPosition(0, DECO_BASE_H);
	preview_bg.setFillColor(DECO_BG);
	preview_bg.setOutlineColor(DECO_FG);
	preview_bg.setOutlineThickness(-1.0);

	file_bg = 
	sf::RectangleShape(sf::Vector2f(DECO_P_SIZE, 480 - DECO_P_SIZE));
	file_bg.setPosition(0, DECO_BASE_H+DECO_P_SIZE);
	file_bg.setFillColor(DECO_BG);
	file_bg.setOutlineColor(DECO_FG);
	file_bg.setOutlineThickness(-1.0);

	menu_bg = 
	sf::RectangleShape(sf::Vector2f(16, DECO_P_SIZE));
	menu_bg.setPosition(DECO_P_SIZE, DECO_BASE_H);
	menu_bg.setFillColor(DECO_BG);
	menu_bg.setOutlineColor(DECO_FG);
	menu_bg.setOutlineThickness(-1.0);
}

void Decopanel::button_setup() {
	ifstream inp("./ats/mps/tx/tx.list");
	if (!inp.is_open()) {
		cout << "[DECOPANEL] cant open tx.list" << endl;
		return;
	}
	string file;
	vector<string> files;
	while (inp >> file) {
		files.push_back(file);
	}
	inp.close();

	button_count = files.size();
	buttons = new Decobutton[button_count];
	for (unsigned int i = 0; i < button_count; i++) {
		buttons[i].btn = new Button(
			DECO_BG, DECO_FG, 
			files[i],
			sf::FloatRect(
				(i%2)*DECO_B_SIZE,
				(i/2)*DECO_B_SIZE/4+DECO_BASE_H+DECO_P_SIZE,
				DECO_B_SIZE,
				DECO_B_SIZE/4
			)
		);
		buttons[i].file = "./ats/mps/tx/" + files[i];
	}
}

void Decopanel::scroll_setup() {
	sf::IntRect bt = buttons[0].btn->bounds;
	sf::IntRect bb = buttons[button_count - 1].btn->bounds;
	
	scrollview = sf::View(
		sf::Vector2f(
		DECO_P_SIZE / 2, (480 - DECO_P_SIZE - DECO_BASE_H) / 2 + 
		DECO_P_SIZE + DECO_BASE_H),
		sf::Vector2f(DECO_P_SIZE, 480 - DECO_P_SIZE - DECO_BASE_H)
	);
	scrollview.setViewport(sf::FloatRect(
		0,
		(DECO_P_SIZE+DECO_BASE_H)/480.0,
		DECO_P_SIZE/960.0,
		(480 - DECO_P_SIZE - DECO_BASE_H)/480.0
	));
	int scroll_content_height = bb.top + bb.height - bt.top;
	scroll_max = 480 - DECO_P_SIZE - scroll_content_height;
	scroll = new Scrollbar(
		480 - DECO_P_SIZE - DECO_BASE_H,
		scroll_content_height,
		DECO_P_SIZE,
		DECO_BASE_H + DECO_P_SIZE,
		16,
		480 - DECO_P_SIZE - DECO_BASE_H
	);
}

void Decopanel::select_setup() {
	select_r = sf::RectangleShape(sf::Vector2f(0,0));
	select_r.setFillColor(sf::Color(0,0,0,0));
	select_r.setOutlineColor(sf::Color(255,255,255));
	select_r.setOutlineThickness(-1.0);

	select_click = false;
	selected_click = false;
}

bool gen_geom_over(sf::FloatRect u, sf::FloatRect v) {
	return 
	u.left == v.left && 
	u.width == v.width &&
	u.top + u.height + 1 > v.top;
}

bool gen_geom_next_to(sf::FloatRect u, sf::FloatRect v) {
	return 
	u.top == v.top &&
	u.height == v.height &&
	u.left + u.width + 1 > v.left;
}

bool gen_geom_comp(
const sf::FloatRect &u, const sf::FloatRect &v) {
	if (u.top == v.top) {
		if (u.left < v.left) {
			return true;
		}
		return false;
	} else if (u.top < v.top) {
		return true;
	} 
	return false;
}

vector<sf::FloatRect> Decopanel::gen_geom() {
	vector<sf::FloatRect> rects;
	vector<sf::FloatRect> rects2;
	for (auto it = selected.begin(); it != selected.end(); ++it) {
		rects.push_back(map->bg[*it]->sp.getGlobalBounds());
	}
	sort(rects.begin(),rects.end(),gen_geom_comp);

	auto iti = rects.begin();
	sf::FloatRect r = *iti;
	for (auto it = iti + 1; it != rects.end(); ++it) {
		if (gen_geom_next_to(*(it - 1), *it)) {
			r.width = it->left + it->width - r.left;
		} else {
			rects2.push_back(r);
			r = *it;
		}
	}
	rects2.push_back(r);
	rects.clear();
	iti = rects2.begin();
	r = *iti;
	for (auto it = iti + 1; it != rects2.end(); ++it) {
		if (gen_geom_over(*(it - 1), *it)) {
			r.height = it->top + it->height - r.top;
		} else {
			rects.push_back(r);
			r = *it;
		}
	}
	rects.push_back(r);

	return rects;
}
