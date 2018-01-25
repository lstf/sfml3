#include "map.h"

Map::Map()
{
}

bool Map::addWall(const sf::Vector2i &_pos, const sf::Vector2i &_size)
{
	geometry.push_back(sf::IntRect(_pos, _size));
	return true;
}

bool Map::addDeco(const sf::Texture &_tx, const sf::Vector2i _pos, bool _bg)
{
	std::vector<sf::Sprite> *g = _bg ? &bg : &fg;

	g->push_back(sf::Sprite(_tx));
	//g->back().setPosition(_pos);
	return true;
}

void Map::setName(std::string newName)
{
	name = newName;
}

Map::~Map()
{
}
