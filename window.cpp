#include "window.h"

sf::View Window::default_view;
int Window::w;
int Window::h;
sf::Vector2f Window::offset;
float Window::scale;

sf::View letterbox(sf::View view, int w, int h) {
	float window_ratio = w / (float)h;
	float size_x = 1.0;
	float size_y = 1.0;
	float pos_x = 0;
	float pos_y = 0;
	
	if (window_ratio > 2.0) {
		size_x = 2.0 / window_ratio;
		pos_x = (1 - size_x) / 2.0;
	} else {
		size_y = window_ratio / 2.0;
		pos_y = (1 - size_y) / 2.0;
	}

	view.setViewport(sf::FloatRect(pos_x, pos_y, size_x, size_y));

	return view;
}

void Window::init(sf::View view) {
	log_dbg("initializing window 960x480:windowed");
	w = 960;
	h = 480;
	default_view = view;
}

void Window::set_size(int _w, int _h) {
	w = _w;
	h = _h;

	default_view = letterbox(default_view, w, h);

	float window_ratio = w / (float)h;

	offset = sf::Vector2f(0, 0);
	if (window_ratio > 2.0) {
		offset.x = (w - (w * 2.0 / window_ratio)) / 2;
	} else {
		offset.y = (h - (h * window_ratio / 2.0)) / 2;
	}

	scale = w / 960.0;
}
