#ifndef EDITOR_O
#define EDITOR_O

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

#include "map.h"
#include "entity.h"
#include "player.h"
#include "sign.h"
#include "enemy.h"
#include "testenemy.h"

#define ABS(x) ((x) < 0 ? (x) * -1 : (x))

enum Modes
{
	EDIT,
	GEOM,
	DECO,
	PLAY,
	DOOR,
	DOOR_TARGET,
	SIGN,
	ENEMY
};

class Editor: public sf::Drawable
{
private:
	std::vector<Map*>*		maps;			//map list belonging to main
	sf::RenderWindow*		w;				//window belonging to main
	Player*					player;

	Modes mode;								//Current editor mode

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	//Translates sfml keycodes to ascii for the console (appended to str)
	//Returns true if input was a typable character
	bool getConsoleInput(const sf::Event &event, std::string &str);
		std::string command;		//command being typed in console
		sf::Text	command_text;	//drawable representation of command text
		sf::Text	message_text;	//drawable message text
		bool 		console;		//True if console active
		//Print help screen
		void helpCommand();
		//Door subprogram
		void doorCommand();
		//Quit subprogram
		void quitCommand(const std::string &_arg);
		//Toggles boolean and prints message to screen
		void toggleCommand(bool &togg, const std::string &_name);
		//Map subprogram 
		void mapCommand();
		//Img subprogram
		void imgCommand();
		void signCommand();
		void enemyCommand();
		//Print unknown command to screen in message
		void unknownCommand();
		
	//Translates screen coords to world coords and performs snapping
	//Returns resulting vector
	sf::Vector2f getMouseCoordinates();
		sf::Vector2f				mouse_pos;		//Mouse position storage
		sf::RectangleShape 			mouse_box;		//drawable box for EDIT and GEOM
		sf::Sprite 					deco_cursor;	//drawable sprite for DECO
		std::string					deco_name;		//name of deco_cursor's tx
		bool						snap;			//True if snapping active
		bool						mouse_left;		//True if left button active
		bool 						mouse_middle;	//True if middle button active 
		bool 						selectClicked;	//True if selection left clicked
		std::vector<sf::FloatRect> 	selection;		//Rect locations of selected objects
		Entity*						active_ent;
		Enemy*						active_enm;


	Door* door;

	bool imgView;	//True if img pane active

	//Returns true if unsaved maps exist and lists them in unsaved
	bool checkUnsaved(std::string &unsaved);
	//Returns index of mapName if it exists and -1 otherwise
	int findMap(std::string mapName);

	//Swap u and v components so they form a rectangle
	void orient(sf::Vector2f &u, sf::Vector2f &v);

public:
	sf::View 			view;			//Editor centered view
	int					map_index;		//working map index

	bool consoleActive();

	//Set Mode 
	void setMode(Modes _mode);

	sf::View getView();

	Editor(sf::RenderWindow* _w, Player* _p, std::vector<Map*>* _maps, sf::Font &_font);

	void handleInput(sf::Event event);
};

#endif
