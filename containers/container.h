#ifndef _CONTAINER_H
#define _CONTAINER_H

#include <map>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../inventory/inventory.h"
#include "../inventory/itemutils.h"

struct ContainerQuantity {
	string category;
	string name;
	int count;
};

class Container : public sf::Drawable {
private:
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const = 0;

public:
	//static map<map<sf::Vector2f, vector<ContainerQuantity>>>

	//static vector<Container*> list;

	//static map<pair<string, string> int> init_contents;

	Inventory inv;
};


#endif
