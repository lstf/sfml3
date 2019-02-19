#include "map.h"

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

		gRect.setOutlineThickness(2.0);
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

Door* Map::addDoor()
{
	doors.push_back(new Door);
	return doors.at(doors.size()-1);
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
	im->sp.setTexture(*(getTexture(name)));
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

void Map::writeDoors(std::ofstream &o)
{
	int length = doors.size();
	sf::Vector2f sp_pos;

	o.write((char*)&length, sizeof(length));
	for (int i = 0; i < length; i++)
	{
		sp_pos = doors.at(i)->sp.getPosition();
		o.write((char*)&sp_pos, sizeof(sp_pos));

		writeString(doors.at(i)->target, o);

		sp_pos = doors.at(i)->target_pos;
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
		ntx = txmap::get_tx(TEX_DIR+name);
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

void Map::readDoors(std::ifstream &inp)
{
	int length;

	sf::Vector2f sp_pos;

	inp.read((char*)&length, sizeof(length));
	for (int i = 0; i < length; i++)
	{
		doors.push_back(new Door);

		inp.read((char*)&sp_pos, sizeof(sp_pos));
		doors.at(i)->sp.setPosition(sp_pos);

		readString(doors.at(i)->target, inp);

		inp.read((char*)&sp_pos, sizeof(sp_pos));
		doors.at(i)->target_pos = sp_pos;
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



void Map::select(sf::FloatRect &s, std::vector<sf::FloatRect> &v)
{
	if (deco)
	{
		sf::FloatRect r;

		for (int i = (int)bg.size()-1; i >= 0; i--)
		{
			r = bg[i]->sp.getGlobalBounds();

			if (s.intersects(r))
			{
				if (findRect(r,v) < 0)
				{
					v.push_back(r);
					bgSelection.push_back(i);
				}
			}
		}
	}
	if (geom)
	{
		for (int i = (int)geometry.size()-1; i >= 0; i--)
		{
			if (s.intersects(geometry[i]))
			{
				if (findRect(geometry[i], v) < 0)
				{
					v.push_back(geometry[i]);
					geometrySelection.push_back(i);
				}
			}
		}
	}
	for (int i = (int)doors.size()-1; i >= 0; i--)
	{
		if (s.intersects(doors.at(i)->sp.getGlobalBounds()))
		{
			v.push_back(doors.at(i)->sp.getGlobalBounds());
			doorSelection.push_back(i);
		}
	}
}

void Map::clearSelect()
{
	bgSelection.clear();
	geometrySelection.clear();
	doorSelection.clear();
}

void Map::deleteSelect()
{
	modified = true;
	auto iti = bg.begin();
	int length = (int)bgSelection.size();
	for (int i = 0; i < length; i++)
	{
		bg.erase(iti + bgSelection[i]);
	}
	bgSelection.clear();

	auto itg = geometry.begin();
	length = (int)geometrySelection.size();
	for (int i = 0; i < length; i++)
	{
		geometry.erase(itg + geometrySelection[i]);
	}
	geometrySelection.clear();

	auto itd = doors.begin();
	length = (int)doorSelection.size();
	for (int i = 0; i < length; i++)
	{
		delete doors.at(doorSelection[i]);
		doors.erase(itd + doorSelection[i]);
	}
	doorSelection.clear();
}

void Map::duplicateSelect()
{
	modified = true;

	int length = (int)bgSelection.size();
	for (int i = 0; i < length; i++)
	{
		bg.push_back(new img);
		bg[bg.size()-1]->name = bg[bgSelection[i]]->name;
		bg[bg.size()-1]->sp.setTexture(*bg[bgSelection[i]]->sp.getTexture());
		bg[bg.size()-1]->sp.setPosition(bg[bgSelection[i]]->sp.getPosition());
		bgSelection[i] = bg.size()-1;
	}

	length = (int)geometrySelection.size();
	for (int i = 0; i < length; i++)
	{
		geometry.push_back(geometry[geometrySelection[i]]);
		geometrySelection[i] = geometry.size()-1;
	}
}

void Map::moveSelect(const sf::Vector2f &v, bool snap)
{
	modified = true;
	int length = (int)bgSelection.size();
	for (int i = 0; i < length; i++)
	{
		bg[bgSelection[i]]->sp.move(v);
		if (snap)
		{
			sf::Vector2f new_pos = bg[bgSelection[i]]->sp.getPosition();
			new_pos.x = (int)new_pos.x - (int)new_pos.x % 32;
			new_pos.y = (int)new_pos.y - (int)new_pos.y % 32;
			bg[bgSelection[i]]->sp.setPosition(new_pos);
		}
	}

	length = (int)geometrySelection.size();
	for (int i = 0; i < length; i++)
	{
		geometry[geometrySelection[i]].left += (int)v.x;
		geometry[geometrySelection[i]].top += (int)v.y;

		if (snap)
		{
			sf::FloatRect r = geometry[geometrySelection[i]];
			geometry[geometrySelection[i]].left -= (int)r.left % 32;
			geometry[geometrySelection[i]].top -= (int)r.top % 32;
		}
	}
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
