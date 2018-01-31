#ifndef _MAP_O
#define _MAP_O
#include <vector>
#include <iterator>
#include <string>
#include <fstream>
#include <unordered_set>
#include <set>
#include <iostream>

#include <SFML/Graphics.hpp>

#define MAP_DIR "./ats/mps/"
#define TEX_DIR "./ats/mps/tx/"


enum BgFg
{
	FG,
	BG
};

struct img
{
	std::string name;
	sf::Sprite sp;
};

struct named_tx
{
	sf::Texture texture;
	std::string name;
};

class Map : public sf::Drawable
{
private:
	bool selection;
	std::vector<int> bgSelection;
	std::vector<int> geometrySelection;
	std::vector<img*> fg;
	bool deco;
	bool geom;

	Map();

	//saveMap() helper functions
	void writeVecImg	(std::vector<img*> v, std::ofstream &o);
	void writeRect		(sf::IntRect r, std::ofstream &o);
	void writeVecGeo	(std::vector<sf::IntRect> v, std::ofstream &o);
	void writeString	(std::string s, std::ofstream &o);

	//loadMap() helper functions
	bool readVecImg		(std::vector<img*> &v, std::ifstream &inp);
	void readRect		(sf::IntRect &r, std::ifstream &inp);
	void readVecGeo		(std::vector<sf::IntRect> &v, std::ifstream &inp);
	void readString		(std::string &s, std::ifstream &inp);

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	std::vector<img*> bg;
	std::string name;
	std::vector<sf::IntRect> geometry;
	std::vector<named_tx*> tx;
	std::unordered_set<std::string> tx_names;

	bool modified;
	sf::Texture* getTexture(std::string img_name);
	
	int findRect(const sf::IntRect &r, const std::vector<sf::IntRect> &v);

	void clearSelect();

	void deleteSelect();

	void select(sf::IntRect &s, std::vector<sf::IntRect> &v);

	std::vector<sf::IntRect>* select(sf::IntRect s);

	bool toggleDeco();
	bool toggleGeom();
	bool loadTexture(std::string img_name);
	Map(std::string _name);
	bool addWall(const sf::Vector2i &_pos, const sf::Vector2i &_size);
	bool addDeco(std::string name, const sf::Vector2f _pos, BgFg _bg);
	bool saveMap();
	bool loadMap();
	~Map();
};

#endif
