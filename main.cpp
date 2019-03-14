#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

#include "game.h"
#include "ui/editor.h"
#include "ui/inventoryscreen.h"
#include "ui/mainmenu.h"

using namespace std;

enum MainState {
	GAME,
	MENU,
	EDITOR,
	INVENTORY,
	LOADING
};

void load_map(Game &game, string name, sf::Vector2f pos) {
	cout << "[MAIN] clearing things" << endl;
	game.clear();
	cout << "[MAIN] setting position " << pos.x << " " << pos.y << endl;
	game.player.setPosition(pos);
	if (!game.load_map(name)) {
		cout << "[MAIN] failed to load map " << name << endl;
	}
}

int main() {
	bool quit = false;
	cout << "[MAIN] window init" << endl;
	sf::RenderWindow window(sf::VideoMode(960,480), "Game");
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);

	sf::Event event;

	cout << "[MAIN] game init" << endl;
	Game game;
	game.init();

	cout << "[MAIN] editor init" << endl;
	Editor editor(&window, &game);
	window.setView(editor.view);

	cout << "[MAIN] inventory init" << endl;
	InventoryScreen inventory(&game.player);

	cout << "[MAIN] main menu init" << endl;
	MainMenu mainmenu(&game);

	MainState state = MENU;

	cout << "[MAIN] main while" << endl;
	while (!quit && window.isOpen()) {
		game.frame_calc();		

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Tab) {
					if (state == EDITOR) {
						state = GAME;
					} else if (state == GAME) {
						state = EDITOR;
					}
				} else if (event.key.code == sf::Keyboard::Enter) {
					if (state == INVENTORY) {
						state = GAME;
					} else if (state == GAME) {
						inventory.reload_inventory();
						state = INVENTORY;
					}
				}
			}

			if (state == EDITOR) {
				editor.handle_input(event);
			} else if (state == GAME) {
				game.handle_input(event);
			} else if (state == INVENTORY) {
				inventory.handle_input(event);
			} else if (state == MENU) {
				MainMenuSelection mms = mainmenu.handle_input(event);
				if (mms == MMS_NEW_GAME) {
					state = GAME;
				} else if (mms == MMS_CONTINUE) {
					state = GAME;
				} else if (mms == MMS_OPTIONS) {
					//
				} else if (mms == MMS_QUIT) {
					quit = true;
				}
			}
		}	
		if (state == LOADING) {
			state = GAME;
			game.resetState();
		}
		if (state == EDITOR) {
			EditorTrans* et = editor.update();
			if (et) {
				if (et->is_new) {
					game.new_map(et->name);
					game.player.setPosition(et->position);
				} else {
					load_map(game, et->name, et->position);
				}
				editor.reset();
				delete et;
			}
		} else if (state == GAME) {
			GameTrans* gt = game.update();
			if (gt) {
				load_map(game, gt->name, gt->position);
				delete gt;
				state = LOADING;
				editor.reset();
				continue;
			}
		}

		window.clear();

		if (state == EDITOR) {
			window.setView(editor.view);
		} else if (state == GAME) {
			window.setView(game.player.view);
		} else if (state == MENU) {
			window.setView(window.getDefaultView());
		}

		if (state == GAME || state == EDITOR) {
			window.draw(game);
		} else if (state == INVENTORY) {
			window.draw(inventory);
		} else if (state == MENU) {
			window.draw(mainmenu);
		}

		if (state == EDITOR) {
			window.draw(editor);
		}

		window.display();
	}

	cout << "[MAIN] storing map list" << endl;

	game.save_maplist();

	return 0;
}
