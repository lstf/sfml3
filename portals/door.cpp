#include "door.h"

////////////////////////////////////////////////
//
// Portal 
//
////////////////////////////////////////////////

Door::Door() {
	log_dbg("constructing door");

	name = "door";

	anim.w = DOOR_W;
	anim.h = DOOR_H;
	anim.set_fps(DOOR_FPS);
	anim.set_frame_count(DOOR_FRAMES);
	anim.set_png(DOOR_PNG);
	frame = 0;

	sp.setTexture(anim.tx);
}

MapTrans* Door::interact() {
	log_dbg("player interacted");

	MapTrans* ret = new MapTrans;

	ret->p = (Portal*)this;
	ret->map_name = target;
	ret->position = target_pos;

	return ret;
}

bool Door::update() {
	if (anim.advance()) {
		frame++;
		if (frame < DOOR_FRAMES) {
			sp.setTexture(anim.tx);
			return false;
		} else {
			frame = 0;
			anim.restart();
			sp.setTexture(anim.tx);
			return true;
		}
	}

	return false;
}

sf::FloatRect Door::bounds() {
	return sp.getGlobalBounds();
}

void Door::set_position(sf::Vector2f pos) {
	sp.setPosition(pos);
}

void Door::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(sp, states);
}

Door::~Door() {
	log_dbg("constructing door spawner");
}

////////////////////////////////////////////////
//
// Spawner
//
////////////////////////////////////////////////

DoorSpawner::DoorSpawner() {
	log_dbg("constructing door spawner");
	Png png = txmap::get_png(DOOR_PNG);
	tx.loadFromMemory(png.mem, png.length, sf::IntRect(
		0,
		0,
		DOOR_W,
		DOOR_H
	));
	name = "door";
	sp.setTexture(tx);
	sp_name = DOOR_PNG;
}

#ifdef EDITOR_BUILD
sf::FloatRect DoorSpawner::bounds() {
	return sp.getGlobalBounds();
}

void DoorSpawner::set_pos(sf::Vector2f pos) {
	sp.setPosition(pos);
}

void DoorSpawner::write(ofstream &out) {
	log_dbg("writing door spawner");

	write_string(name, out);
	write_string(sp_name, out);
	write_vec2(sp.getPosition(), out);

	write_string(target, out);
	write_vec2(target_pos, out);
}
#endif

void DoorSpawner::read(ifstream &inp) {
	log_dbg("reading door spawner");

	sf::Vector2f pos;

	read_string(sp_name, inp);
	read_vec2(pos, inp);
	sp.setPosition(pos);

	read_string(target, inp);
	read_vec2(target_pos, inp);
}

DoorSpawner::~DoorSpawner() {
	log_dbg("destructing door spawner");
}

PortalSpawner* read_door_spawner(ifstream &inp) {
	DoorSpawner* spawn = new DoorSpawner();
	spawn->read(inp);
	return (PortalSpawner*)spawn;
}

void new_door(DoorSpawner* spawn) {
	Door* door = new Door();
	door->target = spawn->target;
	door->target_pos = spawn->target_pos;
	door->sp.setPosition(spawn->sp.getPosition());
}
