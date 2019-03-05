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
	got_key->delete_ent = true;
	got_key->delete_ent_ptr = this;
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
	DBox* ret = new DBox(d, &player, &lstate, &gstate);

	return ret;
}

bool KeyItemEnt::update(Player &player, map<string, int> &lstate,
map<string, int> &gstate) {
	(void)player;
	(void)gstate;
	if (levent != "") {
		if (lstate[levent] != 0) {
			return true;
		} 
	}
	return false;
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

void KeyItemEntUI::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(*name_b, states);
	w.draw(*levent_b, states);
	if (typing) {
		w.draw(*tb, states);
	}
}

bool KeyItemEntUI::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (typing) {
		if (field == 0) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				active_ent->key_name = *sp;
				delete sp;
			}
		} else if (field == 1) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				active_ent->levent = *sp;
				delete sp;
			}
		}
		if (event.type == sf::Event::KeyPressed &&
		event.key.code == sf::Keyboard::Escape) {
			typing = false;
		}
		if (typing == false) {
			delete tb;
			tb = NULL;
		}
		return true;
	}

	BState bs = name_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 0;
			string en = active_ent->key_name;
			typing = true;
			tb = new Textbox(
				irfr(name_b->bounds),
				false, en
			);
			return true;
		}
	} 
	bs = levent_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 1;
			string en = active_ent->levent;
			typing = true;
			tb = new Textbox(
				irfr(levent_b->bounds),
				false, en
			);
			return true;
		}
	}

	return false;
}

void KeyItemEntUI::reset(KeyItemEnt* ent) {
	if (typing) {
		typing = false;
		delete tb;
		tb = NULL;
	}
	active_ent = ent;
}

KeyItemEntUI::KeyItemEntUI(int x, int y, int w, int h) {
	name_b = new Button(
		KEYENT_BG, KEYENT_FG, "name", sf::FloatRect(
			x,
			y,
			w,
			h	
		)
	);
	levent_b = new Button(
		KEYENT_BG, KEYENT_FG, "levent", sf::FloatRect(
			x + w,
			y,
			w,
			h
		)
	);
	typing = false;
	tb = NULL;
	active_ent = NULL;
}
