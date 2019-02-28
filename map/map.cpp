#include "map.h"

void Map::deleteDoor(Portal* d) {
	for (auto it = doors.begin(); it != doors.end(); ++it) {
		if (*it == d) {
			doors.erase(it);
			delete d;
			return;
		}
	}
	cout << "[MAP] request for delete on non existing door" << endl;
}

void Map::setPlayer(Player* p)
{
	player = p;
}

void Map::draw(sf::RenderTarget& w, sf::RenderStates states) const
{
	if (deco)
	{
		w.draw(*background, states);
		for (int i = (int)bg.size() - 1; i >= 0; i--)
		{
			w.draw(bg[i]->sp, states);
		}
	}
	if (geom)
	{
		sf::RectangleShape gRect;
		sf::Vector2f gV;

		gRect.setOutlineThickness(-1.0);
		gRect.setOutlineColor(sf::Color::Blue);
		gRect.setFillColor(sf::Color::Transparent);

		for (int i = (int)geometry.size() - 1; i >= 0; i--)
		{
			gV.x = geometry[i].left;
			gV.y = geometry[i].top;
			gRect.setPosition(gV);
			gV.x = geometry[i].width;
			gV.y = geometry[i].height;
			gRect.setSize(gV);

			w.draw(gRect, states);
		}
	}
	for (int i = (int)doors.size()-1; i >= 0; i--)
	{
		w.draw(*doors.at(i));
	}
}

Portal* Map::addDoor(string name) {
	if (name == "door") {
		doors.push_back((Portal*)new Door);
		doors.at(doors.size()-1)->name = "door";
		return doors.at(doors.size()-1);
	} else {
		cout << "[MAP] invalid portal name " << name << endl;
		return NULL;
	}
}

bool Map::addWall(const sf::Vector2f &_pos, const sf::Vector2f &_size)
{
	modified = true;
	geometry.push_back(sf::FloatRect(_pos, _size));
	return true;
}

bool Map::addDeco(std::string name, const sf::Vector2f _pos)
{
	img* im = new img;
	im->sp.setPosition(_pos);
	im->name = name;
	im->sp.setTexture(*txmap::get_tx(name));
	bg.push_back(im);

	modified = true;
	return true;
}

bool Map::loadTexture(std::string img_name)
{
	tx.push_back(new named_tx);

	tx[tx.size()-1]->name = img_name;

	tx[tx.size()-1]->texture = txmap::get_tx(TEX_DIR + img_name);
	
	tx_names.insert(img_name);
	return true;
}



bool Map::saveMap()
{
	std::ofstream map_file((MAP_DIR + name).c_str(), std::ios::out|std::ios::binary);

	if (!map_file.is_open()) return false;

	modified = false;

	writeString	(name, map_file);
	writeVecImg	(bg, map_file);
	writeVecGeo	(geometry, map_file);
	writeDoors	(map_file);

	map_file.close();
	
	return true;
}

void Map::writeVecImg(std::vector<img*> v, std::ofstream &o)
{
	int length = v.size();
	sf::Vector2f pos;

	o.write((char*)&length, sizeof(length));
	for (int i = 0; i < (int)v.size(); i++)
	{
		writeString(v[i]->name, o);
		pos = v[i]->sp.getPosition();
		o.write((char*)&pos, sizeof(pos));
	}
}

void Map::writeRect(sf::FloatRect r, std::ofstream &o)
{
	o.write((char*)&r.left, sizeof(r.left));
	o.write((char*)&r.top, sizeof(r.top));
	o.write((char*)&r.width, sizeof(r.width));
	o.write((char*)&r.height, sizeof(r.height));
}

void Map::writeVecGeo(std::vector<sf::FloatRect> v, std::ofstream &o)
{
	int length = v.size();

	o.write((char*)&length, sizeof(length));
	for (int i = 0; i < length; i++)
	{
		writeRect(v[i], o);
	}
}

void Map::writeString(std::string s, std::ofstream &o)
{
	int length = s.length();

	o.write((char*)&length, sizeof(length));
	for (int i = 0; i < length; i++)
	{
		o.write((char*)&s[i], sizeof(s[i]));
	}
}

void Map::writeDoors(std::ofstream &o) {
	int length = doors.size();
	sf::Vector2f sp_pos;

	o.write((char*)&length, sizeof(length));
	for (int i = 0; i < length; i++) {
		writeString(doors.at(i)->name, o);

		writeString(doors.at(i)->target, o);

		sp_pos = doors.at(i)->target_pos;
		o.write((char*)&sp_pos, sizeof(sp_pos));

		sp_pos = doors.at(i)->getPosition();
		o.write((char*)&sp_pos, sizeof(sp_pos));
	}
}

bool Map::loadMap()
{
	std::ifstream map_file((MAP_DIR + name).c_str(), std::ios::in|std::ios::binary);

	if (!map_file.is_open()) return false;

	modified = false;

	readString	(name, map_file);
	if (!readVecImg(bg, map_file)) return false;
	readVecGeo	(geometry, map_file);
	readDoors	(map_file);

	map_file.close();

	return true;
}

bool Map::readVecImg(std::vector<img*> &v, std::ifstream &inp)
{
	int length;
	sf::Vector2f p;
	sf::Texture* ntx;
	string name;

	v.clear();
	tx.clear();

	inp.read((char*)&length, sizeof(length));
	for (int i = 0; i < length; i++)
	{
		readString(name, inp);
		ntx = txmap::get_tx(name);
		if (!ntx) {
			continue;
		}

		v.push_back(new img);

		v[v.size()-1]->sp.setTexture(*ntx);
		v[v.size()-1]->name = name;
		inp.read((char*)&p, sizeof(p));
		v[v.size()-1]->sp.setPosition(p);
	}

	return true;
}

void Map::readRect(sf::FloatRect &r, std::ifstream &inp)
{
	inp.read((char*)&r.left, sizeof(r.left));
	inp.read((char*)&r.top, sizeof(r.top));
	inp.read((char*)&r.width, sizeof(r.width));
	inp.read((char*)&r.height, sizeof(r.height));
}

void Map::readVecGeo(std::vector<sf::FloatRect> &v, std::ifstream &inp)
{
	int length;
	sf::FloatRect t;

	v.clear();

	inp.read((char*)&length, sizeof(length));
	for (int i = 0; i < length; i++)
	{
		readRect(t, inp);
		v.push_back(t);
	}
}

void Map::readString(std::string &s, std::ifstream &inp)
{
	int length;
	char c;

	s = "";

	inp.read((char*)&length, sizeof(length));
	for (int i = 0; i < length; i++)
	{
		inp.read((char*)&c, sizeof(c));
		s += c;
	}
}

void Map::readDoors(std::ifstream &inp) {
	int length;
	sf::Vector2f sp_pos;
	string str;

	inp.read((char*)&length, sizeof(length));
	int skip = 0;
	for (int i = 0; i < length; i++) {
		readString(str, inp);
		if (str == "door") {
			doors.push_back(new Door);
			doors.at(i - skip)->name = "door";
		} else {
			skip++;
			cout << "[MAP] Garbage portal detected" << endl;
			readString(str, inp);
			inp.read((char*)&sp_pos, sizeof(sp_pos));
			inp.read((char*)&sp_pos, sizeof(sp_pos));
			continue;
		}

		readString(doors.at(i)->target, inp);

		inp.read((char*)&sp_pos, sizeof(sp_pos));
		doors.at(i)->target_pos = sp_pos;

		inp.read((char*)&sp_pos, sizeof(sp_pos));
		doors.at(i)->setPosition(sp_pos);
	}
}

sf::Texture* Map::getTexture(std::string img_name)
{
	for (int i = 0; i < (int)tx.size(); i++)
	{
		if (tx[i]->name == img_name)
		{
			return tx[i]->texture;
		}
	}
	return NULL;
}

int Map::findRect(const sf::FloatRect &r, const std::vector<sf::FloatRect> &v)
{
	int i;
	for (i = v.size()-1; i >= 0; i--)
	{
		if (v[i] == r)
		{
			break;
		}
	}
	return i;
}


std::vector<sf::FloatRect>* Map::getGeom()
{
	return &geometry;
}

Map::Map(std::string _name)
{
	modified = false;
	deco = true;
	geom = false;
	name = _name;
	nextMap = 0;
	background = new Background(0,1920);
}

Map::~Map()
{
	
}
