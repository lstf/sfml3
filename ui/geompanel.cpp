#include "geompanel.h"

void Geompanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::RectangleShape selected_rect(sf::Vector2f(0,0));
	selected_rect.setFillColor(sf::Color(0,0,0,0));
	selected_rect.setOutlineColor(sf::Color(255,255,255));
	selected_rect.setOutlineThickness(-1.0);
	for (int i = selected.size() - 1; i >= 0; i--) {
		sf::FloatRect gr = map->geometry[selected[i]];
		selected_rect.setPosition(gr.left, gr.top);
		sf::FloatRect bgb = map->geometry[selected[i]];
		selected_rect.setSize(sf::Vector2f(bgb.width, bgb.height));
		w.draw(selected_rect, states);
	}
	if (select_click || right_click) {
		w.draw(select_r, states);
	}
}

void Geompanel::handle_input(
sf::Event &event, sf::Vector2f w_pos) {
	sf::Vector2f w_pos_snap = snap(w_pos, *sv);
	bool shift = 
	sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || 
	sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

	//Click
	if (event.type == sf::Event::MouseButtonPressed) {
		//Left click
		if (event.mouseButton.button == sf::Mouse::Left && !right_click) {
			for (auto it = selected.begin(); it != selected.end(); ++it) {
				if (map->geometry[*it].contains(w_pos_snap)) {
					selected_click = true;
					return;	
				}
			}
			select_click = true;
			if (!shift) {
				selected.clear();
			}
			select_r.setOutlineColor(sf::Color::White);
			select_r.setPosition(w_pos_snap);
			select_r.setSize(sf::Vector2f(1, 1));
		//Right click
		} else if (event.mouseButton.button == sf::Mouse::Right && 
		!select_click && !selected_click) {
			right_click = true;
			select_r.setOutlineColor(sf::Color::Green);
			select_r.setPosition(w_pos_snap);
			select_r.setSize(sf::Vector2f(1, 1));
		}
	//Left Release
	} else if (event.type == sf::Event::MouseButtonReleased) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (select_click) {
				select_click = false;
				sf::FloatRect sb = select_r.getGlobalBounds();
				for (int i = map->geometry.size() - 1; i >= 0; i--) {
					if (sb.intersects(map->geometry[i])) {
						selected.push_back(i);
					}
					sort(selected.begin(), selected.end(), greater<int>());
					auto se = unique(selected.begin(), selected.end());
					selected.erase(se, selected.end());
				}
			} else if (selected_click) {
				selected_click = false;
			}
		} else if (event.mouseButton.button == sf::Mouse::Right) {
			right_click = false;
			map->geometry.push_back(select_r.getGlobalBounds());
		}
	//Mouse moved
	} else if (event.type == sf::Event::MouseMoved) {
		if (select_click || right_click) {
			select_r.setSize(w_pos_snap - select_r.getPosition());
		} else if (selected_click) {
			for (auto it = selected.begin(); it != selected.end(); ++it) {
				sf::FloatRect* gr = &map->geometry[*it];
				sf::Vector2f diff = w_pos_snap - select_p_mouse;
				gr->left += diff.x;
				gr->top += diff.y;
			}
		} 
		select_p_mouse = w_pos_snap;
	//Key pressed
	} else if (event.type == sf::Event::KeyPressed) {
		//Delete key
		if (event.key.code == sf::Keyboard::Delete) {
			auto mb = map->geometry.begin();
			for (auto it = selected.begin(); it != selected.end(); ++it) {
				map->geometry.erase(mb + *it);
			}
			selected.clear();
		//Shift d, duplicate
		} else if (event.key.code == sf::Keyboard::D && shift) {
			for (auto it = selected.begin(); it != selected.end(); ++it) {
				map->geometry.push_back(map->geometry[*it]);
			}
		} 
	}
}

void Geompanel::reset() {
	selected.clear();
	select_click	= false;
	selected_click	= false;
	right_click		= false;
	map = game->map_current;
}

Geompanel::Geompanel(Game* _game, SnapVals* _sv) {
	game	= _game;
	map		= game->map_current;
	sv		= _sv;

	select_r = sf::RectangleShape(sf::Vector2f(0,0));
	select_r.setFillColor(sf::Color(0,0,0,0));
	select_r.setOutlineColor(sf::Color(255,255,255));
	select_r.setOutlineThickness(-1.0);

	select_click	= false;
	selected_click	= false;
	select_p_mouse	= sf::Vector2f(0,0);

	right_click	= false;
}
