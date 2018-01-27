#ifndef _MAP_O
#define _MAP_O
#include <vector>
#include <string>
#include <fstream>
#include <unordered_set>
#include <iostream>

#include <SFML/Graphics.hpp>

#define MAP_DIR "./ats/mps/"


enum BgFg
{
	FG,
	BG
};

struct img
{
	std::string file;
	sf::Sprite sp;
};

class Map
{
private:
	std::vector<img> bg;
	std::vector<img> fg;

	Map();

	//saveMap() helper functions
	void writeVecImg	(std::vector<img> v, std::ofstream &o);
	void writeRect		(sf::IntRect r, std::ofstream &o);
	void writeVecGeo	(std::vector<sf::IntRect> v, std::ofstream &o);
	void writeString	(std::string s, std::ofstream &o);

	//loadMap() helper functions
	void readVecImg		(std::vector<img> &v, std::ifstream &inp);
	void readRect		(sf::IntRect &r, std::ifstream &inp);
	void readVecGeo		(std::vector<sf::IntRect> &v, std::ifstream &inp);
	void readString		(std::string &s, std::ifstream &inp);

public:
	std::string name;
	std::vector<sf::IntRect> geometry;
	std::vector<sf::Texture> tx;
	std::unordered_set<std::string> tx_names;

	bool modified;

	Map(std::string _name);
	bool addWall(const sf::Vector2i &_pos, const sf::Vector2i &_size);
	bool addDeco(const sf::Texture &_tx, const sf::Vector2i _pos, BgFg _bg = BG);
	bool saveMap();
	bool loadMap();
	~Map();
};

#endif
