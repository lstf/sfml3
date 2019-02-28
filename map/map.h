#ifndef _MAP_H
#define _MAP_H
#include <vector>
#include <iterator>
#include <string>
#include <fstream>
#include <unordered_set>
#include <set>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "background.h"
#include "../actors/player.h"
#include "../actors/portal.h"
#include "../actors/portals/door.h"

#define MAP_DIR "./ats/mps/"
#define TEX_DIR "./ats/mps/tx/"

struct img
{
	std::string name;
	sf::Sprite sp;
};

struct named_tx
{
	void* png;
	sf::Texture* texture;
	std::string name;
};

class Map : public sf::Drawable {
	friend class Editor;
	friend class Decopanel;
	friend class Geompanel;
	friend class Portpanel;

private:
	std::vector<img*> bg;
	std::vector<sf::FloatRect> geometry;
	std::vector<named_tx*> tx;
	std::vector<Portal*> doors;
	Background* background;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	////
	//   Editor members
	////
	
	bool deco;		//Draw environment sprites
	bool geom; 		//Draw geometry

	//Adds Door
	Portal* addDoor(string name);
	//Adds geometry
	bool addWall(const sf::Vector2f &_pos, const sf::Vector2f &_size);
	//Adds decoration
	bool addDeco(std::string name, const sf::Vector2f _pos);
	//Loads new texture
	bool loadTexture(std::string img_name);

	bool modified;	//True if map has been modified

		//Set used to load only one instance of each texture
		//Also used for listing textures in editor
		std::unordered_set<std::string> tx_names;
		//saveMap() helper functions
		void writeVecImg	(std::vector<img*> v, std::ofstream &o);
		void writeRect		(sf::FloatRect r, std::ofstream &o);
		void writeVecGeo	(std::vector<sf::FloatRect> v, std::ofstream &o);
		void writeString	(std::string s, std::ofstream &o);
		void writeDoors		(std::ofstream &o);

		//loadMap() helper functions
		bool readVecImg		(std::vector<img*> &v, std::ifstream &inp);
		void readRect		(sf::FloatRect &r, std::ifstream &inp);
		void readVecGeo		(std::vector<sf::FloatRect> &v, std::ifstream &inp);
		void readString		(std::string &s, std::ifstream &inp);
		void readDoors		(std::ifstream &inp);

	//Returns texture by name
	sf::Texture* getTexture(std::string img_name);
	//Returns index of rectange in vector (-1 if not found)
	int findRect(const sf::FloatRect &r, const std::vector<sf::FloatRect> &v);

	void deleteDoor(Portal* d);

	////
	  // END - Editor members
	////

public:
	int nextMap;
	Player* player;
	sf::Vector2f nextMap_pos;
	std::string name;

	std::vector<sf::FloatRect>* getGeom();

	//Constructor - Creates Map from map file with _name
	void setPlayer(Player* p);
	Map(std::string _name);
	bool saveMap();
	bool loadMap();
	~Map();
};

#endif
