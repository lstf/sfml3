#ifndef _IOUTILS_H
#define _IOUTILS_H

#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>

using namespace std;

void write_string(string s, ofstream &out);
void read_string(string &s, ifstream &inp);

void write_int(int i, ofstream &out);
void read_int(int &i, ifstream &inp);

void write_vec2(sf::Vector2f v, ofstream &out);
void write_vec2(sf::Vector2i v, ofstream &out);
void read_vec2(sf::Vector2f &v, ifstream &inp);
void read_vec2(sf::Vector2i &v, ifstream &inp);

void write_rect(sf::FloatRect r, ofstream &out);
void write_rect(sf::IntRect r, ofstream &out);
void read_rect(sf::FloatRect &r, ifstream &inp);
void read_rect(sf::IntRect &r, ifstream &inp);

#endif
