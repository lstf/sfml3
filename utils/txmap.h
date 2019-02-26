#ifndef _TXMAP_H
#define _TXMAP_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include <SFML/Graphics.hpp>

using namespace std;

struct Png {
	unsigned int length;
	void* mem;
};

//TODO make this a class
namespace txmap
{
	static map<string, sf::Texture*> txs;
	static map<string, sf::Font*> fonts;
	static map<string, Png> pngs;

	sf::Texture* get_tx(const string &name);
	sf::Font* get_font(const string &name);
	Png get_png(const string &name);
}

#endif
