#ifndef _MAP_O
#define _MAP_O
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

class Map
{
private:
	std::vector<sf::IntRect> geometry;
	std::vector<sf::Sprite> bg;
	std::vector<sf::Sprite> fg;
	std::string name;

public:
	Map();
	bool addWall(const sf::Vector2i &_pos, const sf::Vector2i &_size);
	bool addDeco(const sf::Texture &_tx, const sf::Vector2i _pos, bool _bg = true);
	void setName(std::string newName);
	~Map();
};

#endif
