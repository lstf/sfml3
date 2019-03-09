#include "animation.h"

void Animation::refresh_tx() {
	cout << "refreshing tx" << endl;
	tx.loadFromMemory(png.mem, png.length, sf::IntRect(
		x * w,
		y * h,
		w,
		h
	));
}

Animation::Animation() {
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
		cout << "[ANIM] request to set fps for out of range animation" << endl;
	}
}

void Animation::set_frame_count(int n, int a) {
	if (a < MAX_ANIMATIONS && a >= 0) {
		frame_count[a] = n;
	} else {
		cout << 
		"[ANIM] request to set frame count for out of range animation" << endl;
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
		cout << "[ANIM] request to set out of range animation" << endl;
	}
}

void Animation::restart() {
	x = 0;
	refresh_tx();
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
	
}
