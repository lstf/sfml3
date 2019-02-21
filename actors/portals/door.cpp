#include "door.h"

Door::Door()
{
	opening = false;
	traversed = false;
	
	Png png = txmap::get_png(DOOR_DIR + string("D.PNG"));

	sp_sheet.png = png.mem;
	sp_sheet.pngSize = png.length;

	sp_sheet.tx.loadFromMemory(sp_sheet.png, sp_sheet.pngSize, sf::IntRect(0,0,32,48));
	sp.setTexture(sp_sheet.tx);

	sp_sheet.clock.restart();
	sp_sheet.time = 0;
	sp_sheet.x = 0;
	sp_sheet.y = 0;
	sp_sheet.frameCount = 3;
	sp_sheet.fps = 6;
}

void Door::draw(sf::RenderTarget& w, sf::RenderStates states) const
{
	w.draw(sp, states);
}

bool Door::advanceAnimation()
{
	sp_sheet.time += sp_sheet.clock.getElapsedTime().asSeconds();
	if (sp_sheet.time > 1/sp_sheet.fps)
	{
		sp_sheet.time = 0;
		sp_sheet.clock.restart();
		sp_sheet.x++;
		if (sp_sheet.x >= sp_sheet.frameCount)
		{
			sp_sheet.x = 0;
			return true;
		}
		sp_sheet.tx.loadFromMemory(sp_sheet.png, sp_sheet.pngSize, sf::IntRect(sp_sheet.x*32,0,32,48));
		sp.setTexture(sp_sheet.tx);
	}
	return false;
}
sf::FloatRect Door::bounds() {
	return sp.getGlobalBounds();
}
MapTrans* Door::interact() {
	MapTrans* ret = new MapTrans;
	ret->p = (Portal*)this;
	ret->map_name = target;
	ret->position = target_pos;
	return ret;
}
sf::Vector2f Door::size() {
	sf::FloatRect r = sp.getGlobalBounds();
	return sf::Vector2f(r.width, r.height);
}

bool Door::update()
{
	return advanceAnimation();	
}
