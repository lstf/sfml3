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

void Player::setPosition(sf::Vector2f _pos)
{
	sp.setPosition(_pos);
}

void Player::draw(sf::RenderTarget& w, sf::RenderStates states) const
{
	sf::RectangleShape down;
	down.setSize(sf::Vector2f(1,1));
	down.setOutlineThickness(1.0);
	down.setFillColor(sf::Color::Transparent);

	for (int i = 0; i < 3; i++)
	{
		down.setOutlineColor(sf::Color::Yellow);
		down.setPosition(collide.top[i].p);
		if (collide.top[i].c) w.draw(down, states);

		down.setPosition(collide.down[i].p);
		if (collide.down[i].c) w.draw(down, states);

		down.setOutlineColor(sf::Color::Blue);
		down.setPosition(collide.right[i].p);
		if (collide.right[i].c) w.draw(down, states);

		down.setPosition(collide.left[i].p);
		if (collide.left[i].c) w.draw(down, states);


	}
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
	}
	if (input.left)
	{
		setState(WALKING_LEFT);
	}
	else if (input.right)
	{
		setState(WALKING_RIGHT);
	}
	if (!input.left && !input.right)
	{
		setState(STANDING);
	}
	if (event.type == sf::Event::MouseButtonPressed)
	{
	
	}
}

void Player::updateCollide(std::vector<sf::FloatRect>* geo)
{
	collide.top[L].i = -1; 
	collide.top[M].i = -1;
	collide.top[R].i = -1;
	collide.down[L].i = -1;
	collide.down[M].i = -1;
	collide.down[R].i = -1;
	collide.right[T].i = -1;
	collide.right[M].i = -1;
	collide.right[D].i = -1;
	collide.left[T].i = -1;
	collide.left[M].i = -1;
	collide.left[D].i = -1;
	collide.top[L].c = false; 
	collide.top[M].c = false;
	collide.top[R].c = false;
	collide.down[L].c = false;
	collide.down[M].c = false;
	collide.down[R].c = false;
	collide.right[T].c = false;
	collide.right[M].c = false;
	collide.right[D].c = false;
	collide.left[T].c = false;
	collide.left[M].c = false;
	collide.left[D].c = false;
	collide.bl.c = false;
	collide.br.c = false;
	collide.top[L].p = sp.getPosition() + sf::Vector2f(0, 0);
	collide.top[M].p = collide.top[L].p + sf::Vector2f(16.0, 0);
	collide.top[R].p = collide.top[L].p + sf::Vector2f(31.0, 0);
	collide.down[L].p = collide.top[L].p + sf::Vector2f(0, 47.0);
	collide.down[M].p = collide.down[L].p + sf::Vector2f(16.0, 0);
	collide.down[R].p = collide.down[L].p + sf::Vector2f(31.0, 0);
	collide.br.p = collide.down[R].p + sf::Vector2f(2.0, 0);
	collide.bl.p = collide.down[L].p - sf::Vector2f(2.0, 0);
	collide.right[T].p = collide.top[L].p + sf::Vector2f(31.0, 0);
	collide.right[M].p = collide.right[T].p + sf::Vector2f(0, 24.0);
	collide.right[D].p = collide.right[T].p + sf::Vector2f(0, 47.0);
	collide.left[T].p = collide.right[T].p + sf::Vector2f(-31.0, 0);
	collide.left[M].p = collide.right[M].p + sf::Vector2f(-31.0, 0);
	collide.left[D].p = collide.right[D].p + sf::Vector2f(-31.0, 0);
	collide.right[T].p += sf::Vector2f(2,0);
	collide.right[M].p += sf::Vector2f(2,0);
	collide.right[D].p += sf::Vector2f(2,0);
	collide.down[L].p += sf::Vector2f(0,2);
	collide.down[M].p += sf::Vector2f(0,2);
	collide.down[R].p += sf::Vector2f(0,2);
	collide.left[L].p -= sf::Vector2f(2,0);
	collide.left[M].p -= sf::Vector2f(2,0);
	collide.left[R].p -= sf::Vector2f(2,0);
	collide.top[L].p -= sf::Vector2f(0,2);
	collide.top[M].p -= sf::Vector2f(0,2);
	collide.top[R].p -= sf::Vector2f(0,2);

	for (int i = geo->size()-1; i >= 0; i--)
	{
		for (int j = 0; j < 3; j++)
		{
			if ((*geo)[i].contains(collide.top[j].p))
			{
				collide.top[j].i = i;
				collide.top[j].c = true;
			}
			if ((*geo)[i].contains(collide.down[j].p))
			{
				collide.down[j].i = i;
				collide.down[j].c = true;
			}
			if ((*geo)[i].contains(collide.left[j].p))
			{
				collide.left[j].i = i;
				collide.left[j].c = true;
			}
			if ((*geo)[i].contains(collide.right[j].p))
			{
				collide.right[j].i = i;
				collide.right[j].c = true;
			}
			if ((*geo)[i].contains(collide.br.p))
			{
				collide.br.c = true;
			}
			if ((*geo)[i].contains(collide.bl.p))
			{
				collide.bl.c = true;
			}
		}
	}
}

bool Player::collisionResolver(sf::Vector2f op, std::vector<sf::FloatRect>* geo)
{
	sf::Vector2f diff = sp.getPosition() - op;

	if (diff.x != 0 && diff.y != 0 &&
		std::fabs(std::fabs(diff.x/diff.y) - 1) < 0.1)
	{
		
	}

	int n = 0;
	bool moved = false;
	for (int j = (*geo).size()-1; j >= 0; j--)
	{
		while ((*geo)[j].intersects(sp.getGlobalBounds()))
		{
			moved = true;
			n++;
			if (n > 10)
			{
				break;
			}
			sp.setPosition(op + sf::Vector2f(diff.x*(1.0-n*.1), (diff.y*(1.0-n*.1)))); 
		}
	}
	return moved;
}

void Player::update(std::vector<sf::FloatRect>* geo, float frameTime)
{
	if (fresh)
	{
		updateCollide(geo);
		fresh = false;
	}
	CollisionPoints old_collision = collide;
	sf::Vector2f old_position = sp.getPosition();

	velocity.x = 0;
	
	//Gravity
	if (!old_collision.down[R].c &&
		!old_collision.down[M].c &&
		!old_collision.down[L].c)
	{
		velocity.y += frameTime*fallA;
		velocity.y = velocity.y > fallM ? fallM : velocity.y;
	}
	else 
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) &&
			!old_collision.top[L].c &&
			!old_collision.top[M].c &&
			!old_collision.top[R].c)
		{
			velocity.y = -5;
		}
		else
		{
			velocity.y = 0;
		}
	}
	if (velocity.y < 0 &&
		(old_collision.top[L].c ||
		old_collision.top[M].c ||
		old_collision.top[R].c))
	{
		velocity.y = 0;
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
		if (state == WALKING_LEFT &&
			!old_collision.left[T].c &&
			!old_collision.left[M].c &&
			!old_collision.left[D].c)
		{
			velocity.x = -1*frameTime*speed;
			colbox.down.left -= frameTime*speed;
		}
		else if (state == WALKING_RIGHT &&
				!old_collision.right[T].c &&
				!old_collision.right[M].c &&
				!old_collision.right[D].c)
		{
			velocity.x = frameTime*speed;
			colbox.down.left += frameTime*speed;
		}
		advanceAnimation();
	}

	bool moved;
	
	sp.move(velocity);

	moved = collisionResolver(old_position, geo);
	
	updateCollide(geo);

	if (moved)
	{
		bool floating = true;

		for (int i = 0; i < 3; i++)
		{
			if (collide.top[i].c) floating = false;
			if (collide.down[i].c) floating = false;
			if (collide.left[i].c) floating = false;
			if (collide.right[i].c) floating = false;
		}

		if (floating)
		{
			if (velocity.x == 0)
			{
				
			}
			else
			{
				velocity = sf::Vector2f(velocity.x, 0);
				sp.move(velocity + sf::Vector2f(0, -1));
			}
		}
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
		fallM = 10.0;
		velocity = sf::Vector2f(0,0);
		fresh = true;
	}
}

