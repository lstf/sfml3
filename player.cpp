#include "player.h"

Weapon::Weapon() {
	anim.w = 32;
	anim.h = 32;
	anim.set_fps(24, 0);
	anim.set_fps(24, 1);
	anim.set_frame_count(4, 0);
	anim.set_frame_count(4, 1);
	anim.set_png(WEAPON_PNG);
	frame_counter = 0;

	dir = 0;

	sp.setTexture(anim.tx);

	active = false;
	buffered_attack = false;
}

void Weapon::draw(sf::RenderTarget& w, sf::RenderStates states) const {
	sf::Sprite sp_i = sp;
	sf::Vector2f sp_pos_f = sp.getPosition() + sf::Vector2f(0.5, 0.5);
	sp_i.setPosition((int)sp_pos_f.x, (int)sp_pos_f.y);
	w.draw(sp_i, states);
}

void Weapon::update() {
	if (anim.advance()) {
		frame_counter++;
		if (frame_counter == 4) {
			frame_counter = 0;
			if (buffered_attack) {
				buffered_attack = false;
			} else {
				active = false;
			}
		}
		sp.setTexture(anim.tx);
	}
}

void Weapon::attack() {
	if (!active) {
		anim.restart();
		active = true;
	} else {
		buffered_attack = true;
	}
}

void Weapon::set_direction(int d) {
	if (dir != d) {
		frame_counter = 0;
		active = false;
		anim.set_animation(d);
		sp.setTexture(anim.tx);
		dir = d;
	}
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
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Left) {
			input.left = true;
			input.right = false;
		} else if (event.key.code == sf::Keyboard::Right) {
			input.right = true;
			input.left = false;
		} else if (event.key.code == sf::Keyboard::Up) {
			input.up = true;
			input.down = false;
		} else if (event.key.code == sf::Keyboard::Down) {
			input.down = true;
			input.up = false;
		} else if (event.key.code == sf::Keyboard::Z) {
			if (coldirs.down && !coldirs.up) {
				input.jump = true;
			}
		} else if (event.key.code == sf::Keyboard::X) {
			weapon.attack();
		}
	} else if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::Left) {
			input.left = false;
		} else if (event.key.code == sf::Keyboard::Right) {
			input.right = false;
		} else if (event.key.code == sf::Keyboard::Up) {
			input.up = false;
		} else if (event.key.code == sf::Keyboard::Down) {
			input.down = false;
		} else if (event.key.code == sf::Keyboard::Z) {
			input.jump = false;
		}	
	}

	if (input.left && state != WALKING_LEFT) {
		state = WALKING_LEFT;
		weapon.set_direction(1);
		anim.set_animation(1);
		sp.setTexture(anim.tx);
	} else if (input.right && state != WALKING_RIGHT) {
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

	u.height = 1;
	u.top -= 1;

	r.width = 1;
	r.left = l.left + l.width;

	l.width = 1;
	l.left -= 1;

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

void Player::collisionResolver(sf::Vector2f op,
std::vector<sf::FloatRect>* geo) {
	sf::Vector2f np = sp.getPosition();
	sf::Vector2f lp = np.x < op.x ? np : op;
	sf::Vector2f rp = np.x < op.x ? op : np;

	bool vertical = np.x == op.x;

	float m; 
	float x0;
	float x1;
	float y0;
	float y1;
	float b;

	if (!vertical) {
		m = (rp - lp).y / (rp - lp).x; 
		x0 = lp.x;
		x1 = rp.x;
		y0 = lp.y;
		b = y0 - m * x0;
	} else {
		y0 = np.y < op.y ? np.y : op.y;
		y1 = np.y < op.y ? op.y : np.y;
	}

	//int n = 0;
	bool moved = false;

	//back up
	for (int j = (*geo).size()-1; j >= 0; j--) {
		sf::FloatRect gr = (*geo)[j];
		sf::FloatRect pr = sp.getGlobalBounds();
		if (gr.intersects(pr)) {
			moved = true;
			if (!vertical) {
				if (gr.left + gr.width < x1 && gr.left + gr.width > x0) {
					float x2 = gr.left + gr.width;
					sp.setPosition(x2, m * x2 + b);
				} else if (gr.left < x1 + pr.width && gr.left > x0 + pr.width) {
					float x2 = gr.left - pr.width;
					sp.setPosition(x2, m * x2 + b);
				} else {
					float y2;
					y0 = np.y < op.y ? np.y : op.y;
					y1 = np.y < op.y ? op.y : np.y;
					if (gr.top + gr.height < y1) {
						y2 = gr.top + gr.height;
					} else {
						y2 = gr.top - pr.height;
					}
					velocity.y = 0;
					sp.setPosition((y2 - b) / m, y2);
				}
			} else {
				float y2;
				if (gr.top + gr.height < y1) {
					y2 = gr.top + gr.height;
				} else {
					y2 = gr.top - pr.height;
				}
				velocity.y = 0;
				sp.setPosition(np.x, y2);
			}
		}
	}

	updateCollide(geo);

	//if the player is left floating on corner, push up
	if (moved && !coldirs.up && !coldirs.down && !coldirs.right && 
	!coldirs.left) {
		velocity = sf::Vector2f(velocity.x, 0);
		sp.move(velocity + sf::Vector2f(0, -1));
	}
}


void Player::update(std::vector<sf::FloatRect>* geo, double frameTime) {
	if (fresh) {
		updateCollide(geo);
		fresh = false;
	}

	sf::Vector2f old_position = sp.getPosition();

	velocity.x = 0;
	
	if (!coldirs.down) {
		velocity.y += frameTime*frameTime*fallA;
		velocity.y = velocity.y > fallM*frameTime ? fallM*frameTime : velocity.y;
	} else {
		if (input.jump) {
		//	max_jump = 109882;
			velocity.y = jumph*frameTime;
			input.jump = false;
		} 
	}

	//TODO jump height varies by a few pixels
	//if (velocity.y > 0 && !coldirs.down) {
	//	float c_y = sp.getPosition().y;
	//	if (c_y < max_jump) {
	//		max_jump = c_y;
	//		cout << c_y << endl;
	//	}
	//}
	if (state != STANDING) {
		if (state == WALKING_LEFT && !coldirs.left) {
			velocity.x = -1*frameTime*speed;
		} else if (state == WALKING_RIGHT && !coldirs.right) {
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

	collisionResolver(old_position, geo);

	refresh();
}

void Player::refresh() {
	sf::Vector2f pp = sp.getPosition();
	sf::FloatRect pb = sp.getGlobalBounds();
	weapon.set_position(pp);
	view = Window::default_view;
	view.setCenter(sf::Vector2f(
		int(pp.x + 0.5 + pb.width / 2),
		int(pp.y + 0.5 + pb.height / 2)
	));
	
}

void Player::reset_input() {
	state = STANDING;
	input.left = false;
	input.right = false;
	input.up = false;
	input.down = false;
	input.jump = false;
	interaction = false;
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
	view = Window::default_view;
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
