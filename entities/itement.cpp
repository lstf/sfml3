#include "itement.h"

void read_item_ent(ifstream &inp) {
	cout << "[READ ENT] reading item ent" << endl;
	ItemEnt* new_ent = new ItemEnt;
	new_ent->read(inp);
	if (World::lstate->find(new_ent->levent) != World::lstate->end() &&
	World::lstate->at(new_ent->levent) != new_ent->lval) {
		delete new_ent;
	}
}

ItemEnt::ItemEnt() {
	name = "item";
	got = false;
	lval = 0;
}

void ItemEnt::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	if (got) {
		return;
	}
	w.draw(sp, states);
	w.draw(sparkle, states);
}

sf::FloatRect ItemEnt::bounds() {
	if (got) {
		return sf::FloatRect(0, 0, 0, 0);
	}
	return sp.getGlobalBounds();
}

DBox* ItemEnt::interact(Player &player) {
	if (got) {
		return NULL;
	}
	DTree* d = new DTree;
	d->root = newDnode(d, "[Pick up " + item_name + "?]");
	d->root->options[0]->text = "yes";
	DNode* got_item = newDnode(d, "[got " + item_name + "]");
	got_item->delete_ent = true;
	got_item->delete_ent_ptr = this;
	if (levent != "") {
		got_item->levent = new DEvent;
		got_item->levent->key = levent;
		got_item->levent->val = 1;
	}
	got_item->item = new DItem;
	got_item->item->item = new_item(item_cat, item_name);
	got_item->item->count = 1;
	d->root->options[0]->target = got_item;
	DNode* not_got_item = newDnode(d, "[did not pick up " + item_name + "]");
	newDoption(d, d->root, "no", not_got_item);
	DBox* ret = new DBox(d, &player);

	return ret;
}

bool ItemEnt::update(Player &player) {
	(void)player;
	sparkle.update();
	return false;
}

void ItemEnt::set_pos(sf::Vector2f pos) {
	sp.setPosition(pos);
	sparkle.set_position(pos);
}

void ItemEnt::set_item(string cat, string name) {
	item_name = name;
	item_cat = cat;
	sp.setTexture(*txmap::get_tx(item_ent_sp(cat, name)), true);
	sparkle.set_rect(frir(sp.getGlobalBounds()));
}

sf::Vector2f ItemEnt::size() {
	if (got) {
		return sf::Vector2f(0, 0);
	}
	sf::FloatRect spb = sp.getGlobalBounds();
	return sf::Vector2f(spb.width, spb.height);
}

void ItemEnt::write(ofstream &out) {
	write_string(name, out);

	write_int(lval, out);
	write_string(levent, out);
	write_string(item_name, out);
	write_string(item_cat, out);
	write_vec2(sp.getPosition(), out);
}
void ItemEnt::read(ifstream &inp) {
	sf::Vector2f pos;

	read_int(lval, inp);
	read_string(levent, inp);
	read_string(item_name, inp);
	read_string(item_cat, inp);
	read_vec2(pos, inp);

	set_item(item_cat, item_name);
	set_pos(pos);
}

void ItemEntUI::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(*category_b, states);
	w.draw(*name_b, states);
	w.draw(*levent_b, states);
	w.draw(*get_sp_b, states);
	if (typing) {
		w.draw(*tb, states);
	}
}

bool ItemEntUI::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (typing) {
		if (field == 0) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				item_cat = *sp;
				delete sp;
			}
		} else if (field == 1) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				item_name = *sp;
				delete sp;
			}
		} else if (field == 2) {
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

	BState bs = category_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 0;
			typing = true;
			tb = new Textbox(
				irfr(category_b->bounds),
				false, item_cat
			);
			return true;
		}
	} 
	bs = name_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 1;
			typing = true;
			tb = new Textbox(
				irfr(name_b->bounds),
				false, item_name
			);
			return true;
		}
	} 
	bs = levent_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 2;
			string en = active_ent->levent;
			typing = true;
			tb = new Textbox(
				irfr(levent_b->bounds),
				false, en
			);
			return true;
		}
	}
	bs = get_sp_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			if (item_name != "" && item_cat != "") {
				active_ent->set_item(item_cat, item_name);
			}
			return true;
		}
	}

	return false;
}

void ItemEntUI::reset(ItemEnt* ent) {
	if (typing) {
		typing = false;
		delete tb;
		tb = NULL;
	}
	active_ent = ent;
	item_name = ent->item_name;
	item_cat = ent->item_cat;
}

ItemEntUI::ItemEntUI(int x, int y, int w, int h) {
	category_b = new Button(
		KEYENT_BG, KEYENT_FG, "category", sf::FloatRect(
			x,
			y,
			w,
			h	
		)
	);
	name_b = new Button(
		KEYENT_BG, KEYENT_FG, "name", sf::FloatRect(
			x + w,
			y,
			w,
			h	
		)
	);
	levent_b = new Button(
		KEYENT_BG, KEYENT_FG, "levent", sf::FloatRect(
			x,
			y + h,
			w,
			h
		)
	);
	get_sp_b = new Button(
		KEYENT_BG, KEYENT_FG, "get sp", sf::FloatRect(
			x + w,
			y + h,
			w,
			h
		)
	);
	typing = false;
	tb = NULL;
	active_ent = NULL;
}