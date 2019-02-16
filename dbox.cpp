#include "dbox.h"

DOption* newDoption(DTree* d, DNode* n, string t, DNode* targ) {
	d->olist.push_back(new DOption);
	d->olist.back()->text = t;
	d->olist.back()->target = targ;
	n->options.push_back(d->olist.back());
	return d->olist.back();
}

DNode* newDnode(DTree* d, string t) {
	d->list.push_back(new DNode);
	d->list.back()->text = t;
	newDoption(d, d->list.back());
	return d->list.back();
}


void DBox::init()
{
	sp.setTexture(*txmap::get_tx("ats/dbg.png"));
	font.loadFromFile(ATS_DIR + string("fonts/thintel.ttf"));
	text.setFont(font);
	text.setCharacterSize(32);
	text.setFillColor(sf::Color::White);

	for (int i = 0; i < 4; i++) {
		otext[i].setFont(font);
		otext[i].setCharacterSize(32);
		otext[i].setFillColor(sf::Color::Black);
	}

	text_width = 784;
	text_height = 35;
	text_y = 324;
	text_x = 88;

	otext[0].setPosition(88,359);
	otext[1].setPosition(392,359);
	otext[2].setPosition(88,377);
	otext[3].setPosition(392,377);

	oindex = 0;

	box_width = 832;
	box_height = 128;
	box_y = 320;
	box_x = 64;

	finished = false;

	sp.setPosition(box_x, box_y);
	text.setPosition(text_x, text_y);
}

DBox::DBox(DTree* _d, Player* _player, map<string, int>* _lstate, map<string, int>* _gstate)
{
	init();

	lstate = _lstate;
	gstate = _gstate;
	player = _player;

	d = _d;

	dnode = d->root;

	fillBox();
}

void DBox::draw(sf::RenderTarget& w, sf::RenderStates states) const
{
	sf::View temp = w.getView();
	w.setView(w.getDefaultView());
	w.draw(sp, states);
	w.draw(text, states);
	for (int i = oindexmax; i >= 0; i--) {
		w.draw(otext[i], states);
	}
	w.setView(temp);
}

void DBox::fillBox()
{
	text.setString(dnode->text);
	int len = dnode->options.size();
	len = len > 4 ? 4 : len;
	for (int i = 0; i < len; i++) {
		otext[i].setString(dnode->options[i]->text);
		otext[i].setFillColor(sf::Color(127,127,127));
	}
	oindexmax = len - 1;
	oindex = 0;
	otext[oindex].setFillColor(sf::Color::White);
}

void DBox::setText(string msg)
{
	lines = queue<string>();
	lines.push(msg);
	fillBox();
}

void DBox::update(sf::Event e)
{
	if (e.type == sf::Event::KeyPressed)
	{
		if (e.key.code == sf::Keyboard::Right) {
			switch (oindex) {
			case 0:
				oindex = 1 > oindexmax ? oindex : 1;
				break;
			case 2:
				oindex = 3 > oindexmax ? oindex : 3;
				break;
			default:
				break;
			};
		} else if (e.key.code == sf::Keyboard::Left) {
			switch (oindex) {
			case 1:
				oindex = 0;
				break;
			case 3:
				oindex = 2 > oindexmax ? oindex : 2;
				break;
			default:
				break;
			};
		} else if (e.key.code == sf::Keyboard::Up) {
			switch (oindex) {
			case 2:
				oindex = 0;
				break;
			case 3:
				oindex = 1 > oindexmax ? oindex : 1;
				break;
			default:
				break;
			};
		} else if (e.key.code == sf::Keyboard::Down) {
			switch (oindex) {
			case 0:
				oindex = 2 > oindexmax ? oindex : 2;
				break;
			case 1:
				oindex = 3 > oindexmax ? oindex : 3;
				break;
			default:
				break;
			};
		} else if (e.key.code == sf::Keyboard::X ||
			e.key.code == sf::Keyboard::Z)
		{
			if (dnode->options[oindex]->target) {
				dnode = dnode->options[oindex]->target;
				fillBox();
			} else {
				finished = true;
			}
		}
		for (int i = oindexmax; i >= 0; i--) {
			otext[i].setFillColor(sf::Color(127,127,127));
		}
		otext[oindex].setFillColor(sf::Color::White);
	}
}

DBox::~DBox() {
	for (auto it = d->list.begin(); it != d->list.end(); ++it) {
		delete *it;
	}
	for (auto it = d->olist.begin(); it != d->olist.end(); ++it) {
		delete *it;
	}
	delete d;
}
