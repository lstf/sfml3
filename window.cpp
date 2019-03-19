#include "window.h"

sf::View Window::default_view;
int Window::w;
int Window::h;

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
	w = 960;
	h = 480;
	default_view = view;
}

void Window::set_size(int _w, int _h) {
	w = _w;
	h = _h;
	default_view = letterbox(default_view, w, h);
}

sf::Vector2f Window::offset() {
	float window_ratio = w / (float)h;

	sf::Vector2f off(0, 0);

	if (window_ratio > 2.0) {
		off.x = (w - (w * 2.0 / window_ratio)) / 2;
	} else {
		off.y = (h - (h * window_ratio / 2.0)) / 2;
	}

	return off;
}

float Window::scale(){
	return w / 960.0;
}
