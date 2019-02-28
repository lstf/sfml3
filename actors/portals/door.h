#ifndef _DOOR_H
#define _DOOR_H
#include <fstream>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../portal.h"
#include "../../utils/txmap.h"

#define DOOR_DIR "./ats/mps/doors/"

using namespace std;

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

class Door : public Portal
{
private:
	DSpriteSheet sp_sheet;
	sf::Texture tx;
	bool opening;

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;
	
	
public:
	sf::Sprite sp;

	bool traversed;
	void setTargetPos(sf::Vector2f _target_pos);
	bool advanceAnimation();
	virtual sf::FloatRect bounds();
	virtual MapTrans* interact();
	virtual sf::Vector2f size();
	virtual void setPosition(sf::Vector2f pos);
	virtual sf::Vector2f getPosition();
	virtual bool update();
	Door();
};

#endif 
