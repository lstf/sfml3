#include "keylock.h"

////////////////////////////////////////////////
//
// Entity
//
////////////////////////////////////////////////

void KeyLock::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	(void)w;
	(void)states;
}

KeyLock::KeyLock() {
	log_dbg("constructing key lock");
	name = "keylock";
}

sf::FloatRect KeyLock::bounds() {
	return r;
}

DBox* KeyLock::interact(Player &player) {
	log_dbg("player interacted");
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
		if (levent != "") {
			unlock->levent = new DEvent;
			unlock->levent->key = levent;
			unlock->levent->val = 1;
		}
		unlock->delete_ent = true;
		unlock->delete_ent_ptr = this;
		d->root->options[0]->target = unlock;
		DNode* not_unlock = newDnode(d, "[Door is locked]");
		newDoption(d, d->root, "no", not_unlock);
	} else {
		d->root = newDnode(d, "[Door is locked]");
	}
	DBox* ret = new DBox(d, &player);
	return ret;
}

bool KeyLock::update(Player &player) {
	(void)player;
	return false;
}

void KeyLock::set_pos(sf::Vector2f pos) {
	r.left = pos.x;
	r.top = pos.y;
}

sf::Vector2f KeyLock::size() {
	return sf::Vector2f(r.width, r.height);
}

KeyLock::~KeyLock() {
	log_dbg("destructing key lock");
}

////////////////////////////////////////////////
//
// Spawner
//
////////////////////////////////////////////////

KeyLockSpawner::KeyLockSpawner() {
	log_dbg("constructing key lock spawner");
	name = "keylock";
	lval = 0;
}

sf::FloatRect KeyLockSpawner::bounds() {
	if (r.width == 0 || r.height == 0) {
		return sf::FloatRect(
			r.left,
			r.top,
			32,
			32
		);
	}
	return r;
}

void KeyLockSpawner::set_pos(sf::Vector2f pos) {
	r.left = pos.x;
	r.top = pos.y;
}

void KeyLockSpawner::write(ofstream &out) {
	log_dbg("writing key lock spawner");
	write_string(name, out);

	write_int(lval, out);
	write_string(levent, out);
	write_string(key_name, out);
	write_rect(r, out);
}

void KeyLockSpawner::read(ifstream &inp) {
	log_dbg("reading key lock spawner");
	read_int(lval, inp);
	read_string(levent, inp);
	read_string(key_name, inp);
	read_rect(r, inp);
}

EntitySpawner* read_keylock_spawner(ifstream &inp) {
	KeyLockSpawner* spawn = new KeyLockSpawner();
	spawn->read(inp);

	return (EntitySpawner*)spawn;
}

void new_keylock_ent(KeyLockSpawner* spawn) {
	if (spawn->levent != "" &&
	World::lstate->find(spawn->levent) != World::lstate->end() &&
	World::lstate->at(spawn->levent) != spawn->lval) {
		return;
	}
	KeyLock* keylock = new KeyLock();
	keylock->r = spawn->r;
	keylock->levent = spawn->levent;
	keylock->lval = spawn->lval;
	keylock->key_name = spawn->key_name;
}

KeyLockSpawner::~KeyLockSpawner() {
	log_dbg("destructing key lock spawner");
}

////////////////////////////////////////////////
//
// UI
//
////////////////////////////////////////////////

KeyLockUI::KeyLockUI(int x, int y, int w, int h) {
	log_dbg("constructing key lock ui");

	w_b = new Button("width", sf::FloatRect(x, y, w, h));
	h_b = new Button("height", sf::FloatRect(x + w, y, w, h));
	key_name_b = new Button("key name", sf::FloatRect(x, y + h, w, h));
	levent_b = new Button("levent", sf::FloatRect(x + w, y + h, w, h));

	typing = false;
	tb = NULL;
	spawn = NULL;
}

void KeyLockUI::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(*w_b, states);
	w.draw(*h_b, states);
	w.draw(*key_name_b, states);
	w.draw(*levent_b, states);
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
				spawn->r.width = stoi(*sp);
				log_dbg("set width " << spawn->r.width);
				delete sp;
			}
		} else if (field == 1) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				spawn->r.height  = stoi(*sp);
				log_dbg("set height " << spawn->r.height);
				delete sp;
			}
		} else if (field == 2) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				spawn->key_name = *sp;
				log_dbg("set key name " << spawn->key_name);
				delete sp;
			}
		} else if (field == 3) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				spawn->levent = *sp;
				log_dbg("set levent " << spawn->levent);
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
			string sw = to_string((int)spawn->r.width);
			typing = true;
			tb = new Textbox(
				irfr(w_b->bounds),
				true, sw 
			);
			return true;
		}
	} 
	bs = h_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 1;
			string sh = to_string((int)spawn->r.height);
			typing = true;
			tb = new Textbox(
				irfr(h_b->bounds),
				true, sh
			);
			return true;
		}
	}
	bs = key_name_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 2;
			string sk = spawn->key_name;
			typing = true;
			tb = new Textbox(
				irfr(key_name_b->bounds),
				false, sk
			);
			return true;
		}
	}
	bs = levent_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 3;
			string sl = spawn->levent;
			typing = true;
			tb = new Textbox(
				irfr(levent_b->bounds),
				false, sl
			);
			return true;
		}
	}

	return false;
}

void KeyLockUI::reset(KeyLockSpawner* _spawn) {
	log_dbg("resetting");
	if (typing) {
		typing = false;
		delete tb;
		tb = NULL;
	}
	spawn = _spawn;
}

KeyLockUI::~KeyLockUI() {
	log_dbg("destructing key lock ui");
}
