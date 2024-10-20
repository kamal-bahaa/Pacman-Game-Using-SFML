#include "menu.h"
#include<SFML/Graphics.hpp>
using namespace std;
using namespace sf;

Menu::Menu(float width, float height) 
{
	font.loadFromFile("font_play.otf");
	
	mainmenu[0].setFont(font);
	mainmenu[0].setFillColor(Color{ 255, 137, 0 } );
	mainmenu[0].setString("play");
	mainmenu[0].setCharacterSize(90);
	mainmenu[0].setPosition(Vector2f(width / (2)-115, height / (3)+40));

	mainmenu[1].setFont(font);
	mainmenu[1].setFillColor(Color::White);
	mainmenu[1].setString("instructions");
	mainmenu[1].setCharacterSize(90);
	mainmenu[1].setPosition(Vector2f(width / (2)-290, height / (3) + 240));

	mainmenu[2].setFont(font);
	mainmenu[2].setFillColor(Color::White);
	mainmenu[2].setString("Exit");
	mainmenu[2].setCharacterSize(90);
	mainmenu[2].setPosition(Vector2f(width / (2)-115, height / (3) + 440));


	selected = 0;
}

Menu::~Menu() {

}
void Menu::setSelected(int b){
	selected = b;
}

void Menu::draw(RenderWindow& window)
{
	for (int i=0; i < 3; i++) 
	{
		window.draw(mainmenu[i]);
	}
}

void Menu::MoveDown()
{
	if (selected + 1 <= 3)      // not Exit
	{
		mainmenu[selected].setFillColor(Color::White);
		selected++;
		if (selected == 3)
		{
			selected = 0;
		}
		mainmenu[selected].setFillColor(Color{ 255, 137, 0 });
	}
}

void Menu::MoveUp()
{
	if (selected - 1 >= -1)      //not play
	{
		mainmenu[selected].setFillColor(Color::White);
		selected--;
	}
	if (selected == -1) 
	{ 
		selected = 2;
	}
	mainmenu[selected].setFillColor(Color{ 255, 137, 0 });
}
	
