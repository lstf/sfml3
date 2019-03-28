#include "itement.h"

////////////////////////////////////////////////
//
// Entity
//
////////////////////////////////////////////////

ItemEnt::ItemEnt() {
	log_dbg("constructing item ent");
	name = "item";
	got = false;
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
	log_dbg("player interacted");
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

ItemEnt::~ItemEnt() {
	log_dbg("destructing item ent");
}

////////////////////////////////////////////////
//
// Spawner
//
////////////////////////////////////////////////

ItemEntSpawner::ItemEntSpawner() {
	log_dbg("constructing item ent spawner");
	name = "item";
	lval = 0;
}

sf::FloatRect ItemEntSpawner::bounds() {
	if (sp_name == "") {
		return sf::FloatRect(
			sp.getPosition().x,
			sp.getPosition().y,
			32,
			32
		);
	}
	return sp.getGlobalBounds();
}

void ItemEntSpawner::set_pos(sf::Vector2f pos) {
	sp.setPosition(pos);
}

void ItemEntSpawner::get_sp() {
	sp_name = item_ent_sp(item_cat, item_name);
	if (sp_name != "") {
		sp.setTexture(*txmap::get_tx(sp_name), true);
	}
}

void ItemEntSpawner::write(ofstream &out) {
	log_dbg("writing item ent spawner");
	write_string(name, out);

	write_int(lval, out);
	write_string(levent, out);
	write_string(item_name, out);
	write_string(item_cat, out);
	write_vec2(sp.getPosition(), out);
}

void ItemEntSpawner::read(ifstream &inp) {
	log_dbg("reading item ent spawner");
	sf::Vector2f pos;

	read_int(lval, inp);
	read_string(levent, inp);
	read_string(item_name, inp);
	read_string(item_cat, inp);
	read_vec2(pos, inp);

	sp.setPosition(pos);
}

ItemEntSpawner::~ItemEntSpawner() {
	log_dbg("destructing item ent spawner");
}

EntitySpawner* read_itement_spawner(ifstream &inp) {
	ItemEntSpawner* spawn = new ItemEntSpawner();
	spawn->read(inp);
	spawn->get_sp();
	
	return (EntitySpawner*)spawn;
}

void new_itement_ent(ItemEntSpawner* spawn){
	if (spawn->levent != "" &&
	World::lstate->find(spawn->levent) != World::lstate->end() &&
	World::lstate->at(spawn->levent) != spawn->lval) {
		return;
	}
	ItemEnt* itement = new ItemEnt();
	itement->levent = spawn->levent;
	itement->lval = spawn->lval;
	itement->set_item(spawn->item_cat, spawn->item_name);
	itement->set_pos(spawn->sp.getPosition());
}

////////////////////////////////////////////////
//
// UI
//
////////////////////////////////////////////////

void ItemEntUI::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(*category_b, states);
	w.draw(*name_b, states);
	w.draw(*levent_b, states);
	w.draw(*get_sp_b, states);
	if (typing) {
		w.draw(*tb, states);
	}
}

ItemEntUI::ItemEntUI(int x, int y, int w, int h) {
	log_dbg("constructing item ent ui");
	category_b = new Button(
		"category",
		sf::FloatRect(
			x,
			y,
			w,
			h	
		)
	);
	name_b = new Button(
		"name",
		sf::FloatRect(
			x + w,
			y,
			w,
			h	
		)
	);
	levent_b = new Button(
		"levent",
		sf::FloatRect(
			x,
			y + h,
			w,
			h
		)
	);
	get_sp_b = new Button(
		"get sp",
		sf::FloatRect(
			x + w,
			y + h,
			w,
			h
		)
	);
	typing = false;
	tb = NULL;
	spawn = NULL;
}

bool ItemEntUI::handle_input(sf::Event &event, sf::Vector2i m_pos) {
	if (typing) {
		if (field == 0) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				spawn->item_cat = *sp;
				log_dbg("set item cat " << spawn->item_cat);
				delete sp;
			}
		} else if (field == 1) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				spawn->item_name = *sp;
				log_dbg("set item cat " << spawn->item_name);
				delete sp;
			}
		} else if (field == 2) {
			string* sp = tb->handle_input(event);
			if (sp) {
				typing = false;
				spawn->levent = *sp;
				log_dbg("set item cat " << spawn->levent);
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
			string ic = spawn->item_cat;
			tb = new Textbox(
				irfr(category_b->bounds),
				false, ic
			);
			return true;
		}
	} 
	bs = name_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 1;
			typing = true;
			string in = spawn->item_name;
			tb = new Textbox(
				irfr(name_b->bounds),
				false, in
			);
			return true;
		}
	} 
	bs = levent_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			field = 2;
			string le = spawn->levent;
			typing = true;
			tb = new Textbox(
				irfr(levent_b->bounds),
				false, le
			);
			return true;
		}
	}
	bs = get_sp_b->handle_input(event, m_pos);
	if (bs) {
		if (bs == BCLICK) {
			spawn->get_sp();
			return true;
		}
	}

	return false;
}

void ItemEntUI::reset(ItemEntSpawner* _spawn) {
	log_dbg("resetting");
	if (typing) {
		typing = false;
		delete tb;
		tb = NULL;
	}
	spawn = _spawn;
}

ItemEntUI::~ItemEntUI() {
	log_dbg("destructing item ent ui");
}
