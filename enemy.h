#ifndef ENEMY_O
#define ENEMY_O

#include <SFML/Graphics.hpp>

#include "entity.h"


using namespace std;

class Enemy : public Entity
{
private:
	sf::Texture tx;
	sf::Sprite sp;

public:
	Enemy();
};
