#ifndef DBOX_O
#define DBOX_O

#include <queue>
#include <string>

#include <SFML/Graphics.hpp>

#define ATS_DIR "./ats/"

using namespace std;

class DBox : public sf::Drawable
{
private:
	sf::Sprite sp;
	sf::Texture tx;
	sf::Font font;
	sf::Text text;
	queue<string> words;

	int text_width;
	int text_height;
	int text_y;
	int text_x;

	int box_width;
	int box_height;
	int box_y;
	int box_x;

	void draw(sf::RenderTarget& w, sf::RenderStates states) const;

public:
	DBox();
	void fillBox();
	void setText(string msg);
};

#endif
