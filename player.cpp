#include "player.h"

void Player::setState(States _state)
{
	state = _state;
	stateModified = true;
}

void Player::setAnimation(Animation a)
{
	animationCol = 0;

	switch(a)
	{
	case RIGHT:
		animationRow = 0;
		animationFrameCount = 6;
	break;
	case LEFT:
		animationRow = 1;
		animationFrameCount = 6;
	break;	
	}
}

void Player::draw(sf::RenderTarget& w, sf::RenderStates states) const
{
	sf::RectangleShape down;
	down.setPosition(sf::Vector2f(colbox.down.left,colbox.down.top));
	down.setSize(sf::Vector2f(colbox.down.width,colbox.down.height));
	down.setOutlineColor(sf::Color::Blue);
	down.setOutlineThickness(1.0);
	down.setFillColor(sf::Color::Transparent);
	w.draw(down, states);
	w.draw(sp, states);
}

void Player::handleInput(sf::Event event)
{
	input.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

	if (event.type == sf::Event::KeyPressed)
	{
		//Pick most recent direction so left + right doesnt stop movement
		if (event.key.code == sf::Keyboard::Left &&
			input.right)
		{
			input.right = false;
		}
		else if (event.key.code == sf::Keyboard::Right &&
				 input.left)
		{
			input.left = false;
		}

		if (input.left)
		{
			setState(WALKING_LEFT);
		}
		else if (input.right)
		{
			setState(WALKING_RIGHT);
		}
	}
	if (!input.left && !input.right)
	{
		setState(STANDING);
	}
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Right &&
			sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			fallS = 0;
			sp.setPosition(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
			colbox.down.top = event.mouseButton.y + 48;
			colbox.down.left = event.mouseButton.x;
		}
	}
}

void Player::update(std::vector<sf::IntRect>* geo, float frameTime)
{
	bool grounded = false;
	bool blockedRight = false;
	int blockRightGeo;
	int	groundGeo;
	colbox.down.top = sp.getPosition().y + 48;
	colbox.down.left = sp.getPosition().x;
	colbox.right.top = sp.getPosition().y;
	colbox.right.left = sp.getPosition().x + 32;

	sf::IntRect downInt;
	downInt.top = colbox.down.top;
	downInt.left = colbox.down.left;
	downInt.height = colbox.down.height;
	downInt.width = colbox.down.width;

	sf::IntRect rightInt;
	rightInt.top = colbox.right.top;
	rightInt.left = colbox.right.left;
	rightInt.width = colbox.right.width;
	rightInt.height = colbox.right.height;

	//DOWN
	for (int i = geo->size()-1; i >= 0; i--)
	{
		
		if (!grounded && downInt.intersects((*geo)[i]))
		{
			grounded = true;
			groundGeo = i;
		}
		if (!blockedRight && rightInt.intersects((*geo)[i]))
		{
			blockedRight = true;
			blockRightGeo = i;
		}
	}
	//RIGHT
	if (blockedRight)
	{
		sf::IntRect intBounds;
		if (intBounds.intersects((*geo)[blockRightGeo]))
		{
			sp.setPosition((*geo)[blockRightGeo].left-32, sp.getPosition().y);
		}
	}
	if (!grounded)
	{
		fallS += frameTime*fallA;
		if (fallS > fallM)
		{
			fallS = fallM;
		}
		sp.move((float)0, fallS);
		colbox.down.top += fallS;
	}
	else
	{
		sf::IntRect intBounds;
		intBounds.left = (int)sp.getPosition().x;
		intBounds.top = (int)sp.getPosition().y;
		intBounds.width = 32;
		intBounds.height = 48;

		////Inside floor->on top of floor
		if (intBounds.intersects((*geo)[groundGeo]))
		{
			sp.setPosition(sp.getPosition().x, (*geo)[groundGeo].top-48);
		}
		fallS = 0.0;
	}

	if (stateModified)
	{
		stateModified = false;
		if (state == WALKING_LEFT)
		{
			setAnimation(LEFT);
		}
		else if (state == WALKING_RIGHT)
		{
			setAnimation(RIGHT);
		}
	}
	if (state != STANDING)
	{
		if (state == WALKING_LEFT)
		{
			sp.move(-1*frameTime*speed, (float)0);
			colbox.down.left -= frameTime*speed;
		}
		else if (state == WALKING_RIGHT && !blockedRight)
		{
			sp.move(frameTime*speed, (float)0);
			colbox.down.left += frameTime*speed;
		}
		advanceAnimation();
	}
}

void Player::advanceAnimation()
{
	
	animationTime += animationClock.getElapsedTime().asSeconds();
	if (animationTime > 1/PLAY_FPS)
	{
		animationTime = 0;
		animationClock.restart();
		animationCol++;
		animationCol %= animationFrameCount;

		tx.loadFromMemory(spriteSheet, spriteSheetLength, sf::IntRect(animationCol*32, animationRow*48,32,48));
		sp.setTexture(tx);
	}
}


Player::Player()
{
	std::ifstream inp(std::string(PLAY_DIR) + "P.PNG", std::ifstream::binary);
	if (!inp.is_open())
	{
		//Bad
	}
	else 
	{
		inp.seekg(0, inp.end);
		spriteSheetLength = inp.tellg();
		inp.seekg(0, inp.beg);
		spriteSheet = (void*)new char[spriteSheetLength];
		inp.read((char*)spriteSheet, spriteSheetLength);
		tx.loadFromMemory(spriteSheet, spriteSheetLength, sf::IntRect(0,0,32,48));
		sp.setTexture(tx);
		animationClock.restart();
		animationTime = 0;
		animationCol = 0;
		animationFrameCount = 1;
		animationRow = 0;
		state = STANDING;
		stateModified = false;
		speed = 200;
		colbox.down.top = 48;
		colbox.down.left = 0;
		colbox.down.width = 32;
		colbox.down.height = 1;
		colbox.right.top = 0;
		colbox.right.left = 32;
		colbox.right.width = 1;
		colbox.right.height = 48;
		fallA = 10;
		fallS = 0.0;
		fallM = 200.0;
	}
}

