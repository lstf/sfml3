#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#define PLAY_DIR "./ats/player/"
#define PLAY_FPS 12.0

#include "window.h"
#include "map.h"
#include "editor.h"

enum Animation
{
	RIGHT,
	LEFT
};

enum States
{
	STANDING,
	WALKING_LEFT,
	WALKING_RIGHT
};

struct CollisionBoxes
{
	sf::FloatRect down;
};

struct InputBools
{
	bool left = false;
	bool right = false;
};

class Player : public sf::Drawable
{
private:
	std::vector<sf::IntRect>* geometry;
	void* spriteSheet;
	int spriteSheetLength;
	sf::Clock animationClock;
	float animationTime;
	int animationCol;
	int animationFrameCount;
	int animationRow;
	sf::Sprite sp;
	sf::Texture tx;
	InputBools input;
	States state;
	bool stateModified;
	int speed;
	int fallA;
	float fallS;
	float fallM;
	CollisionBoxes colbox;

	void setState(States _state)
	{
		state = _state;
		stateModified = true;
	}
	
	void setAnimation(Animation a)
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
	};
	
	virtual void draw(sf::RenderTarget& w, sf::RenderStates states) const
	{
		w.draw(sp, states);
	};

public:
	void handleInput(sf::Event event)
	{
		input.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

		if (event.type == sf::Event::KeyPressed)
		{
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
			}
		}
	}

	void update(std::vector<sf::IntRect>* geo, float frameTime)
	{
		bool grounded = false;
		int	groundGeo;
		sf::IntRect downInt;
		downInt.top = colbox.down.top;
		downInt.left = colbox.down.left;
		downInt.height = colbox.down.height;
		downInt.width = colbox.down.width;
		for (int i = geo->size()-1; i >= 0; i--)
		{
			
			if (downInt.intersects((*geo)[i]))
			{
				grounded = true;
				groundGeo = i;
				break;
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
			//sf::FloatRect bounds = sp.getGlobalBounds();
			//sf::IntRect intBounds;
			//intBounds.left = (int)bounds.left;
			//intBounds.top = (int)bounds.top;
			//intBounds.width = (int)bounds.width;
			//intBounds.height = (int)bounds.height;
			////Inside floor
			//if (intBounds.intersects((*geo)[groundGeo]))
			//{
			//	sp.move((float)0, (float)(bounds.top+48 - (*geo)[groundGeo].top));
			//}
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
			else if (state == WALKING_RIGHT)
			{
				sp.move(frameTime*speed, (float)0);
				colbox.down.left += frameTime*speed;
			}
			advanceAnimation();
		}
	};
	
	void advanceAnimation()
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
	};
	

	Player()
	{
		std::ifstream inp(std::string(PLAY_DIR) + "P.PNG", std::ifstream::binary);
		if (!inp.is_open())
		{
			std::cout << "Missing asset: P.PNG" << std::endl;
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
			fallA = 10;
			fallS = 0.0;
			fallM = 200.0;
		}
	};
};

int main()
{
	sf::Font thintel;
	thintel.loadFromFile("./ats/fonts/thintel.ttf");

	w::init(thintel);
	w::window.setFramerateLimit(60);
	w::window.setKeyRepeatEnabled(false);
	w::show_fps = false;
	
	//bm:
	sf::Clock frame_clock; //for frame rate calculation
	float frameTime;
	
	//Map setup
	std::vector<Map*> maps;
	std::string map_name;
	int 		map_count;

	maps.push_back(new Map("null map"));

	std::ifstream 	maps_file(std::string(MAP_DIR) + "maps.list", std::ifstream::in);
	if (!maps_file.is_open())
	{
		std::cout << std::string("(map load) Failed to open ") + MAP_DIR + "maps.list";
		return -1;
	}

	maps_file >> map_count;
	for (int i = 0; i < map_count; i++)
	{
		maps_file >> map_name;
		
		maps.push_back(new Map(map_name));
		if (!(maps[maps.size()-1]->loadMap()))
		{
			std::cout << std::string("(map load) Failed to open ") + MAP_DIR + map_name;
		}
	}

	maps_file.close();


	Player player;


	//Editor setup
	Editor			editor(&w::window, &maps, thintel);
	w::window.setView(editor.view);

	/**************************************************************************/
	//
	//	GameLoop: gl:
	//
	//
    while (w::window.isOpen())
    {
		frameTime = frame_clock.getElapsedTime().asSeconds();
		w::frame_calc(frameTime);
		frame_clock.restart();

        sf::Event event;
        while (w::window.pollEvent(event))
        {
			//
			//	Closed
			//
            if (event.type == sf::Event::Closed)
			{
                w::window.close();
			}

			//Send event to listening classes
			editor.handleInput(event);
			player.handleInput(event);
		}	

		//updates
		player.update(maps[editor.map_index]->getGeom(), frameTime);

		//Drawing
        w::window.clear();

		w::window.setView(editor.view);

		w::window.draw(*(maps[editor.map_index]));
		w::window.draw(player);
		w::window.draw(editor);
		
		if (w::show_fps) w::window.draw(w::frame_rate);
		w::window.display();
    }

	//Store maps
	std::ofstream maps_file_out(std::string(MAP_DIR) + "maps.list", std::ofstream::out);
	if (!maps_file_out.is_open())
	{
		std::cout << std::string("(save) Failed to open ") + MAP_DIR + "maps.list\n";
		return false;
	}
	
	maps_file_out << maps.size()-1 << std::endl;
	for (int i = 1; i < maps.size(); i++)
	{
		maps_file_out << maps[i]->name << std::endl;
	}

	maps_file_out.close();

	//r:
    return 0;
}
