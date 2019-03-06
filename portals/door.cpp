#include "door.h"

Door::Door() {
	name = "door";

	Png png = txmap::get_png(DOOR_DIR + string("D.PNG"));

	sp_sheet.png = png.mem;
	sp_sheet.pngSize = png.length;

	sp_sheet.tx.loadFromMemory(sp_sheet.png, sp_sheet.pngSize, sf::IntRect(0,0,32,48));
	sp.setTexture(sp_sheet.tx);

	sp_sheet.clock.restart();
	sp_sheet.time = 0;
	sp_sheet.x = 0;
	sp_sheet.y = 0;
	sp_sheet.frameCount = 3;
	sp_sheet.fps = 6;
}

void Door::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(sp, states);
}

bool Door::advanceAnimation() {
	sp_sheet.time += sp_sheet.clock.getElapsedTime().asSeconds();
	if (sp_sheet.time > 1/sp_sheet.fps) {
		sp_sheet.time = 0;
		sp_sheet.clock.restart();
		sp_sheet.x++;
		if (sp_sheet.x >= sp_sheet.frameCount) {
			sp_sheet.x = 0;
			return true;
		}
		sp_sheet.tx.loadFromMemory(sp_sheet.png, sp_sheet.pngSize, sf::IntRect(sp_sheet.x*32,0,32,48));
		sp.setTexture(sp_sheet.tx);
	}
	return false;
}

sf::FloatRect Door::bounds() {
	return sp.getGlobalBounds();
}

MapTrans* Door::interact() {
	MapTrans* ret = new MapTrans;
	ret->p = (Portal*)this;
	cout << target << endl;
	ret->map_name = target;
	cout << target_pos.x << " " << target_pos.y << endl;
	ret->position = target_pos;
	return ret;
}
sf::Vector2f Door::size() {
	sf::FloatRect r = sp.getGlobalBounds();
	return sf::Vector2f(r.width, r.height);
}

void Door::setPosition(sf::Vector2f pos) {
	sp.setPosition(pos);
}

sf::Vector2f Door::getPosition() {
	return sp.getPosition();
}

void Door::write(ofstream &out) {
	cout << "writing name" << endl;
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

	setPosition(pos);
}

bool Door::update() {
	return advanceAnimation();	
}

