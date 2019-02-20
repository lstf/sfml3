#include "decopanel.h"

void Decopanel::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();
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
}

void Decopanel::handle_input(sf::Event &event) {
	sf::Vector2i m_pos = sf::Mouse::getPosition(*window);
	for (unsigned int i = 0; i < button_count; i++) {
		BState bs = buttons[i].btn->handle_input(event, m_pos);	
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
		}
	}
	float scroll_val = scroll->handle_input(event);
	for (unsigned int i = 0; i < button_count; i++) {
		sf::Vector2f b_pos = buttons[i].btn->getPosition();
		b_pos.y = 
		(i/2)*DECO_B_SIZE/4+DECO_P_SIZE+DECO_BASE_H+int(scroll_val*scroll_max);
		b_pos.x = (i%2)*DECO_B_SIZE;
		buttons[i].btn->setPosition(b_pos);
	}
}

Decopanel::Decopanel(sf::RenderWindow* _w, Map* _map) {
	map = _map;

	window = _w;

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
				//0,
				//i*DECO_B_SIZE/4+DECO_BASE_H+DECO_P_SIZE,
				//DECO_B_SIZE,
				//DECO_B_SIZE/4
			)
		);
		buttons[i].file = "./ats/mps/tx/" + files[i];
	}

	sf::IntRect bt = buttons[0].btn->bounds;
	sf::IntRect bb = buttons[button_count - 1].btn->bounds;
	
	scrollview = sf::View(
		sf::Vector2f(DECO_P_SIZE/2,(480-DECO_P_SIZE)/2+DECO_P_SIZE+DECO_BASE_H),
		sf::Vector2f(DECO_P_SIZE, 480 - DECO_P_SIZE)
	);
	scrollview.setViewport(sf::FloatRect(
		0,
		(DECO_P_SIZE+DECO_BASE_H)/480.0,
		DECO_P_SIZE/960.0,
		(480 - DECO_P_SIZE)/480.0
	));
	int scroll_content_height = bb.top + bb.height - bt.top;
	scroll_max = 480 - DECO_P_SIZE - scroll_content_height;
	scroll = new Scrollbar(_w,
		480 - DECO_P_SIZE,
		scroll_content_height,
		DECO_P_SIZE,
		DECO_BASE_H + DECO_P_SIZE,
		16,
		480 - DECO_P_SIZE
	);
}
