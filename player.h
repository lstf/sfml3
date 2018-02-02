#ifndef PLAYER_O
#define PLAYER_O
#include <vector>
#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>

#define PLAY_DIR "./ats/player/"
#define PLAY_FPS 12.0
#define L 0
#define M 1
#define R 2
#define T 0
#define D 2

enum Animation
{
	RIGHT,
	LEFT
};

enum States
{
	STANDING,
	WALKING_LEFT,
	WALKING_RIGHT
};

struct CollisionPoint
{
	sf::Vector2f p;
	bool c;
};

struct CollisionPoints
{
	CollisionPoint top[3];
	CollisionPoint down[3];
	CollisionPoint right[3];
	CollisionPoint left[3];
};

struct CollisionBoxes
{
	sf::FloatRect down;
	sf::FloatRect right;
};

struct InputBools
{
	bool left = false;
	bool right = false;
};

class Player : public sf::Drawable
{
private:
	std::vector<sf::FloatRect>* geometry;
	void* spriteSheet;
	int spriteSheetLength;
	sf::Clock animationClock;
	float animationTime;
	int animationCol;
	int animationFrameCount;
	int animationRow;
	sf::Sprite sp;
	sf::Texture tx;
	InputBools input;
	States state;
	bool stateModified;
	int speed;
	int fallA;
	float fallS;
	float fallM;
	CollisionBoxes colbox;
	CollisionPoints collide;

	void updateCollide();

	void setState(States _state);
	
	void setAnimation(Animation a);

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	void handleInput(sf::Event event);

	void update(std::vector<sf::FloatRect>* geo, float frameTime);
		
	void advanceAnimation();

	Player();
};

#endif
