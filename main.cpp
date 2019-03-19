#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

#include "game.h"
#include "window.h"
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


int main() {
	bool quit = false;
	cout << "[MAIN] window init" << endl;
	sf::RenderWindow window(sf::VideoMode(960,480), "Game");
	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(30);
	window.setKeyRepeatEnabled(false);
	Window::init(window.getDefaultView());

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
			} else if (event.type == sf::Event::KeyPressed) {
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
			} else if (event.type == sf::Event::Resized) {
				Window::set_size(event.size.width, event.size.height);
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
				} else {
					game.load_map(et->name, et->position);
				}
				editor.reset();
				delete et;
			}
		} else if (state == GAME) {
			GameTrans* gt = game.update();
			if (gt) {
				game.load_map(gt->name, gt->position);
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
			window.setView(Window::default_view);
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
