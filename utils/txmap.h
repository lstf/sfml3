#ifndef TXMAP_O
#define TXMAP_O

#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include <SFML/Graphics.hpp>

using namespace std;

struct Png
{
	unsigned int length;
	void* mem;
};

namespace txmap
{
	static map<string, sf::Texture*> txs;
	static map<string, Png> pngs;

	sf::Texture* get_tx(const string &name);
	Png get_png(const string &name);
}

#endif
