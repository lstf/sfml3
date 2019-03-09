#include "door.h"

Door::Door() {
	name = "door";

	anim.w = 32;
	anim.h = 48;
	anim.set_fps(6);
	anim.set_frame_count(3);
	anim.set_png(DOOR_PNG_FILE);

	sp.setTexture(anim.tx);
	frame = 0;
}

void Door::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(sp, states);
}

MapTrans* Door::interact() {
	MapTrans* ret = new MapTrans;

	ret->p = (Portal*)this;
	ret->map_name = target;
	ret->position = target_pos;

	return ret;
}

bool Door::update() {
	if (anim.advance()) {
		frame++;
		if (frame < DOOR_PNG_FRAMES) {
			sp.setTexture(anim.tx);
			return false;
		} else {
			return true;
		}
	}

	return false;
}

sf::FloatRect Door::bounds() {
	return sp.getGlobalBounds();
}

sf::Vector2f Door::size() {
	sf::FloatRect r = sp.getGlobalBounds();

	return sf::Vector2f(r.width, r.height);
}

void Door::set_position(sf::Vector2f pos) {
	sp.setPosition(pos);
}

sf::Vector2f Door::get_position() {
	return sp.getPosition();
}

vector<string>* Door::ats_names() {
	vector<string>* ret = new vector<string>;

	ret->push_back(string(DOOR_PNG_FILE));

	return ret;
}

void Door::write(ofstream &out) {
	write_string(name, out);

	write_string(target, out);
	write_vec2(target_pos, out);
	write_vec2(sp.getPosition(), out);
}

void Door::read(ifstream &inp) {
	sf::Vector2f pos;

	read_string(target, inp);
	read_vec2(target_pos, inp);
	read_vec2(pos, inp);

	set_position(pos);
}

Door::~Door() {
	
}
