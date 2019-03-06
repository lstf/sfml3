#include "player.h"

Weapon::Weapon() {
	Png png = txmap::get_png(std::string(PLAY_DIR) + "W.PNG");
	sp_sheet.png = png.mem;
	sp_sheet.pngSize = png.length;

	tx.loadFromMemory(sp_sheet.png, sp_sheet.pngSize, sf::IntRect(0,0,32,32));
	sp.setTexture(tx);

	sp_sheet.clock.restart();
	sp_sheet.time = 0;
	sp_sheet.x = 0;
	sp_sheet.y = 0;
	sp_sheet.frameCount = 4;
	sp_sheet.dir = RIGHT;
	sp_sheet.fps = 6;

	active = false;
}

void Weapon::advanceAnimation() {
	sp_sheet.time += sp_sheet.clock.getElapsedTime().asSeconds();
	if (sp_sheet.time > 1/sp_sheet.fps) {
		sp_sheet.clock.restart();
		sp_sheet.x++;
		if (sp_sheet.x >= sp_sheet.frameCount) {
			active = false;
			sp_sheet.x = 0;
		}
		sp_sheet.y =  sp_sheet.dir == RIGHT ? 0 : 1;

		tx.loadFromMemory(sp_sheet.png, sp_sheet.pngSize, sf::IntRect(sp_sheet.x*32, sp_sheet.y*32, 32, 32));
		sp.setTexture(tx);
	}
}

void Weapon::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(sp, states);
}


void Weapon::attack() {
	if (!active) {
		active = true;
	}
}

void Weapon::setDirection(Animation _a) {
	sp_sheet.dir = _a;
	sp_sheet.x = -1;
	advanceAnimation();
}

void Weapon::setPosition(sf::Vector2f _pos) {
	sf::Vector2f offset(0,0);
	offset.x = sp_sheet.dir == RIGHT ? 16 : -16;
	sp.setPosition(_pos + offset);
}

sf::FloatRect Weapon::bounds() {
	return sp.getGlobalBounds();
}

void Player::setState(States _state) {
	state = _state;
	stateModified = true;
}

void Player::setAnimation(Animation a) {
	animationCol = 0;

	switch(a) {
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

void Player::setPosition(sf::Vector2f _pos) {
	state = STANDING;
	sp.setPosition(_pos);
}

void Player::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(sp, states);
	w.draw(weapon, states);
}

void Player::handleInput(sf::Event event) {
	input.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	input.down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	input.up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		weapon.attack();
	}
	if (event.type == sf::Event::KeyPressed) {
		//Pick most recent direction so left + right doesnt stop movement
		if (event.key.code == sf::Keyboard::Left && input.right) {
			input.right = false;
		}
		else if (event.key.code == sf::Keyboard::Right && input.left) {
			input.left = false;
		}
		else if (event.key.code == sf::Keyboard::Up && input.down) {
			input.down = false;
		}
		else if (event.key.code == sf::Keyboard::Down && input.up) {
			input.up = false;
		}
		else if (event.key.code == sf::Keyboard::Z) {
			input.jump = true;
		}
	}
	if (input.left) {
		setState(WALKING_LEFT);
		weapon.setDirection(LEFT);
	}
	else if (input.right) {
		setState(WALKING_RIGHT);
		weapon.setDirection(RIGHT);
	}
	if (!input.left && !input.right) {
		setState(STANDING);
	}
	if (input.up) {
		interaction = true;
	}
}

void Player::updateCollide(std::vector<sf::FloatRect>* geo) {
	coldirs.up = false;
	coldirs.down = false;
	coldirs.left = false;
	coldirs.right = false;

	sf::FloatRect u = sp.getGlobalBounds();
	sf::FloatRect d = u;
	sf::FloatRect l = u;
	sf::FloatRect r = u;

	d.height = 1;
	d.top = u.top + u.height;
	//d.width *=  3 / 4; 
	//d.left += d.width / 3 / 2;

	u.height = 1;
	u.top -= 1;
	//u.width *= 3 / 4;
	//u.left += u.width / 3 / 2;

	r.width = 1;
	r.left = l.left + l.width;
	//r.height *= 3 / 4;
	//r.top += r.height / 3 / 2;

	l.width = 1;
	l.left -= 1;
	//l.height *= 3 / 4;
	//l.top += l.height / 3 / 2;

	for (int i = geo->size()-1; i >= 0; i--) {
		if ((*geo)[i].intersects(u)) {
			coldirs.up = true;
		}
		if ((*geo)[i].intersects(d)) {
			coldirs.down  = true;
		}
		if ((*geo)[i].intersects(l)) {
			coldirs.left = true;
		}
		if ((*geo)[i].intersects(r)) {
			coldirs.right = true;
		}
	}
}

bool Player::collisionResolver(sf::Vector2f op,
std::vector<sf::FloatRect>* geo) {
	sf::Vector2f diff = sp.getPosition() - op;

	int n = 0;
	bool moved = false;

	//back up
	for (int j = (*geo).size()-1; j >= 0; j--) {
		while ((*geo)[j].intersects(sp.getGlobalBounds())) {
			moved = true;
			n++;
			if (n > 10) {
				break;
			}
			sp.setPosition(op + sf::Vector2f(diff.x*(1.0-n*.1), (diff.y*(1.0-n*.1)))); 
		}
	}

	return moved;
}


void Player::update(std::vector<sf::FloatRect>* geo, double frameTime) {
	if (fresh) {
		updateCollide(geo);
		fresh = false;
	}

	ColDirs oldirs = coldirs;
	sf::Vector2f old_position = sp.getPosition();

	velocity.x = 0;
	
	if (!oldirs.down) {
		velocity.y += frameTime*frameTime*fallA;
		velocity.y = velocity.y > fallM*frameTime ? fallM*frameTime : velocity.y;
	} else {
		if (input.jump) {
			if (!oldirs.up) {
				velocity.y = jumph*frameTime;
			}
			input.jump = false;
		} else {
			velocity.y = 0;
		}
	}
	if (velocity.y < 0 && oldirs.up) {
		velocity.y = 0;
	}
	//TODO something weird going on with jump height
	//if (velocity.y > 0 && !oldirs.down) {
	//	cout << sp.getPosition().y << endl;
	//}
	if (stateModified) {
		stateModified = false;
		if (state == WALKING_LEFT) {
			setAnimation(LEFT);
		} else if (state == WALKING_RIGHT) {
			setAnimation(RIGHT);
		}
	}
	if (state != STANDING) {
		if (state == WALKING_LEFT && !oldirs.left) {
			velocity.x = -1*frameTime*speed;
		} else if (state == WALKING_RIGHT && !oldirs.right) {
			velocity.x = frameTime*speed;
		}
		advanceAnimation();
	}
	if (weapon.active) {
		weapon.advanceAnimation();
	}

	sp.move(velocity);

	bool moved = collisionResolver(old_position, geo);

	updateCollide(geo);

	if (moved && !coldirs.up && !coldirs.down && !coldirs.right && 
	!coldirs.left) {
		velocity = sf::Vector2f(velocity.x, 0);
		sp.move(velocity + sf::Vector2f(0, -1));
	}

	refresh();
}

void Player::refresh() {
	sf::Vector2f pp = sp.getPosition();
	weapon.setPosition(pp);
	view.setCenter(sf::Vector2f(int(pp.x), int(pp.y)));
}

void Player::advanceAnimation() {
	animationTime += animationClock.getElapsedTime().asSeconds();
	if (animationTime > 1/PLAY_FPS) {
		animationTime = 0;
		animationClock.restart();
		animationCol++;
		animationCol %= animationFrameCount;

		tx.loadFromMemory(png.mem, png.length, sf::IntRect(animationCol*32, animationRow*48,32,48));
		sp.setTexture(tx);
	}
}

sf::FloatRect Player::weaponBounds() {
	return weapon.bounds();
}

bool Player::weaponActive() {
	return weapon.active;
}

Player::Player() {
	png = txmap::get_png(std::string(PLAY_DIR) + "P.PNG");
	tx.loadFromMemory(png.mem, png.length, sf::IntRect(0,0,32,48));

	sp.setTexture(tx);

	animationClock.restart();
	animationTime = 0;
	animationCol = 0;
	animationFrameCount = 1;
	animationRow = 0;
	state = STANDING;
	stateModified = false;
	speed = 200;
	fallA = 1000;
	fallM = 1000;
	jumph = -400;
	velocity = sf::Vector2f(0,0);
	fresh = true;
	interaction = false;
	view.setSize(960,480);
	view.setCenter(0,0);
}

sf::FloatRect Player::bounds() {
	return sp.getGlobalBounds();
}

bool Player::interacted() {
	if (interaction) {
		interaction = false;
		return true;
	}
	return false;
}
