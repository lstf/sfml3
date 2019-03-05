#ifndef _MAP_H
#define _MAP_H
#include <map>
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

#define MAP_SP_LAYERS 5

using namespace std;

struct img {
	string name;
	sf::Sprite sp;
};

struct named_tx {
	void* png;
	sf::Texture* texture;
	string name;
};

class Map : public sf::Drawable {
	friend class Editor;
	friend class Decopanel;
	friend class Geompanel;
	friend class Portpanel;

private:
	map<string, int> init_lstate;
	map<string, int>* lstate;

	vector<img*> sp[MAP_SP_LAYERS];
	vector<sf::FloatRect> geometry;
	vector<named_tx*> tx;
	vector<Portal*> doors;
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
	bool addDeco(string name, const sf::Vector2f _pos, int l);
	//Loads new texture
	bool loadTexture(string img_name);

	bool modified;	//True if map has been modified

		//Set used to load only one instance of each texture
		//Also used for listing textures in editor
		unordered_set<string> tx_names;
		//saveMap() helper functions
		void writeVecImg	(vector<img*> v, ofstream &o);
		void writeRect		(sf::FloatRect r, ofstream &o);
		void writeVecGeo	(vector<sf::FloatRect> v, ofstream &o);
		void writeString	(string s, ofstream &o);
		void writeDoors		(ofstream &o);

		//loadMap() helper functions
		bool readVecImg		(vector<img*> &v, ifstream &inp);
		void readRect		(sf::FloatRect &r, ifstream &inp);
		void readVecGeo		(vector<sf::FloatRect> &v, ifstream &inp);
		void readString		(string &s, ifstream &inp);
		void readDoors		(ifstream &inp);

	//Returns texture by name
	sf::Texture* getTexture(string img_name);
	//Returns index of rectange in vector (-1 if not found)
	int findRect(const sf::FloatRect &r, const vector<sf::FloatRect> &v);

	void deleteDoor(Portal* d);

	////
	  // END - Editor members
	////

public:
	int nextMap;
	Player* player;
	sf::Vector2f nextMap_pos;
	string name;

	vector<sf::FloatRect>* getGeom();

	//Constructor - Creates Map from map file with _name
	void setLstate(map<string, int>* _lstate);
	void setPlayer(Player* p);
	Map(string _name);
	bool saveMap();
	bool loadMap();
	~Map();
};

#endif
