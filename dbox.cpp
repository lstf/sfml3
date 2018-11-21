#include "dbox.h"

void DBox::init()
{
	prev_paused = isPaused();
	pause();
	sp.setTexture(*txmap::get_tx("ats/dbg.png"));
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

DBox::DBox(DBox** p, string file)
{
	ptr = p;
	init();

	ifstream f(file.c_str());
	if (!f.is_open())
	{
		setText("Could not find text file");
		return;
	}

	string line;

	while (getline(f, line))
	{
		lines.push(line);
		lines.back() += "\n";
	}

	fillBox();	
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
	string msg;
	text.setString("");
	for (int i = 0; i < 4; i++)
	{
		if (!lines.empty())
		{
			msg += lines.front();
			lines.pop();
		}
		else
		{
			text.setString(msg);
			return;
		}
	}
	text.setString(msg);
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
		if (e.key.code == sf::Keyboard::X ||
			e.key.code == sf::Keyboard::Z)
		{
			fillBox();
		}

	}
	if (text.getString() == "")
	{
		if (!prev_paused)
		{
			unpause();
		}
		*ptr = NULL;
		delete this;
	}
}
