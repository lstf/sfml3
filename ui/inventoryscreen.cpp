#include "inventoryscreen.h"

void InventoryScreen::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::View temp = w.getView();

	w.setView(Window::default_view);
	w.draw(bg, states);
	w.draw(item_bg, states);

	for (int i = 0; i < INVENTORYCATEGORIES; i++) {
		w.draw(categories[i].body, states);
		w.draw(categories[i].text, states);
	}

	if (category_selection == INV_KEYS) {
		for (auto it = keys.begin(); it != keys.end(); ++it) {
			w.draw(it->body, states);
			w.draw(it->text, states);
		}
	}

	w.setView(temp);
}

void InventoryScreen::handle_input(sf::Event &event) {
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Right) {
			if (category_selection < INVENTORYCATEGORIES - 1) {
				category_selection++;
				select_category(category_selection);
			}
		} else if (event.key.code == sf::Keyboard::Left) {
			if (category_selection > 0) {
				category_selection--;
				select_category(category_selection);
			}
		}
	}
}

void InventoryScreen::reload_inventory() {
	vector<ItemQuantity>* list = &player->inv.keys;
	int len = list->size();
	if (list->size() != keys.size()) {
		keys.clear();
		MenuKeyButton mkb;
		ItemQuantity* iq;
		mkb.text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
		mkb.text.setCharacterSize(16);
		mkb.text.setFillColor(INVS_FG);
		mkb.body.setSize(sf::Vector2f(INVENTORYCATEGORIES * 64 - 2, 20));
		mkb.body.setFillColor(INVS_BG);
		mkb.body.setOutlineColor(INVS_BG);
		mkb.body.setOutlineThickness(-1.0);
		for (int i = 0; i < len; i++) {
			iq = &((*list)[i]);
			mkb.body.setPosition(1, 24 + i * 20);
			mkb.text.setPosition(4, 24 + i * 20);
			mkb.text.setString(to_string(iq->count) +"    "+ iq->item->name);
			keys.push_back(mkb);
		}
	}
}

void InventoryScreen::select_category(int i) {
	for (int j = 0; j < INVENTORYCATEGORIES; j++) {
		categories[j].body.setFillColor(INVS_BG);
		categories[j].text.setFillColor(INVS_FG);
	}
	categories[i].body.setFillColor(INVS_FG);
	categories[i].text.setFillColor(INVS_BG);
}

InventoryScreen::InventoryScreen(Player* _player) {
	player = _player;

	bg.setSize(sf::Vector2f(960, 480));
	bg.setPosition(0, 0);
	bg.setFillColor(INVS_BG);
	item_bg.setSize(sf::Vector2f(INVENTORYCATEGORIES * 64, 480 - 24));
	item_bg.setPosition(0, 24);
	item_bg.setFillColor(INVS_BG);
	item_bg.setOutlineColor(INVS_FG);
	item_bg.setOutlineThickness(-1.0);

	string category_str[] = {"Consumable", "Valuble", "Weapon", "Ammo", "Armor",
	"Keys", "Quest", "Mods"};
	for (int i = 0; i < INVENTORYCATEGORIES; i++) {
		categories[i].body.setSize(sf::Vector2f(64, 24));
		categories[i].body.setPosition(sf::Vector2f(i*64, 0));
		categories[i].body.setFillColor(INVS_BG);
		categories[i].body.setOutlineColor(INVS_FG);
		categories[i].body.setOutlineThickness(-1.0);
		categories[i].text.setFont(*txmap::get_font("./ats/fonts/thintel.ttf"));
		categories[i].text.setCharacterSize(18);
		categories[i].text.setFillColor(INVS_FG);
		categories[i].text.setString(category_str[i]);
		sf::Vector2f tp = centerIn(
			categories[i].text.getGlobalBounds(),
			categories[i].body.getGlobalBounds()
		);
		categories[i].text.setPosition((int)tp.x, -2);
	}
	category_selection = 0;
	select_category(category_selection);
}
