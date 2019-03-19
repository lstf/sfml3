#include "background.h"


void Background::draw(sf::RenderTarget& w, sf::RenderStates states) const
{
	sf::View temp = w.getView();
	sf::View view = Window::default_view;
	float relpos;

	relpos = temp.getCenter().x - begin;
	relpos /= end - begin;
	
	for (auto it = tx.begin(); it != tx.end(); ++it)
	{
		view.move(relpos * ((*it)->size.x - 960), 0);
		w.setView(view);
		w.draw((*it)->sprite, states);
		view = Window::default_view;
	}

	w.setView(temp);
}

Background::Background(int b, int e)
{
	sf::Vector2u size;

	begin = b + 480;
	end = e - 480;
	tx.push_back(new bg_layer);
	tx.back()->sprite.setTexture(*txmap::get_tx("ats/bgs/test0.png"));
	size = tx.back()->sprite.getTexture()->getSize();
	tx.back()->size = size;
	tx.push_back(new bg_layer);
	tx.back()->sprite.setTexture(*txmap::get_tx("ats/bgs/test1.png"));
	size = tx.back()->sprite.getTexture()->getSize();
	tx.back()->size = size;
}
