#include "animation.h"

void Animation::refresh_tx() {
	tx.loadFromMemory(png.mem, png.length, sf::IntRect(
		x * w,
		y * h,
		w,
		h
	));
}

Animation::Animation() {
	log_dbg("constructing animation");
	x = 0;
	y = 0;

	clock.restart();
	time = 0;
	
	w = 0;
	h = 0;

	for (int i = 0; i < MAX_ANIMATIONS; i++) {
		frame_time[i] = FLT_MAX;
		frame_count[i] = 0;
	}
}

void Animation::set_fps(float fps, int a) {
	if (a < MAX_ANIMATIONS && a >= 0) {
		frame_time[a] = 1/fps;
	} else {
		log_err("out of range: " << a << " >= " << MAX_ANIMATIONS);
	}
}

void Animation::set_frame_count(int n, int a) {
	if (a < MAX_ANIMATIONS && a >= 0) {
		frame_count[a] = n;
	} else {
		log_err("out of range: " << a << " >= " << MAX_ANIMATIONS);
	}
}

void Animation::set_png(string name) {
	png = txmap::get_png(name);
	refresh_tx();
}

void Animation::set_animation(int a) {
	if (a < MAX_ANIMATIONS && a >= 0) {
		y = a;
		restart();
	} else {
		log_err("out of range: " << a << " >= " << MAX_ANIMATIONS);
	}
}

void Animation::restart() {
	x = 0;
	refresh_tx();
	clock.restart();
	time = 0;
}

bool Animation::advance() {
	time += clock.restart().asSeconds();

	if (time > frame_time[y]) {
		time = 0;

		x = x + 1 >= frame_count[y] ? 0 : x + 1;
		refresh_tx();
		
		return true;
	}

	return false;
}

Animation::~Animation() {
	log_dbg("destructing animation");
}
