#ifndef _INVENTORYSCREEN_H
#define _INVENTORYSCREEN_H

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "uiutils.h"
#include "../player.h"
#include "../inventory/inventory.h"
#include "../inventory/keyitem.h"

#define INVS_BG sf::Color::Black
#define INVS_FG sf::Color::White

struct MenuKeyButton {
	sf::RectangleShape body;
	sf::Text text;
};

class InventoryScreen : public sf::Drawable {
private:
	Player* player;

	sf::RectangleShape bg;
	sf::RectangleShape item_bg;

	MenuButton categories[INVENTORYCATEGORIES];
	int category_selection;

	vector<MenuKeyButton> keys;

	void select_category(int i);

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	void handle_input(sf::Event &event);

	void reload_inventory();
	
	InventoryScreen(Player* _player);
};

#endif
