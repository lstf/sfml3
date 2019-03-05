#ifndef _DBOX_H
#define _DBOX_H

#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include <SFML/Graphics.hpp>

#include "../actors/player.h"
#include "../inventory/inventory.h"
#include "../utils/txmap.h"


#define ATS_DIR "./ats/"

using namespace std;

struct DOption;

struct DNode;

struct DItem {
	Item* item;
	int count;
};

struct DEvent {
	string key;
	int val;
};

struct DTree {
	DNode* root;
	vector<DNode*> list;
	vector<DOption*> olist;
};

struct DNode {
	string text;
	vector<DOption*> options;
	DItem* item;
	DEvent* levent;
	DEvent* gevent;
	bool delete_ent;
	void* delete_ent_ptr;
};

struct DOption {
	string text;
	DNode* target;
};

DNode* newDnode(DTree* d, string t = "placeholder");

DOption* newDoption(DTree* d, DNode* n, string t = "...", DNode* targ = NULL);

class DBox : public sf::Drawable {
private:
	map<string, int>* lstate;
	map<string, int>* gstate;
	Player* player;
	sf::Sprite sp;
	sf::Font font;
	sf::Text text;
	sf::Text otext[4];
	int oindexmax;
	int oindex;
	queue<string> lines;
	DTree* d;
	DNode* dnode;

	int text_width;
	int text_height;
	int text_y;
	int text_x;

	int box_width;
	int box_height;
	int box_y;
	int box_x;

	bool prev_paused;

	void draw(sf::RenderTarget& w, sf::RenderStates states) const;

	void init();

public:
	bool finished;
	bool destroy_ent;
	void* destroy_ent_ptr;

	DBox(DTree* _d, Player* player, map<string, int>* _lstate, map<string,
	int>* _gstate);

	void fillBox();

	void setText(string msg);

	void update(sf::Event e);

	~DBox();
};

#endif
