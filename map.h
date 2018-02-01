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
	friend class Editor;

private:

	std::vector<int> bgSelection;
	std::vector<int> geometrySelection;
	std::vector<img*> bg;
	std::vector<sf::IntRect> geometry;
	std::vector<named_tx*> tx;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	////
	//   Editor members
	////
	
	bool deco;		//Draw environment sprites
	bool geom; 		//Draw geometry

	//Adds geometry
	bool addWall(const sf::Vector2i &_pos, const sf::Vector2i &_size);
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
		void writeRect		(sf::IntRect r, std::ofstream &o);
		void writeVecGeo	(std::vector<sf::IntRect> v, std::ofstream &o);
		void writeString	(std::string s, std::ofstream &o);

		//loadMap() helper functions
		bool readVecImg		(std::vector<img*> &v, std::ifstream &inp);
		void readRect		(sf::IntRect &r, std::ifstream &inp);
		void readVecGeo		(std::vector<sf::IntRect> &v, std::ifstream &inp);
		void readString		(std::string &s, std::ifstream &inp);

	//Returns texture by name
	sf::Texture* getTexture(std::string img_name);
	//Returns index of rectange in vector (-1 if not found)
	int findRect(const sf::IntRect &r, const std::vector<sf::IntRect> &v);

	//Select visible decoration and geometry inside rectangle s
	void select(sf::IntRect &s, std::vector<sf::IntRect> &v);
	//Deselect all
	void clearSelect();
	//Delete selected
	void deleteSelect();
	//Duplicate selected (copy on same coords as selected, while original is deselected
	void duplicateSelect();
	//Move selected
	void moveSelect(const sf::Vector2f &v);

	////
	  // END - Editor members
	////

public:
	std::string name;

	//Constructor - Creates Map from map file with _name
	Map(std::string _name);
	bool saveMap();
	bool loadMap();
	~Map();
};

#endif
