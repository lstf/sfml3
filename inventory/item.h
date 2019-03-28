#ifndef _ITEM_H
#define _ITEM_H

#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>

using namespace std;

class Item { 
private:
public:
	string category;
	string name;
	string description;
	sf::Sprite sp;
	string sp_name;
};

#endif
