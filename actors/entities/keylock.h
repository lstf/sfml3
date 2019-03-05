#ifndef _KEYLOCK_H
#define _KEYLOCK_H

#include <string>

#include <SFML/Graphics.hpp>

#include "../entity.h"
#include "../../inventory/inventory.h"
#include "../../inventory/keyitem.h"

class KeyLock : public Entity {
private:
	sf::FloatRect r;
	
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	string key_name;

	void set_w(int w);

	void set_h(int h);

	virtual sf::FloatRect bounds();

	virtual DBox* interact(Player &player, map<string, int> &lstate,
	map<string, int> &gstate);

	virtual bool update(Player &player, map<string, int> &lstate,
	map<string, int> &gstate);

	virtual void set_pos(sf::Vector2f pos);

	virtual sf::Vector2f size();

	KeyLock();
};

#define KEYLOCK_BG sf::Color(127,127,127)
#define KEYLOCK_FG sf::Color(0,0,0)

class KeyLockUI : public sf::Drawable {
private:
	KeyLock* active_ent;
	Button* w_b;
	Button* h_b;
	Button* key_name_b;
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