#include <map>
#include <string>
#include <vector>

#include "game.h"
#include "logger.h"
#include "window.h"
#ifdef EDITOR_BUILD
#include "ui/editor.h"
#endif
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
	log_dbg("main begin");
	bool quit = false;

	log_dbg("window init");
	sf::RenderWindow window(sf::VideoMode(960,480), "Game");
	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(30);
	window.setKeyRepeatEnabled(false);
	Window::init(window.getDefaultView());
	Window::set_size(960, 480);

	Game game;
	if (!game.ok) {
		log_dbg("game failed to initialize");
		return -1;
	}

	#ifdef EDITOR_BUILD
	Editor editor(&window, &game);
	#endif

	InventoryScreen inventory(&game.player);

	MainMenu mainmenu(&game, &window);

	MainState state = MENU;

	log_dbg("main while begin");
	while (!quit && window.isOpen()) {
		sf::Event event;

		game.frame_calc();		

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Tab) {
					#ifdef EDITOR_BUILD
					if (state == EDITOR) {
						state = GAME;
					} else if (state == GAME) {
						state = EDITOR;
					}
					#endif
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
				#ifdef EDITOR_BUILD
				editor.reset();
				#endif
			}

			if (state == EDITOR) {
				#ifdef EDITOR_BUILD
				editor.handle_input(event);
				#endif
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
			#ifdef EDITOR_BUILD
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
			#endif
		} else if (state == GAME) {
			GameTrans* gt = game.update();
			if (gt) {
				game.load_map(gt->name, gt->position);
				delete gt;
				state = LOADING;
				#ifdef EDITOR_BUILD
				editor.reset();
				#endif
				continue;
			}
		}

		window.clear();

		if (state == EDITOR) {
			#ifdef EDITOR_BUILD
			window.setView(editor.view);
			#endif
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
			#ifdef EDITOR_BUILD
			window.draw(editor);
			#endif
		}

		window.display();
	}

	#ifdef EDITOR_BUILD
	game.save_maplist();
	#endif

	log_dbg("main end");
	return 0;
}
