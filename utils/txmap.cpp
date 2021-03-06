#include "txmap.h"

sf::Texture* txmap::get_tx(const string &name) {
	try {
		return txs.at(name);	
	} catch (const out_of_range& oor) {
		sf::Texture* tx = new sf::Texture;
		if (!tx->loadFromFile(name)) {
			cerr << "[TXMAP] tx " << name << " could not be opened" << endl;
			return NULL;
		}
	
		txs.insert(pair<string, sf::Texture*>(name, tx));
		return tx;
	}
}

sf::Font* txmap::get_font(const string &name) {
	try {
		return fonts.at(name);	
	} catch (const out_of_range& oor) {
		sf::Font* font = new sf::Font;
		if (!font->loadFromFile(name)) {
			cerr << "[TXMAP] font " << name << " could not be opened" << endl;
			return NULL;
		}
	
		fonts.insert(pair<string, sf::Font*>(name, font));
		return font;
	}
}

Png txmap::get_png(const string &name) {
	try {
		return pngs.at(name);	
	} catch (const out_of_range& oor) {
		ifstream inp(name, std::ifstream::binary);
		if (!inp.is_open()) {
			cerr << "[TXMAP] png " << name << " could not be opened" << endl;
			Png ret;
			ret.length = 0;
			ret.mem = NULL;
			return ret;
		}

		Png png;

		inp.seekg(0, inp.end);
		png.length = inp.tellg();
		inp.seekg(0, inp.beg);
		png.mem = (void*)new char[png.length];

		inp.read((char*)png.mem, png.length);

		pngs.insert(pair<string, Png>(name, png));
		return png;
	}
}
