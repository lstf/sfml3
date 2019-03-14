#include "player.h"

Weapon::Weapon() {
	anim.w = 32;
	anim.h = 32;
	anim.set_fps(6, 0);
	anim.set_fps(6, 1);
	anim.set_frame_count(4, 0);
	anim.set_frame_count(4, 1);
	anim.set_png(WEAPON_PNG);

	dir = 0;

	sp.setTexture(anim.tx);

	active = false;
}

void Weapon::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	w.draw(sp, states);
}

void Weapon::update() {
	if (anim.advance()) {
		sp.setTexture(anim.tx);
	}
}

void Weapon::attack() {
	if (!active) {
		active = true;
	}
}

void Weapon::set_direction(int d) {
	anim.set_animation(d);
	sp.setTexture(anim.tx);
	dir = d;
}

void Weapon::set_position(sf::Vector2f _pos) {
	sf::Vector2f offset(0,0);
	offset.x = dir == 0 ? 16 : -16;
	sp.setPosition(_pos + offset);
}

sf::FloatRect Weapon::bounds() {
	return sp.getGlobalBounds();
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
		state = WALKING_LEFT;
		weapon.set_direction(1);
		anim.set_animation(1);
		sp.setTexture(anim.tx);
	} else if (input.right) {
		state = WALKING_RIGHT;
		weapon.set_direction(0);
		anim.set_animation(0);
		sp.setTexture(anim.tx);
	}
	if (!input.left && !input.right) {
		state = STANDING;
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
			sp.setPosition(op + sf::Vector2f(
				int(diff.x*(1.0-n*.1)),
				int(diff.y*(1.0-n*.1))
			)); 
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
	if (state != STANDING) {
		if (state == WALKING_LEFT && !oldirs.left) {
			velocity.x = -1*frameTime*speed;
		} else if (state == WALKING_RIGHT && !oldirs.right) {
			velocity.x = frameTime*speed;
		}
		if (anim.advance()) {
			sp.setTexture(anim.tx);
		}
	}
	if (weapon.active) {
		weapon.update();
	}

	sp.move(velocity);
	sf::Vector2i int_pos = vfvi(sp.getPosition());
	sp.setPosition(int_pos.x, int_pos.y);

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
	weapon.set_position(pp);
	view.setCenter(sf::Vector2f(int(pp.x), int(pp.y)));
}

sf::FloatRect Player::weaponBounds() {
	return weapon.bounds();
}

bool Player::weaponActive() {
	return weapon.active;
}

void Player::write(ofstream &out) {
	int length;

	write_vec2(sp.getPosition(), out);

	length = inv.keys.size();
	write_int(length, out);
	for (auto it = inv.keys.begin(); it != inv.keys.end(); ++it) {
		it->item->write(out);
		write_int(it->count, out);
	}
}

void Player::read(ifstream &inp) {
	int length;
	int count;
	sf::Vector2f pos;

	read_vec2(pos, inp);
	setPosition(pos);

	read_int(length, inp);
	for (int i = 0; i < length; i++) {
		KeyItem* new_key = new KeyItem;

		new_key->read(inp);
		read_int(count, inp);

		inv.addItem(new_key, count);
	}
}

Player::Player() {
	anim.w = 32;
	anim.h = 48;
	anim.set_fps(12, 0);
	anim.set_fps(12, 1);
	anim.set_frame_count(6, 0);
	anim.set_frame_count(6, 1);
	anim.set_png(PLAY_PNG);

	sp.setTexture(anim.tx);

	state = STANDING;
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
