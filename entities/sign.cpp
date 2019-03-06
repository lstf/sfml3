#include "sign.h"

sf::FloatRect Sign::bounds() {
	return sp.getGlobalBounds();
}

void Sign::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(sp, states);
}

DBox* Sign::interact(Player &player, map<string, int> &lstate, map<string, int> &gstate) {
	DTree* dtree = new DTree;
	dtree->list.push_back(new DNode);
	dtree->root = dtree->list.back();
	dtree->root->text = "Testing!";
	dtree->olist.push_back(new DOption);
	dtree->root->options.push_back(dtree->olist.back());
	dtree->root->options.back()->text = "...";
	dtree->root->options.back()->target = NULL;
	DBox* ret = new DBox(dtree, &player, &lstate, &gstate);
	return ret;
}
bool Sign::update(Player &player, map<string, int> &lstate, map<string, int> &gstate) {
	(void)player;
	(void)lstate;
	(void)gstate;
	return false;
}

sf::Vector2f Sign::size() {
	sf::FloatRect r = sp.getGlobalBounds();
	return sf::Vector2f(r.width, r.height);
}

void Sign::set_pos(sf::Vector2f pos) {
	sp.setPosition(pos);
}

void Sign::write(ofstream &out) {
	write_string(name, out);

	write_vec2(sp.getPosition(), out);
}
void Sign::read(ifstream &inp) {
	sf::Vector2f pos;
	
	read_vec2(pos, inp);
	
	set_pos(pos);
}

Sign::Sign() {
	name = "sign";
	sp.setTexture(*txmap::get_tx("ats/signs/sign0.png"));
}
