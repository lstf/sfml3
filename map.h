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

#include "game.h"
#include "player.h"
#include "background.h"

#define MAP_DIR "./ats/mps/"
#define TEX_DIR "./ats/mps/tx/"
#define DOOR_DIR "./ats/mps/doors/"

struct img
{
	std::string name;
	sf::Sprite sp;
};

struct named_tx
{
	void* png;
	sf::Texture texture;
	std::string name;
};

struct DSpriteSheet
{
	void* 		png = NULL;
	sf::Texture tx;
	int 		pngSize = 0;
	int 		x = 0;
	int 		y = 0;
	int 		frameCount = 1;
	sf::Clock	clock;
	float		time = 0;
	float		fps = 1;
};


class Door : public sf::Drawable
{
private:
	DSpriteSheet sp_sheet;
	sf::Texture tx;
	bool opening;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
	
	
public:
	sf::Sprite sp;
	int target;
	sf::Vector2f target_pos;


	bool traversed;
	void setTargetPos(sf::Vector2f _target_pos);
	void open();
	void advanceAnimation();
	void update();
	Door();
};

class Map : public sf::Drawable, public Game_State
{
	friend class Editor;

private:
	std::vector<int> bgSelection;
	std::vector<int> geometrySelection;
	std::vector<int> doorSelection;
	std::vector<img*> bg;
	std::vector<sf::FloatRect> geometry;
	std::vector<named_tx*> tx;
	std::vector<Door*> doors;
	Background* background;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	////
	//   Editor members
	////
	
	bool deco;		//Draw environment sprites
	bool geom; 		//Draw geometry

	//Adds Door
	Door* addDoor();
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

	//Select visible decoration and geometry inside rectangle s
	void select(sf::FloatRect &s, std::vector<sf::FloatRect> &v);
	//Deselect all
	void clearSelect();
	//Delete selected
	void deleteSelect();
	//Duplicate selected (copy on same coords as selected, while original is deselected
	void duplicateSelect();
	//Move selected
	void moveSelect(const sf::Vector2f &v, bool snap = false);

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
	void handleInput(sf::Event event);
	void update();
	Map(std::string _name);
	bool saveMap();
	bool loadMap();
	~Map();
};

#endif
