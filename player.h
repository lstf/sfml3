#ifndef PLAYER_O
#define PLAYER_O
#include <vector>
#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>

#define PLAY_DIR "./ats/player/"
#define PLAY_FPS 12.0

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
	std::vector<sf::IntRect>* geometry;
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

	void setState(States _state);
	
	void setAnimation(Animation a);

	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	void handleInput(sf::Event event);

	void update(std::vector<sf::IntRect>* geo, float frameTime);
		
	void advanceAnimation();

	Player();
};

#endif
