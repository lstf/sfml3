#include "map.h"

Map::Map(std::string _name)
{
	modified = false;
	name = _name;
}

//void Map::writeVecImg(std::vector<img> v, std::ofstream &o)
//{
//	int length = v.size();
//	sf::Vector2f pos;
//
//	o.write((char*)&length, sizeof(length));
//	for (int i = 0; i < v.size(); i++)
//	{
//		writeString(v[i].file, o);
//		pos = v[i].sp.getPosition();
//		o.write((char*)&pos, sizeof(pos));
//	}
//}

void Map::writeRect(sf::IntRect r, std::ofstream &o)
{
	o.write((char*)&r.left, sizeof(r.left));
	o.write((char*)&r.top, sizeof(r.top));
	o.write((char*)&r.width, sizeof(r.width));
	o.write((char*)&r.height, sizeof(r.height));
}

void Map::writeVecGeo(std::vector<sf::IntRect> v, std::ofstream &o)
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


sf::Texture* Map::getTexture(std::string img_name)
{
	for (int i = 0; i < tx.size(); i++)
	{
		if (tx[i]->name == img_name)
		{
			return &(tx[i]->texture);
		}
	}
	return NULL;
}

bool Map::loadTexture(std::string img_name)
{
	tx.push_back(new named_tx);
	tx[tx.size()-1]->name = img_name;
	if(!tx[tx.size()-1]->texture.loadFromFile(TEX_DIR + img_name))
	{
		tx.pop_back();
		return false;
	}

	
	tx_names.insert(img_name);
	return true;
}

//void Map::readVecImg(std::vector<img> &v, std::ifstream &inp)
//{
//	int length;
//	sf::Vector2f p;
//	img	im;
//	sf::Texture t;
//
//	v.clear();
//	tx.clear();
//	tx_names.clear();
//
//	inp.read((char*)&length, sizeof(length));
//	for (int i = 0; i < length; i++)
//	{
//		readString(v[i].file, inp);
//		tx_names.insert(v[i].file);
//
//		inp.read((char*)&p, sizeof(p));
//
//		im.sp.setPosition(p);
//		v.push_back(im);
//	}
//
//	//for (auto it = tx_names.begin(); it != tx_names.end(); ++it)
//	//{
//	//	t.loadFromFile(MAP_DIR + *it);
//	//	tx.push_back(t);
//	//}
//}

void Map::readRect(sf::IntRect &r, std::ifstream &inp)
{
	inp.read((char*)&r.left, sizeof(r.left));
	inp.read((char*)&r.top, sizeof(r.top));
	inp.read((char*)&r.width, sizeof(r.width));
	inp.read((char*)&r.height, sizeof(r.height));
}

void Map::readVecGeo(std::vector<sf::IntRect> &v, std::ifstream &inp)
{
	int length;
	sf::IntRect t;

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

bool Map::saveMap()
{
	std::ofstream map_file((MAP_DIR + name).c_str(), std::ios::out|std::ios::binary);

	if (!map_file.is_open()) return false;

	modified = false;

	writeString	(name, map_file);
	//writeVecImg	(bg, map_file);
	//writeVecImg	(fg, map_file);
	writeVecGeo	(geometry, map_file);

	map_file.close();
	
	return true;
}

bool Map::loadMap()
{
	std::ifstream map_file((MAP_DIR + name).c_str(), std::ios::in|std::ios::binary);

	if (!map_file.is_open()) return false;

	modified = false;

	readString	(name, map_file);
	//readVecImg	(bg, map_file);
	//readVecImg	(fg, map_file);
	readVecGeo	(geometry, map_file);

	map_file.close();

	return true;
}

bool Map::addWall(const sf::Vector2i &_pos, const sf::Vector2i &_size)
{
	modified = true;
	geometry.push_back(sf::IntRect(_pos, _size));
	return true;
}

bool Map::addDeco(std::string name, const sf::Vector2f _pos, BgFg _bg)
{
	img* im = new img;
	im->sp.setPosition(_pos);
	im->name = name;
	im->sp.setTexture(*(getTexture(name)));
	if (_bg == BG)
	{
		bg.push_back(im);
	}

	modified = true;
	return true;
}

Map::~Map()
{
}
