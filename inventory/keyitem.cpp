#include "keyitem.h"

KeyItem::KeyItem() {
	category = "key";
}


void KeyItemEnt::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	if (got) {
		return;
	}
	w.draw(sp, states);
}

sf::FloatRect KeyItemEnt::bounds() {
	if (got) {
		return sf::FloatRect(0, 0, 0, 0);
	}
	return sp.getGlobalBounds();
}

DBox* KeyItemEnt::interact(Player &player, map<string, int> &lstate,
map<string, int> &gstate) {
	(void)gstate;
	if (got) {
		return NULL;
	}
	DTree* d = new DTree;
	d->root = newDnode(d, "[Pick up " + key_name + "?]");
	d->root->options[0]->text = "yes";
	DNode* got_key = newDnode(d, "[got " + key_name + "]");
	got_key->levent = new DEvent;
	got_key->levent->key = levent;
	got_key->levent->val = 1;
	got_key->item = new DItem;
	got_key->item->item = (Item*)new KeyItem();
	got_key->item->item->name = key_name;
	got_key->item->item->description = key_desc;
	got_key->item->count = 1;
	d->root->options[0]->target = got_key;
	DNode* not_got_key = newDnode(d, "[did not pick up " + key_name + "]");
	newDoption(d, d->root, "no", not_got_key);
	return new DBox(d, &player, &lstate, &gstate);
}

void KeyItemEnt::update(Player &player, map<string, int> &lstate,
map<string, int> &gstate) {
	(void)player;
	(void)gstate;
	if (got) {
		return;
	}
	if (levent != "") {
		if (lstate[levent] != 0) {
			got = true;
		} 
	}
}

void KeyItemEnt::set_pos(sf::Vector2f pos) {
	sp.setPosition(pos);
}

sf::Vector2f KeyItemEnt::size() {
	if (got) {
		return sf::Vector2f(0, 0);
	}
	sf::FloatRect spb = sp.getGlobalBounds();
	return sf::Vector2f(spb.width, spb.height);
}

KeyItemEnt::KeyItemEnt() {
	name = "key";
	got = false;
	sp.setTexture(*txmap::get_tx("./ats/entities/keyent.png"));
}
