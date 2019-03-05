#include "keylock.h"

void KeyLock::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	(void)w;
	(void)states;
}

void KeyLock::set_w(int w) {
	r.width = w;
}

void KeyLock::set_h(int h) {
	r.height = h;
}


sf::FloatRect KeyLock::bounds() {
	return r;
}



DBox* KeyLock::interact(Player &player, map<string, int> &lstate,
map<string, int> &gstate) {
	(void)lstate;
	(void)gstate;

	bool has_key = false;
	vector<ItemQuantity> keys = player.inv.keys;
	for (auto it = keys.begin(); it != keys.end(); ++it) {
		if (it->item->name == key_name) {
			has_key = true;
		}
	}

	DTree* d = new DTree;
	if (has_key) {
		d->root = newDnode(d, "[Unlock door with " + key_name + "?]");
		d->root->options[0]->text = "yes";
		DNode* unlock = newDnode(d, "[Unlocked door]");
		unlock->delete_ent = true;
		unlock->delete_ent_ptr = this;
		d->root->options[0]->target = unlock;
		DNode* not_unlock = newDnode(d, "[Door is locked]");
		newDoption(d, d->root, "no", not_unlock);
	} else {
		d->root = newDnode(d, "[Door is locked]");
	}
	DBox* ret = new DBox(d, &player, &lstate, &gstate);
	return ret;
}


bool KeyLock::update(Player &player, map<string, int> &lstate,
map<string, int> &gstate) {
	(void)player;
	(void)lstate;
	(void)gstate;
	return false;
}


void KeyLock::set_pos(sf::Vector2f pos) {
	r.left = pos.x;
	r.top = pos.y;
}


sf::Vector2f KeyLock::size() {
	return sf::Vector2f(r.width, r.height);
}


KeyLock::KeyLock() {
	name = "keylock";
}

void KeyLockUI::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(*w_b, states);
	w.draw(*h_b, states);
	w.draw(*key_name_b, states);
	if (typing) {
		w.draw(*tb, states);
	}
}

bool KeyLockUI::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (typing) {
		if (field == 0) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				active_ent->set_w(stoi(*sp));
				delete sp;
			}
		} else if (field == 1) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				active_ent->set_h(stoi(*sp));
				delete sp;
			}
		} else if (field == 2) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				active_ent->key_name = *sp;
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

	BState bs = w_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 0;
			string en = to_string((int)active_ent->bounds().width);
			typing = true;
			tb = new Textbox(
				irfr(w_b->bounds),
				true, en
			);
			return true;
		}
	} 
	bs = h_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 1;
			string en = to_string((int)active_ent->bounds().height);
			typing = true;
			tb = new Textbox(
				irfr(h_b->bounds),
				true, en
			);
			return true;
		}
	}
	bs = key_name_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 2;
			string en = active_ent->key_name;
			typing = true;
			tb = new Textbox(
				irfr(key_name_b->bounds),
				false, en
			);
			return true;
		}
	}

	return false;
}

void KeyLockUI::reset(KeyLock* ent) {
	if (typing) {
		typing = false;
		delete tb;
		tb = NULL;
	}
	active_ent = ent;
}

KeyLockUI::KeyLockUI(int x, int y, int w, int h) {
	w_b = new Button(
		KEYENT_BG, KEYENT_FG, "width", sf::FloatRect(
			x,
			y,
			w,
			h	
		)
	);
	h_b = new Button(
		KEYENT_BG, KEYENT_FG, "height", sf::FloatRect(
			x + w,
			y,
			w,
			h
		)
	);
	key_name_b = new Button(
		KEYENT_BG, KEYENT_FG, "key name", sf::FloatRect(
			x,
			y + h,
			w,
			h
		)
	);
	typing = false;
	tb = NULL;
	active_ent = NULL;
}

