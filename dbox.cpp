#include "dbox.h"

void stringBreak(queue<string> &l, string msg)
{
	string word;

	for (auto it = msg.begin(); it != msg.end(); it++)
	{
		if (*it == ' ')
		{
			l.push(word);
			word = "";
		}
		else
		{
			word += *it;
		}
	}
	if (word != "")
	{
		l.push(word);
	}
}

DBox::DBox()
{
	tx.loadFromFile(ATS_DIR + string("dbg.png"));
	sp.setTexture(tx);
	font.loadFromFile(ATS_DIR + string("fonts/thintel.ttf"));
	text.setFont(font);
	text.setCharacterSize(32);
	text.setFillColor(sf::Color::White);

	text_width = 784;
	text_height = 70;
	text_y = 324;
	text_x = 88;

	box_width = 832;
	box_height = 128;
	box_y = 320;
	box_x = 64;

	sp.setPosition(box_x, box_y);
	text.setPosition(text_x, text_y);
}

void DBox::draw(sf::RenderTarget& w, sf::RenderStates states) const
{
	sf::View temp = w.getView();
	w.setView(w.getDefaultView());
	w.draw(sp, states);
	w.draw(text, states);
	w.setView(temp);
}

void DBox::fillBox()
{
	string prev;
	string next;
	text.setString("");
	while (!words.empty())
	{
		prev = text.getString();
		next = prev + (prev == "" ? string("") : string(" ")) + words.front();
		text.setString(next);
		if (text.getLocalBounds().width > text_width)
		{
			text.setString(prev + string("\n") + words.front());
		}
		if (text.getLocalBounds().height > text_height)
		{
			text.setString(prev);
			return;
		}
		words.pop();
	}
}

void DBox::setText(string msg)
{
	words = queue<string>();
	stringBreak(words, msg);
}
