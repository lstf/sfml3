#ifndef EDITOR_O
#define EDITOR_O

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "map.h"

enum Modes
{
	EDIT,
	GEOM,
	DECO
};

class Editor: public sf::Drawable
{
private:
	//Property of game
	std::vector<Map*>*		maps;
	sf::RenderWindow*		w;

	//swaps components of u and v such that
	//	u is the top right corner in the box formed by them
	void orient(sf::Vector2i &u, sf::Vector2i &v);

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	//Translates screen coords to world coords and performs snapping
	sf::Vector2i getMouseCoordinates();

public:
	Editor(sf::RenderWindow* _w, std::vector<Map*>* _maps, sf::Font &_font);

	void setMaps(std::vector<Map*>* _maps);
	void setWindow(sf::RenderWindow* w);

	bool checkUnsaved(std::string &unsaved);
	int findMap(std::string mapName);
	bool getConsoleInput(const sf::Event &event, std::string &str);
	void handleInput(sf::Event event);

	bool imgView;

	sf::View view;
	bool mouse_middle;

	std::string command;
	sf::Text	command_text;
	sf::Text	message_text;
	bool 		console;
	bool		message;
	bool 		snap;
	bool		deco;
	int			map_index;

	sf::Vector2i	mouse_pos;
	sf::RectangleShape mouse_box;	
	sf::Sprite 		deco_cursor;

	std::string		deco_name;

	bool			mouse_left;

	Modes mode;
};

#endif
