#include "ioutils.h"

void write_string(string s, ofstream &out) {
	int length = s.length();

	out.write((char*)&length, sizeof(length));
	for (int i = 0; i < length; i++) {
		out.write((char*)&s[i], sizeof(s[i]));
	}
}
void read_string(string &s, ifstream &inp) {
	int length;
	char c;

	s = "";

	inp.read((char*)&length, sizeof(length));
	for (int i = 0; i < length; i++) {
		inp.read((char*)&c, sizeof(c));
		s += c;
	}
}

void write_int(int i, ofstream &out) {
	out.write((char*)&i, sizeof(i));
}
void read_int(int &i, ifstream &inp) {
	inp.read((char*)&i, sizeof(i));
}

void write_vec2(sf::Vector2f v, ofstream &out) {
	out.write((char*)&v.x, sizeof(v.x));
	out.write((char*)&v.y, sizeof(v.y));
}
void write_vec2(sf::Vector2i v, ofstream &out) {
	out.write((char*)&v.x, sizeof(v.x));
	out.write((char*)&v.y, sizeof(v.y));
}
void read_vec2(sf::Vector2f &v, ifstream &inp) {
	inp.read((char*)&v.x, sizeof(v.x));
	inp.read((char*)&v.y, sizeof(v.y));
}
void read_vec2(sf::Vector2i &v, ifstream &inp) {
	inp.read((char*)&v.x, sizeof(v.x));
	inp.read((char*)&v.y, sizeof(v.y));
}

void write_rect(sf::FloatRect r, ofstream &out) {
	out.write((char*)&r.left, sizeof(r.left));
	out.write((char*)&r.top, sizeof(r.top));
	out.write((char*)&r.width, sizeof(r.width));
	out.write((char*)&r.height, sizeof(r.height));
}
void write_rect(sf::IntRect r, ofstream &out) {
	out.write((char*)&r.left, sizeof(r.left));
	out.write((char*)&r.top, sizeof(r.top));
	out.write((char*)&r.width, sizeof(r.width));
	out.write((char*)&r.height, sizeof(r.height));
}
void read_rect(sf::FloatRect &r, ifstream &inp) {
	inp.read((char*)&r.left, sizeof(r.left));
	inp.read((char*)&r.top, sizeof(r.top));
	inp.read((char*)&r.width, sizeof(r.width));
	inp.read((char*)&r.height, sizeof(r.height));
}
void read_rect(sf::IntRect &r, ifstream &inp) {
	inp.read((char*)&r.left, sizeof(r.left));
	inp.read((char*)&r.top, sizeof(r.top));
	inp.read((char*)&r.width, sizeof(r.width));
	inp.read((char*)&r.height, sizeof(r.height));
}

void write_state(const map<string, int> &state, ofstream &out) {
	int length = state.size();

	write_int(length, out);
	for (auto it = state.begin(); it != state.end(); ++it) {
		write_string(it->first, out);
		write_int(it->second, out);
	}
}
void read_state(map<string, int> &state, ifstream &inp) {
	int length;
	string event;
	int val;

	read_int(length, inp);
	for (int i = 0; i < length; i++) {
		read_string(event, inp);
		read_int(val, inp);
		state[event] = val;
	}
}
