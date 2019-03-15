#ifndef _KEYLOCK_H
#define _KEYLOCK_H

#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>

#include "entity.h"
#include "../world.h"
#include "../inventory/inventory.h"
#include "../inventory/keyitem.h"
#include "../ui/button.h"
#include "../ui/textbox.h"
#include "../utils/ioutils.h"

using namespace std;

class KeyLock : public Entity {
private:
	sf::FloatRect r;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	string key_name;

	string levent;
	int lval;

	void set_w(int w);

	void set_h(int h);

	virtual sf::FloatRect bounds();

	virtual DBox* interact(Player &player);

	virtual bool update(Player &player);

	virtual void set_pos(sf::Vector2f pos);

	virtual sf::Vector2f size();

	virtual void write(ofstream &out);
	virtual void read(ifstream &inp);

	KeyLock();
};

void read_keylock_ent(ifstream &inp);

#define KEYLOCK_BG sf::Color(127,127,127)
#define KEYLOCK_FG sf::Color(0,0,0)

class KeyLockUI : public sf::Drawable {
private:
	KeyLock* active_ent;
	Button* w_b;
	Button* h_b;
	Button* key_name_b;
	Button* levent_b;
	Textbox* tb;
	
	bool typing;
	int field;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	bool handle_input(sf::Event &event, sf::Vector2i m_pos);

	void reset(KeyLock* ent);

	KeyLockUI(int x, int y, int w, int h);
};

#endif
