#include "map.h"

void Map::draw(sf::RenderTarget& w, sf::RenderStates states) const
{
	if (deco)
	{
		for (int i = (int)bg.size() - 1; i >= 0; i--)
		{
			w.draw(bg[i]->sp, states);
		}
	}
	if (geom)
	{
		sf::RectangleShape 	rect;
		rect.setOutlineThickness(2.0);
		rect.setOutlineColor(sf::Color::Blue);
		rect.setFillColor(sf::Color::Transparent);

		sf::Vector2f size;
		sf::Vector2f pos;

		for (int i = (int)geometry.size() - 1; i >= 0; i--)
		{
			size.x 	= (float)geometry[i].width;
			size.y 	= (float)geometry[i].height;
			pos.x 	= (float)geometry[i].left;
			pos.y 	= (float)geometry[i].top;
			
			rect.setPosition(pos);
			rect.setSize(size);

			w.draw(rect, states);
		}
	}
}



bool Map::addWall(const sf::Vector2i &_pos, const sf::Vector2i &_size)
{
	modified = true;
	geometry.push_back(sf::IntRect(_pos, _size));
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
	if(!tx[tx.size()-1]->texture.loadFromFile(TEX_DIR + img_name))
	{
		tx.pop_back();
		return false;
	}

	
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



bool Map::loadMap()
{
	std::ifstream map_file((MAP_DIR + name).c_str(), std::ios::in|std::ios::binary);

	if (!map_file.is_open()) return false;

	modified = false;

	readString	(name, map_file);
	if (!readVecImg(bg, map_file)) return false;
	readVecGeo	(geometry, map_file);

	map_file.close();

	return true;
}

bool Map::readVecImg(std::vector<img*> &v, std::ifstream &inp)
{
	int length;
	sf::Vector2f p;
	sf::Texture t;

	v.clear();
	tx.clear();
	tx_names.clear();

	inp.read((char*)&length, sizeof(length));
	for (int i = 0; i < length; i++)
	{
		v.push_back(new img);

		readString(v[i]->name, inp);
		tx_names.insert(v[i]->name);

		inp.read((char*)&p, sizeof(p));
		v[v.size()-1]->sp.setPosition(p);
	}

	for (auto it = tx_names.begin(); it != tx_names.end(); ++it)
	{
		tx.push_back(new named_tx);

		if (!tx[tx.size()-1]->texture.loadFromFile(TEX_DIR + *it))
		{
			return false;
		}

		tx[tx.size()-1]->name = *it;
	}


	std::string tx_name;
	for (int i = 0; i < length; i++)
	{
		tx_name = v[i]->name;
		for (int j = tx.size()-1; j >= 0; j--)
		{
			if (tx_name == tx[j]->name)
			{
				v[i]->sp.setTexture(tx[j]->texture);
			}
		}
	}

	return true;
}

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



sf::Texture* Map::getTexture(std::string img_name)
{
	for (int i = 0; i < (int)tx.size(); i++)
	{
		if (tx[i]->name == img_name)
		{
			return &(tx[i]->texture);
		}
	}
	return NULL;
}

int Map::findRect(const sf::IntRect &r, const std::vector<sf::IntRect> &v)
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



void Map::select(sf::IntRect &s, std::vector<sf::IntRect> &v)
{
	if (deco)
	{
		sf::Vector2f pos;
		sf::IntRect r;

		r.width = 48;
		r.height = 48;

		for (int i = (int)bg.size()-1; i >= 0; i--)
		{
			pos = bg[i]->sp.getPosition();
			r.left = pos.x;
			r.top = pos.y;

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
}

void Map::clearSelect()
{
	bgSelection.clear();
	geometrySelection.clear();
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

void Map::moveSelect(const sf::Vector2f &v)
{
	modified = true;
	int length = (int)bgSelection.size();
	for (int i = 0; i < length; i++)
	{
		bg[bgSelection[i]]->sp.move(v);
	}

	length = (int)geometrySelection.size();
	for (int i = 0; i < length; i++)
	{
		geometry[geometrySelection[i]].left += (int)v.x;
		geometry[geometrySelection[i]].top += (int)v.y;
	}
}


std::vector<sf::IntRect>* Map::getGeom()
{
	return &geometry;
}

Map::Map(std::string _name)
{
	modified = false;
	deco = true;
	geom = false;
	name = _name;
}

Map::~Map()
{
	
}
