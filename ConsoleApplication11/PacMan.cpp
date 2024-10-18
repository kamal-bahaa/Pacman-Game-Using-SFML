#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <time.h>
#include <vector>
#include <random>
#include <iomanip>
#include <string>
#include <SFML/audio.hpp>
#include <SFML/graphics.hpp>
#include <SFML/window.hpp>
#include <SFML/system.hpp>
#include "menu.h"
using namespace std;
using namespace sf;
struct sounds {
	Sound sound;
	SoundBuffer buffer;
}Beginning_sound, Eating_sound, PacmanDeath, Sound_big_eating;
RenderWindow window(VideoMode(684, 900), "PacMan!", Style::Close || Style::Titlebar || Style::Fullscreen);
RectangleShape Area(Vector2f(160, 60));
RectangleShape gate(Vector2f(36, 10));
RectangleShape sprite(Vector2f(36, 40));
RectangleShape pacman_textures[2];
RectangleShape boardrect[20][19];
RectangleShape eating_rects[20][19];
CircleShape Big_eating_circles[4];
RectangleShape Ghost[4], Ghost_hmbrgr[4];
RectangleShape ready(Vector2f(36 * 3, 40));
RectangleShape game_over(Vector2f(36 * 3, 40));
Texture pacman_SpriteSheet;
bool intersection_of_pacman_with_big_food, pauseIntersection = false, pausePress = false;
const int Velocity = 4;
int _score = 0, highScore = 0;
Text Score;
int n = 1, z = 1, r = 0, direction = 1, f = 1, k = 0, Direction_of_ghosts[4]{ rand() % 2,rand() % 2,rand() % 2,rand() % 2 }, Random, q = 0, i = 0, u = 0, ghost_start = 0, ana_zhgt[4]{};
int pagenum = 1000;
bool Ghost_num[4]{};
Menu menu(684, 900);
Texture minu_B;
Clock clock_of_ghosts, Clock_of_hmbrgr;

void pause_play(RectangleShape& rec, RectangleShape& pause, int& j, int& w, int x, int i, Mouse& ms, bool& intersection, bool& pressed, RectangleShape& sound) {
	while (true) {
		pauseIntersection = false;
		pausePress = false;
		pause.setSize(Vector2f(60, 60));
		rec.setPosition(ms.getPosition().x - 630, ms.getPosition().y - 130);
		if (rec.getGlobalBounds().intersects(pause.getGlobalBounds()))
			pauseIntersection = true;
		else
			pause.setSize(Vector2f(60, 60));
		if ((Mouse::isButtonPressed(Mouse::Left) && (pauseIntersection)))
			pausePress = true;
		if (pauseIntersection) {
			pause.setSize(Vector2f(70, 70));
			if (pausePress) {
				pause.setSize(Vector2f(60, 60));
				if (j != 0) {
					w += 1;
					w %= 2;
					j = 0;
				}
			}
			else {
				j++;
			}
		}
		window.draw(pause);
		if (w == 0) {
			break;
		}
	}
}

RectangleShape Main_Menu() {
	minu_B.loadFromFile("minu.jpg");
	RectangleShape BG;
	BG.setTexture(&minu_B);
	BG.setPosition(0, 0);
	BG.setSize(Vector2f(684, 300));
	return BG;
}
void PacMan(Texture& pacman) {
	pacman.loadFromFile("pacman16.png");
	sprite.setTexture(&pacman);
	sprite.setPosition(336, 100 + 10 * 40);
	sprite.setTextureRect(IntRect(0, 0, 16, 16));
}
void Pacman_lives(Texture& Pacman) {
	for (int i = 0; i < 2; i++)
	{
		pacman_textures[i].setTexture(&Pacman);
		pacman_textures[i].setSize(Vector2f(50, 50));
		pacman_textures[0].setPosition(500, 25);
		pacman_textures[1].setPosition(450, 25);
		window.draw(pacman_textures[i]);
	}
}
void Ready(Texture& readyy) {
	ready.setTexture(&readyy);
	ready.setPosition(boardrect[14][8].getPosition());
	window.draw(ready);
}
void Ghosts() {

	Texture ghost_texture;
	ghost_texture.loadFromFile("image.png");
	for (int i = 0; i < 4; i++) {
		Ghost[i].setTexture(&ghost_texture);
		Ghost[i].setSize(Vector2f(36, 40));

		if (k == 0) {
			Ghost[i].setTextureRect(IntRect(0, (float)i * 78.25, 80.25, 78));
			Ghost[0].setPosition(boardrect[7][9].getPosition());
			Ghost[1].setPosition(boardrect[9][8].getPosition().x, boardrect[9][8].getPosition().y - 10);
			Ghost[2].setPosition(boardrect[9][9].getPosition().x, boardrect[9][9].getPosition().y - 10);
			Ghost[3].setPosition(boardrect[9][10].getPosition().x, boardrect[9][10].getPosition().y - 10);
			k++;
		}
	}
	for (int i = 0; i < 4; i++)
		window.draw(Ghost[i]);
}
void Ghosts_Begin() {
	if (k == 1)
	{
		for (int i = 1; i < 4; i++) {
			if (clock_of_ghosts.getElapsedTime().asSeconds() < 0.25 * (ghost_start + 1))
				if (ghost_start % 2 == 0)
					Ghost[i].move(0, -1.5);
				else
					Ghost[i].move(0, 1.5);
			else
				ghost_start++;
		}
	}
	else if (k == 2)
	{
		if (clock_of_ghosts.getElapsedTime().asSeconds() < 0.25 * (ghost_start + 1))
		{
			if (ghost_start % 2 == 0)
				Ghost[1].move(0, -1.5);
			else
				Ghost[1].move(0, 1.5);
			if (ghost_start % 2 == 0)
				Ghost[3].move(0, -1.5);
			else
				Ghost[3].move(0, 1.5);
		}
		else
			ghost_start++;
	}
	else if (k == 3)
	{
		if (clock_of_ghosts.getElapsedTime().asSeconds() < 0.25 * (ghost_start + 1))
		{
			if (ghost_start % 2 == 0)
				Ghost[3].move(0, -1.5);
			else
				Ghost[3].move(0, 1.5);
		}
		else
			ghost_start++;
	}
	if (clock_of_ghosts.getElapsedTime().asSeconds() > 5 && k == 1)
	{
		Ghost[2].setPosition(boardrect[7][9].getPosition()); k++;
	}
	else if (clock_of_ghosts.getElapsedTime().asSeconds() > 10 && k == 2)
	{
		Ghost[1].setPosition(boardrect[7][9].getPosition()); k++;
	}
	else if (clock_of_ghosts.getElapsedTime().asSeconds() > 15 && k == 3)
	{
		Ghost[3].setPosition(boardrect[7][9].getPosition()); k++;
	}
}
void Inter_Sections() {
	if (sprite.getPosition() == boardrect[10][12].getPosition() || sprite.getPosition() == boardrect[8][12].getPosition() || sprite.getPosition() == boardrect[7][12].getPosition() || sprite.getPosition() == boardrect[8][6].getPosition() || sprite.getPosition() == boardrect[7][6].getPosition() || sprite.getPosition() == boardrect[10][6].getPosition() || sprite.getPosition() == boardrect[12][12].getPosition() || sprite.getPosition() == boardrect[12][10].getPosition()
		|| sprite.getPosition() == boardrect[14][10].getPosition() || sprite.getPosition() == boardrect[14][8].getPosition() || sprite.getPosition() == boardrect[12][8].getPosition() || sprite.getPosition() == boardrect[12][6].getPosition() || sprite.getPosition() == boardrect[12][4].getPosition() || sprite.getPosition() == boardrect[14][1].getPosition() || sprite.getPosition() == boardrect[16][1].getPosition() || sprite.getPosition() == boardrect[16][2].getPosition()
		|| sprite.getPosition() == boardrect[18][1].getPosition() || sprite.getPosition() == boardrect[12][10].getPosition() || sprite.getPosition() == boardrect[19][18].getPosition() || sprite.getPosition() == boardrect[16][4].getPosition() || sprite.getPosition() == boardrect[14][4].getPosition() || sprite.getPosition() == boardrect[14][2].getPosition()
		|| sprite.getPosition() == boardrect[12][1].getPosition() || sprite.getPosition() == boardrect[1][1].getPosition() || sprite.getPosition() == boardrect[3][1].getPosition() || sprite.getPosition() == boardrect[5][1].getPosition() || sprite.getPosition() == boardrect[1][4].getPosition() || sprite.getPosition() == boardrect[3][4].getPosition() || sprite.getPosition() == boardrect[5][4].getPosition()
		|| sprite.getPosition() == boardrect[1][17].getPosition() || sprite.getPosition() == boardrect[3][17].getPosition() || sprite.getPosition() == boardrect[5][17].getPosition() || sprite.getPosition() == boardrect[1][14].getPosition() || sprite.getPosition() == boardrect[3][14].getPosition() || sprite.getPosition() == boardrect[5][14].getPosition() || sprite.getPosition() == boardrect[1][10].getPosition() || sprite.getPosition() == boardrect[1][8].getPosition()
		|| sprite.getPosition() == boardrect[3][8].getPosition() || sprite.getPosition() == boardrect[3][10].getPosition() || sprite.getPosition() == boardrect[3][6].getPosition() || sprite.getPosition() == boardrect[3][12].getPosition() || sprite.getPosition() == boardrect[5][6].getPosition() || sprite.getPosition() == boardrect[5][12].getPosition() || sprite.getPosition() == boardrect[5][10].getPosition() || sprite.getPosition() == boardrect[5][8].getPosition()
		|| sprite.getPosition() == boardrect[7][8].getPosition() || sprite.getPosition() == boardrect[7][10].getPosition() || sprite.getPosition() == boardrect[8][4].getPosition() || sprite.getPosition() == boardrect[14][14].getPosition() || sprite.getPosition() == boardrect[16][14].getPosition() || sprite.getPosition() == boardrect[14][12].getPosition() || sprite.getPosition() == boardrect[16][12].getPosition() || sprite.getPosition() == boardrect[16][10].getPosition()
		|| sprite.getPosition() == boardrect[18][10].getPosition() || sprite.getPosition() == boardrect[18][8].getPosition() || sprite.getPosition() == boardrect[16][8].getPosition() || sprite.getPosition() == boardrect[16][6].getPosition() || sprite.getPosition() == boardrect[14][6].getPosition() || sprite.getPosition() == boardrect[12][14].getPosition() || sprite.getPosition() == boardrect[16][16].getPosition() || sprite.getPosition() == boardrect[8][14].getPosition()
		|| sprite.getPosition() == boardrect[14][17].getPosition() || sprite.getPosition() == boardrect[14][17].getPosition() || sprite.getPosition() == boardrect[12][17].getPosition() || sprite.getPosition() == boardrect[16][17].getPosition() || sprite.getPosition() == boardrect[18][17].getPosition() || sprite.getPosition() == boardrect[14][16].getPosition())
		z = direction;
}
void Background()
{
	r = 0;
	for (int i = 0; i < 4; i++)
		Ghost_num[i] = false;
	bool map[20][19] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
					   {1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
					   {1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,0,1},{1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1},
					   {1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1},{1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1},
					   {0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},{1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1},
					   {1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1},{1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1},
					   {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},{1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},
					   {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},{1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1},
					   {1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1},{1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1},
					   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			boardrect[i][j].setSize(Vector2f(36, 40));
			boardrect[i][j].setPosition(36 * j, 100 + 40 * i);
			if (map[i][j] == 1)
			{
				boardrect[i][j].setFillColor(Color(0, 0, 150));
				window.draw(boardrect[i][j]);
				if (sprite.getGlobalBounds().intersects(boardrect[i][j].getGlobalBounds()))
					r = 1;
				if (Ghost[0].getGlobalBounds().intersects(boardrect[i][j].getGlobalBounds()))
					Ghost_num[0] = true;
				if (Ghost[1].getGlobalBounds().intersects(boardrect[i][j].getGlobalBounds()))
					Ghost_num[1] = true;
				if (Ghost[2].getGlobalBounds().intersects(boardrect[i][j].getGlobalBounds()))
					Ghost_num[2] = true;
				if (Ghost[3].getGlobalBounds().intersects(boardrect[i][j].getGlobalBounds()))
					Ghost_num[3] = true;
			}
		}
	}
}
void Ghosts_movement() {
	for (int i = 0; i < 4; i++) {
		if (Ghost[i].getPosition().x >= 684)
			Ghost[i].setPosition(-36, Ghost[i].getPosition().y);
		else if (Ghost[i].getPosition().x <= -36)
			Ghost[i].setPosition(684, Ghost[i].getPosition().y);
		switch (Direction_of_ghosts[i]) {
		case 0: {
			Ghost[i].setTextureRect(IntRect(0, i * 78.25, 80.25, 78));
			Ghost[i].move(3 / 2 * Velocity, 0);
			Background();  if (Ghost_num[i] == true) { Ghost[i].move(-3 / 2 * Velocity, 0); }
			if (Ghost[i].getPosition() == boardrect[10][12].getPosition() || Ghost[i].getPosition() == boardrect[8][12].getPosition() || Ghost[i].getPosition() == boardrect[7][12].getPosition() || Ghost[i].getPosition() == boardrect[8][6].getPosition() || Ghost[i].getPosition() == boardrect[7][6].getPosition() || Ghost[i].getPosition() == boardrect[10][6].getPosition() || Ghost[i].getPosition() == boardrect[12][12].getPosition() || Ghost[i].getPosition() == boardrect[12][10].getPosition()
				|| Ghost[i].getPosition() == boardrect[14][10].getPosition() || Ghost[i].getPosition() == boardrect[14][8].getPosition() || Ghost[i].getPosition() == boardrect[12][8].getPosition() || Ghost[i].getPosition() == boardrect[12][6].getPosition() || Ghost[i].getPosition() == boardrect[12][4].getPosition() || Ghost[i].getPosition() == boardrect[14][1].getPosition() || Ghost[i].getPosition() == boardrect[16][1].getPosition() || Ghost[i].getPosition() == boardrect[16][2].getPosition()
				|| Ghost[i].getPosition() == boardrect[18][1].getPosition() || Ghost[i].getPosition() == boardrect[12][10].getPosition() || Ghost[i].getPosition() == boardrect[19][18].getPosition() || Ghost[i].getPosition() == boardrect[16][4].getPosition() || Ghost[i].getPosition() == boardrect[14][4].getPosition() || Ghost[i].getPosition() == boardrect[14][2].getPosition()
				|| Ghost[i].getPosition() == boardrect[12][1].getPosition() || Ghost[i].getPosition() == boardrect[1][1].getPosition() || Ghost[i].getPosition() == boardrect[3][1].getPosition() || Ghost[i].getPosition() == boardrect[5][1].getPosition() || Ghost[i].getPosition() == boardrect[1][4].getPosition() || Ghost[i].getPosition() == boardrect[3][4].getPosition() || Ghost[i].getPosition() == boardrect[5][4].getPosition()
				|| Ghost[i].getPosition() == boardrect[1][17].getPosition() || Ghost[i].getPosition() == boardrect[3][17].getPosition() || Ghost[i].getPosition() == boardrect[5][17].getPosition() || Ghost[i].getPosition() == boardrect[1][14].getPosition() || Ghost[i].getPosition() == boardrect[3][14].getPosition() || Ghost[i].getPosition() == boardrect[5][14].getPosition() || Ghost[i].getPosition() == boardrect[1][10].getPosition() || Ghost[i].getPosition() == boardrect[1][8].getPosition()
				|| Ghost[i].getPosition() == boardrect[3][8].getPosition() || Ghost[i].getPosition() == boardrect[3][10].getPosition() || Ghost[i].getPosition() == boardrect[3][6].getPosition() || Ghost[i].getPosition() == boardrect[3][12].getPosition() || Ghost[i].getPosition() == boardrect[5][6].getPosition() || Ghost[i].getPosition() == boardrect[5][12].getPosition() || Ghost[i].getPosition() == boardrect[5][10].getPosition() || Ghost[i].getPosition() == boardrect[5][8].getPosition()
				|| Ghost[i].getPosition() == boardrect[7][8].getPosition() || Ghost[i].getPosition() == boardrect[7][10].getPosition() || Ghost[i].getPosition() == boardrect[8][4].getPosition() || Ghost[i].getPosition() == boardrect[14][14].getPosition() || Ghost[i].getPosition() == boardrect[16][14].getPosition() || Ghost[i].getPosition() == boardrect[14][12].getPosition() || Ghost[i].getPosition() == boardrect[16][12].getPosition() || Ghost[i].getPosition() == boardrect[16][10].getPosition()
				|| Ghost[i].getPosition() == boardrect[18][10].getPosition() || Ghost[i].getPosition() == boardrect[18][8].getPosition() || Ghost[i].getPosition() == boardrect[16][8].getPosition() || Ghost[i].getPosition() == boardrect[16][6].getPosition() || Ghost[i].getPosition() == boardrect[14][6].getPosition() || Ghost[i].getPosition() == boardrect[12][14].getPosition() || Ghost[i].getPosition() == boardrect[16][16].getPosition() || Ghost[i].getPosition() == boardrect[8][14].getPosition()
				|| Ghost[i].getPosition() == boardrect[14][17].getPosition() || Ghost[i].getPosition() == boardrect[14][17].getPosition() || Ghost[i].getPosition() == boardrect[12][17].getPosition() || Ghost[i].getPosition() == boardrect[16][17].getPosition() || Ghost[i].getPosition() == boardrect[18][17].getPosition() || Ghost[i].getPosition() == boardrect[14][16].getPosition())
				Direction_of_ghosts[i] = rand() % 4;
			if (Direction_of_ghosts[i] == 1)
				Direction_of_ghosts[i] = 0;
			break;
		}
		case 1: {
			Ghost[i].setTextureRect(IntRect(2 * 80.25, i * 78.25, 80.25, 78));
			Ghost[i].move(-3 / 2 * Velocity, 0);
			Background();  if (Ghost_num[i] == true) { Ghost[i].move(3 / 2 * Velocity, 0); }
			if (Ghost[i].getPosition() == boardrect[10][12].getPosition() || Ghost[i].getPosition() == boardrect[8][12].getPosition() || Ghost[i].getPosition() == boardrect[7][12].getPosition() || Ghost[i].getPosition() == boardrect[8][6].getPosition() || Ghost[i].getPosition() == boardrect[7][6].getPosition() || Ghost[i].getPosition() == boardrect[10][6].getPosition() || Ghost[i].getPosition() == boardrect[12][12].getPosition() || Ghost[i].getPosition() == boardrect[12][10].getPosition()
				|| Ghost[i].getPosition() == boardrect[14][10].getPosition() || Ghost[i].getPosition() == boardrect[14][8].getPosition() || Ghost[i].getPosition() == boardrect[12][8].getPosition() || Ghost[i].getPosition() == boardrect[12][6].getPosition() || Ghost[i].getPosition() == boardrect[12][4].getPosition() || Ghost[i].getPosition() == boardrect[14][1].getPosition() || Ghost[i].getPosition() == boardrect[16][1].getPosition() || Ghost[i].getPosition() == boardrect[16][2].getPosition()
				|| Ghost[i].getPosition() == boardrect[18][1].getPosition() || Ghost[i].getPosition() == boardrect[12][10].getPosition() || Ghost[i].getPosition() == boardrect[19][18].getPosition() || Ghost[i].getPosition() == boardrect[16][4].getPosition() || Ghost[i].getPosition() == boardrect[14][4].getPosition() || Ghost[i].getPosition() == boardrect[14][2].getPosition()
				|| Ghost[i].getPosition() == boardrect[12][1].getPosition() || Ghost[i].getPosition() == boardrect[1][1].getPosition() || Ghost[i].getPosition() == boardrect[3][1].getPosition() || Ghost[i].getPosition() == boardrect[5][1].getPosition() || Ghost[i].getPosition() == boardrect[1][4].getPosition() || Ghost[i].getPosition() == boardrect[3][4].getPosition() || Ghost[i].getPosition() == boardrect[5][4].getPosition()
				|| Ghost[i].getPosition() == boardrect[1][17].getPosition() || Ghost[i].getPosition() == boardrect[3][17].getPosition() || Ghost[i].getPosition() == boardrect[5][17].getPosition() || Ghost[i].getPosition() == boardrect[1][14].getPosition() || Ghost[i].getPosition() == boardrect[3][14].getPosition() || Ghost[i].getPosition() == boardrect[5][14].getPosition() || Ghost[i].getPosition() == boardrect[1][10].getPosition() || Ghost[i].getPosition() == boardrect[1][8].getPosition()
				|| Ghost[i].getPosition() == boardrect[3][8].getPosition() || Ghost[i].getPosition() == boardrect[3][10].getPosition() || Ghost[i].getPosition() == boardrect[3][6].getPosition() || Ghost[i].getPosition() == boardrect[3][12].getPosition() || Ghost[i].getPosition() == boardrect[5][6].getPosition() || Ghost[i].getPosition() == boardrect[5][12].getPosition() || Ghost[i].getPosition() == boardrect[5][10].getPosition() || Ghost[i].getPosition() == boardrect[5][8].getPosition()
				|| Ghost[i].getPosition() == boardrect[7][8].getPosition() || Ghost[i].getPosition() == boardrect[7][10].getPosition() || Ghost[i].getPosition() == boardrect[8][4].getPosition() || Ghost[i].getPosition() == boardrect[14][14].getPosition() || Ghost[i].getPosition() == boardrect[16][14].getPosition() || Ghost[i].getPosition() == boardrect[14][12].getPosition() || Ghost[i].getPosition() == boardrect[16][12].getPosition() || Ghost[i].getPosition() == boardrect[16][10].getPosition()
				|| Ghost[i].getPosition() == boardrect[18][10].getPosition() || Ghost[i].getPosition() == boardrect[18][8].getPosition() || Ghost[i].getPosition() == boardrect[16][8].getPosition() || Ghost[i].getPosition() == boardrect[16][6].getPosition() || Ghost[i].getPosition() == boardrect[14][6].getPosition() || Ghost[i].getPosition() == boardrect[12][14].getPosition() || Ghost[i].getPosition() == boardrect[16][16].getPosition() || Ghost[i].getPosition() == boardrect[8][14].getPosition()
				|| Ghost[i].getPosition() == boardrect[14][17].getPosition() || Ghost[i].getPosition() == boardrect[14][17].getPosition() || Ghost[i].getPosition() == boardrect[12][17].getPosition() || Ghost[i].getPosition() == boardrect[16][17].getPosition() || Ghost[i].getPosition() == boardrect[18][17].getPosition() || Ghost[i].getPosition() == boardrect[14][16].getPosition())
				Direction_of_ghosts[i] = rand() % 3 + 1;
			break;
		}
		case 2: {
			Ghost[i].setTextureRect(IntRect(7 * 80.25, i * 78.25, 80.25, 78));
			Ghost[i].move(0, -3 / 2 * Velocity);
			Background();  if (Ghost_num[i] == true) { Ghost[i].move(0, 3 / 2 * Velocity); }
			if (Ghost[i].getPosition() == boardrect[10][12].getPosition() || Ghost[i].getPosition() == boardrect[8][12].getPosition() || Ghost[i].getPosition() == boardrect[7][12].getPosition() || Ghost[i].getPosition() == boardrect[8][6].getPosition() || Ghost[i].getPosition() == boardrect[7][6].getPosition() || Ghost[i].getPosition() == boardrect[10][6].getPosition() || Ghost[i].getPosition() == boardrect[12][12].getPosition() || Ghost[i].getPosition() == boardrect[12][10].getPosition()
				|| Ghost[i].getPosition() == boardrect[14][10].getPosition() || Ghost[i].getPosition() == boardrect[14][8].getPosition() || Ghost[i].getPosition() == boardrect[12][8].getPosition() || Ghost[i].getPosition() == boardrect[12][6].getPosition() || Ghost[i].getPosition() == boardrect[12][4].getPosition() || Ghost[i].getPosition() == boardrect[14][1].getPosition() || Ghost[i].getPosition() == boardrect[16][1].getPosition() || Ghost[i].getPosition() == boardrect[16][2].getPosition()
				|| Ghost[i].getPosition() == boardrect[18][1].getPosition() || Ghost[i].getPosition() == boardrect[12][10].getPosition() || Ghost[i].getPosition() == boardrect[19][18].getPosition() || Ghost[i].getPosition() == boardrect[16][4].getPosition() || Ghost[i].getPosition() == boardrect[14][4].getPosition() || Ghost[i].getPosition() == boardrect[14][2].getPosition()
				|| Ghost[i].getPosition() == boardrect[12][1].getPosition() || Ghost[i].getPosition() == boardrect[1][1].getPosition() || Ghost[i].getPosition() == boardrect[3][1].getPosition() || Ghost[i].getPosition() == boardrect[5][1].getPosition() || Ghost[i].getPosition() == boardrect[1][4].getPosition() || Ghost[i].getPosition() == boardrect[3][4].getPosition() || Ghost[i].getPosition() == boardrect[5][4].getPosition()
				|| Ghost[i].getPosition() == boardrect[1][17].getPosition() || Ghost[i].getPosition() == boardrect[3][17].getPosition() || Ghost[i].getPosition() == boardrect[5][17].getPosition() || Ghost[i].getPosition() == boardrect[1][14].getPosition() || Ghost[i].getPosition() == boardrect[3][14].getPosition() || Ghost[i].getPosition() == boardrect[5][14].getPosition() || Ghost[i].getPosition() == boardrect[1][10].getPosition() || Ghost[i].getPosition() == boardrect[1][8].getPosition()
				|| Ghost[i].getPosition() == boardrect[3][8].getPosition() || Ghost[i].getPosition() == boardrect[3][10].getPosition() || Ghost[i].getPosition() == boardrect[3][6].getPosition() || Ghost[i].getPosition() == boardrect[3][12].getPosition() || Ghost[i].getPosition() == boardrect[5][6].getPosition() || Ghost[i].getPosition() == boardrect[5][12].getPosition() || Ghost[i].getPosition() == boardrect[5][10].getPosition() || Ghost[i].getPosition() == boardrect[5][8].getPosition()
				|| Ghost[i].getPosition() == boardrect[7][8].getPosition() || Ghost[i].getPosition() == boardrect[7][10].getPosition() || Ghost[i].getPosition() == boardrect[8][4].getPosition() || Ghost[i].getPosition() == boardrect[14][14].getPosition() || Ghost[i].getPosition() == boardrect[16][14].getPosition() || Ghost[i].getPosition() == boardrect[14][12].getPosition() || Ghost[i].getPosition() == boardrect[16][12].getPosition() || Ghost[i].getPosition() == boardrect[16][10].getPosition()
				|| Ghost[i].getPosition() == boardrect[18][10].getPosition() || Ghost[i].getPosition() == boardrect[18][8].getPosition() || Ghost[i].getPosition() == boardrect[16][8].getPosition() || Ghost[i].getPosition() == boardrect[16][6].getPosition() || Ghost[i].getPosition() == boardrect[14][6].getPosition() || Ghost[i].getPosition() == boardrect[12][14].getPosition() || Ghost[i].getPosition() == boardrect[16][16].getPosition() || Ghost[i].getPosition() == boardrect[8][14].getPosition()
				|| Ghost[i].getPosition() == boardrect[14][17].getPosition() || Ghost[i].getPosition() == boardrect[14][17].getPosition() || Ghost[i].getPosition() == boardrect[12][17].getPosition() || Ghost[i].getPosition() == boardrect[16][17].getPosition() || Ghost[i].getPosition() == boardrect[18][17].getPosition() || Ghost[i].getPosition() == boardrect[14][16].getPosition())
				Direction_of_ghosts[i] = rand() % 3;
			break;
		}
		case 3: {
			Ghost[i].setTextureRect(IntRect(4 * 80.25, i * 78.25, 80.25, 78));
			Ghost[i].move(0, 3 / 2 * Velocity);
			Background();  if (Ghost_num[i] == true) { Ghost[i].move(0, 3 / 2 * -Velocity); }
			if (Ghost[i].getPosition() == boardrect[10][12].getPosition() || Ghost[i].getPosition() == boardrect[8][12].getPosition() || Ghost[i].getPosition() == boardrect[7][12].getPosition() || Ghost[i].getPosition() == boardrect[8][6].getPosition() || Ghost[i].getPosition() == boardrect[7][6].getPosition() || Ghost[i].getPosition() == boardrect[10][6].getPosition() || Ghost[i].getPosition() == boardrect[12][12].getPosition() || Ghost[i].getPosition() == boardrect[12][10].getPosition()
				|| Ghost[i].getPosition() == boardrect[14][10].getPosition() || Ghost[i].getPosition() == boardrect[14][8].getPosition() || Ghost[i].getPosition() == boardrect[12][8].getPosition() || Ghost[i].getPosition() == boardrect[12][6].getPosition() || Ghost[i].getPosition() == boardrect[12][4].getPosition() || Ghost[i].getPosition() == boardrect[14][1].getPosition() || Ghost[i].getPosition() == boardrect[16][1].getPosition() || Ghost[i].getPosition() == boardrect[16][2].getPosition()
				|| Ghost[i].getPosition() == boardrect[18][1].getPosition() || Ghost[i].getPosition() == boardrect[12][10].getPosition() || Ghost[i].getPosition() == boardrect[19][18].getPosition() || Ghost[i].getPosition() == boardrect[16][4].getPosition() || Ghost[i].getPosition() == boardrect[14][4].getPosition() || Ghost[i].getPosition() == boardrect[14][2].getPosition()
				|| Ghost[i].getPosition() == boardrect[12][1].getPosition() || Ghost[i].getPosition() == boardrect[1][1].getPosition() || Ghost[i].getPosition() == boardrect[3][1].getPosition() || Ghost[i].getPosition() == boardrect[5][1].getPosition() || Ghost[i].getPosition() == boardrect[1][4].getPosition() || Ghost[i].getPosition() == boardrect[3][4].getPosition() || Ghost[i].getPosition() == boardrect[5][4].getPosition()
				|| Ghost[i].getPosition() == boardrect[1][17].getPosition() || Ghost[i].getPosition() == boardrect[3][17].getPosition() || Ghost[i].getPosition() == boardrect[5][17].getPosition() || Ghost[i].getPosition() == boardrect[1][14].getPosition() || Ghost[i].getPosition() == boardrect[3][14].getPosition() || Ghost[i].getPosition() == boardrect[5][14].getPosition() || Ghost[i].getPosition() == boardrect[1][10].getPosition() || Ghost[i].getPosition() == boardrect[1][8].getPosition()
				|| Ghost[i].getPosition() == boardrect[3][8].getPosition() || Ghost[i].getPosition() == boardrect[3][10].getPosition() || Ghost[i].getPosition() == boardrect[3][6].getPosition() || Ghost[i].getPosition() == boardrect[3][12].getPosition() || Ghost[i].getPosition() == boardrect[5][6].getPosition() || Ghost[i].getPosition() == boardrect[5][12].getPosition() || Ghost[i].getPosition() == boardrect[5][10].getPosition() || Ghost[i].getPosition() == boardrect[5][8].getPosition()
				|| Ghost[i].getPosition() == boardrect[7][8].getPosition() || Ghost[i].getPosition() == boardrect[7][10].getPosition() || Ghost[i].getPosition() == boardrect[8][4].getPosition() || Ghost[i].getPosition() == boardrect[14][14].getPosition() || Ghost[i].getPosition() == boardrect[16][14].getPosition() || Ghost[i].getPosition() == boardrect[14][12].getPosition() || Ghost[i].getPosition() == boardrect[16][12].getPosition() || Ghost[i].getPosition() == boardrect[16][10].getPosition()
				|| Ghost[i].getPosition() == boardrect[18][10].getPosition() || Ghost[i].getPosition() == boardrect[18][8].getPosition() || Ghost[i].getPosition() == boardrect[16][8].getPosition() || Ghost[i].getPosition() == boardrect[16][6].getPosition() || Ghost[i].getPosition() == boardrect[14][6].getPosition() || Ghost[i].getPosition() == boardrect[12][14].getPosition() || Ghost[i].getPosition() == boardrect[16][16].getPosition() || Ghost[i].getPosition() == boardrect[8][14].getPosition()
				|| Ghost[i].getPosition() == boardrect[14][17].getPosition() || Ghost[i].getPosition() == boardrect[14][17].getPosition() || Ghost[i].getPosition() == boardrect[12][17].getPosition() || Ghost[i].getPosition() == boardrect[16][17].getPosition() || Ghost[i].getPosition() == boardrect[18][17].getPosition() || Ghost[i].getPosition() == boardrect[14][16].getPosition())
				Direction_of_ghosts[i] = rand() % 4;
			if (Direction_of_ghosts[i] == 2)
				Direction_of_ghosts[i] = 3;
			break;
		}
		}
	}
}
void Area_Gate() {
	Area.setPosition(216 + 36 + 10, 430);
	Area.setFillColor(Color::Black);
	gate.setPosition(36 * 9, 420);
}
void eating() {
	bool map[20][19] = { {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
						 {1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
						 {1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,0,1},{1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1},
						 {1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1},{1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1},
				 		 {0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},{1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1},
						 {1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,1,1,1,1},{1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1},
						 {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},{1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1},
						 {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},{1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1}, 
						 {1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1},{1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1}, 
						 {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	eating_rects[7][9].setScale(0, 0);
	eating_rects[1][1].setScale(0, 0);
	eating_rects[1][17].setScale(0, 0);
	eating_rects[18][1].setScale(0, 0);
	eating_rects[18][17].setScale(0, 0);
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			eating_rects[i][j].setSize(Vector2f(5, 5));
			eating_rects[i][j].setPosition(36 * j + 18 - 2.5, 100 + 40 * i + 20 - 2.5);
			eating_rects[i][j].setFillColor(Color(255, 204, 204));
			if (map[i][j] == 0)
			{
				window.draw(eating_rects[i][j]);
				if (sprite.getGlobalBounds().intersects(eating_rects[i][j].getGlobalBounds()))
				{
					_score++;
					eating_rects[i][j].setScale(0, 0);
					if (q == 0)
						Eating_sound.sound.play();
				}
			}
		}
	}
	eating_rects[7][9].setScale(0, 0);
}
void eating_big(Texture& big_eat) {
	Sound_big_eating.buffer.loadFromFile("pacman_eatfruit.wav");
	Sound_big_eating.sound.setBuffer(Sound_big_eating.buffer);
	for (int i = 0; i < 4; i++) {
		Big_eating_circles[i].setTexture(&big_eat);
	}
	for (int i = 0; i < 4; i++)
		Big_eating_circles[i].setRadius(10);
	Big_eating_circles[0].setPosition(boardrect[1][1].getPosition().x + 5, boardrect[1][1].getPosition().y + 5);
	Big_eating_circles[1].setPosition(boardrect[1][17].getPosition().x + 5, boardrect[1][17].getPosition().y + 5);
	Big_eating_circles[2].setPosition(boardrect[18][1].getPosition().x + 5, boardrect[18][1].getPosition().y + 5);
	Big_eating_circles[3].setPosition(boardrect[18][17].getPosition().x + 5, boardrect[18][17].getPosition().y + 5);
	for (int i = 0; i < 4; i++) {
		if (sprite.getGlobalBounds().intersects(Big_eating_circles[i].getGlobalBounds())) {
			intersection_of_pacman_with_big_food = true;
			_score += 5;
			Big_eating_circles[i].setScale(0, 0);
			Clock_of_hmbrgr.restart();
			Sound_big_eating.sound.play();
			for (int i = 0; i < 4; i++) {
				Ghost_hmbrgr[i].setSize(Vector2f(36, 40));
			}
		}
		window.draw(Big_eating_circles[i]);
	}
}
void Beginning_Sound(Texture& readyy, Texture& big_eat)
{
	Clock clock;

	Beginning_sound.buffer.loadFromFile("pacman_beginning.wav");
	Beginning_sound.sound.setBuffer(Beginning_sound.buffer);
	while (true) {
		window.clear();
		Background();
		window.draw(sprite);
		eating();
		window.draw(Area);
		window.draw(gate);
		Ghosts();
		Ready(readyy);
		eating_big(big_eat);
		window.display();
		if (n == 2)
		{
			if (q == 0) {
				Beginning_sound.sound.play(); n++; clock.restart();
			}
		}
		if (clock.getElapsedTime().asSeconds() > 4.5) {
			clock_of_ghosts.restart();
			break;
		}
	}
}
void windowpollevent(Event& ev) {
	while (window.pollEvent(ev))
	{
		if (ev.type == Event::Closed)
			window.close();
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Right) || Joystick::isButtonPressed(0, 1))
		{
			direction = 1;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left) || Joystick::isButtonPressed(0, 3))
		{
			direction = 2;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Up) || Joystick::isButtonPressed(0, 0))
		{
			direction = 3;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down) || Joystick::isButtonPressed(0, 2))
		{
			direction = 4;
		}
		if (ev.type == Event::KeyPressed || ev.type == Event::JoystickButtonPressed) {
			if (ev.key.code == Keyboard::Up || Joystick::isButtonPressed(0, 0)) {
				menu.MoveUp();
			}
			if (ev.key.code == Keyboard::Down || Joystick::isButtonPressed(0, 2)) {
				menu.MoveDown();
			}
			if (ev.key.code == Keyboard::Return) {
				if (menu.pressed() == 0) {
					pagenum = 0;
				}
				if (menu.pressed() == 1) {
					pagenum = 1;
				}
				if (menu.pressed() == 2) {
					pagenum = -1;
				}
				if (menu.pressed() == 3) {
				}
			}
		}
	}
	if ((z == 1 && direction == 2) || (z == 2 && direction == 1) || (z == 3 && direction == 4) || (z == 4 && direction == 3))
		z = direction;
	Inter_Sections();
}
void pacman_movement(Clock& clock) {
	if (z == 1)
	{
		sprite.move(Velocity, 0);
		if (sprite.getPosition().x >= 684)
			sprite.setPosition(-36, sprite.getPosition().y);
		if (clock.getElapsedTime().asSeconds() <= 0.1)
		{
			sprite.setTextureRect(IntRect(0, 0, 16, 16));
		}
		else if (clock.getElapsedTime().asSeconds() <= 0.2)
		{
			sprite.setTextureRect(IntRect(3 * 16, 0, 16, 16));
		}
		else {
			sprite.setTextureRect(IntRect(16, 0, 16, 16));
		}
		Background();  if (r == 1) { sprite.move(-Velocity, 0); }
	}
	else if (z == 2)
	{
		sprite.move(-Velocity, 0);
		if (sprite.getPosition().x <= -36)
			sprite.setPosition(684, sprite.getPosition().y);
		if (clock.getElapsedTime().asSeconds() <= 0.1)
		{
			sprite.setTextureRect(IntRect(16, 16 * 2, 16, 16));
		}
		else if (clock.getElapsedTime().asSeconds() <= 0.2)
		{
			sprite.setTextureRect(IntRect(3 * 16, 16 * 2, 16, 16));
		}
		else
		{
			sprite.setTextureRect(IntRect(0, 0, 16, 16));
		}
		Background();  if (r == 1) { sprite.move(Velocity, 0); }
	}
	else if (z == 3)
	{
		sprite.move(0, -Velocity);
		if (clock.getElapsedTime().asSeconds() <= 0.1)
		{
			sprite.setTextureRect(IntRect(16, 16, 16, 16));
		}
		else if (clock.getElapsedTime().asSeconds() <= 0.2)
		{
			sprite.setTextureRect(IntRect(3 * 16, 16, 16, 16));
		}
		else
		{
			sprite.setTextureRect(IntRect(0, 0, 16, 16));
		}
		Background();  if (r == 1) { sprite.move(0, Velocity); }
	}
	else if (z == 4)
	{
		sprite.move(0, Velocity);
		if (clock.getElapsedTime().asSeconds() <= 0.1)
		{
			sprite.move(0, Velocity); Background(); if (r == 1) { sprite.move(0, -Velocity); }
		}
		else if (clock.getElapsedTime().asSeconds() <= 0.2)
		{
			sprite.setTextureRect(IntRect(3 * 16, 3 * 16, 16, 16));
		}
		else
		{
			sprite.setTextureRect(IntRect(0, 0, 16, 16));
		}
		Background(); if (r == 1) { sprite.move(0, -Velocity); }
	}
	if (clock.getElapsedTime().asSeconds() >= 0.3)
		clock.restart();
}
bool check_Sound_Intersection(bool& intersection, RectangleShape& sound, RectangleShape& rec) {
	if (rec.getGlobalBounds().intersects(sound.getGlobalBounds()))
		intersection = true;
	return intersection;
}
bool check_Sound_Press(bool& pressed, RectangleShape& sound, RectangleShape& rec, bool& intersection) {
	if (Mouse::isButtonPressed(Mouse::Left) && (intersection))
		pressed = true;
	return pressed;
}
void Sound_On_Off(bool& intersection, bool& pressed, int& x, int& i, Mouse& ms, RectangleShape& sound, RectangleShape& rec) {
	intersection = false;
	pressed = false;
	sound.setSize(Vector2f(60, 60));
	rec.setPosition(ms.getPosition().x - 630, ms.getPosition().y - 130);
	if (check_Sound_Intersection(intersection, sound, rec)) {
		sound.setSize(Vector2f(70, 70));
		if (check_Sound_Press(pressed, sound, rec, intersection)) {
			sound.setTextureRect(IntRect(62 * x, 0, 62, 41));
			sound.setSize(Vector2f(60, 60));
			if (i != 0) {
				x += 1;
				x %= 2;
				i = 0;
			}
		}
		else {
			i++;
		}
	}
	if (x == 0) {
		q = 1;
	}
	else {
		q = 0;
	}
}
void score() {
	Font ScoreFont;
	ScoreFont.loadFromFile("Crackman.otf");
	Score.setFont(ScoreFont);
	Score.setString("HIGHEST SCORE: " + to_string(highScore) + "\nSCORE: " + to_string(_score));
	Score.setPosition(0, 0);
	Score.setCharacterSize(40);
	Score.setFillColor(Color{ 255, 137, 0 });
	if (_score >= highScore)
		highScore = _score;
	window.draw(Score);
}
void Drawing(Clock& clock, RectangleShape& BG, RectangleShape& sound, Texture& Pacman, Texture& big_eat) {
	Texture hmbrgr;
	hmbrgr.loadFromFile("hmbrgr.png");
	if (pagenum != 0) {
		n = 1;
		window.draw(BG);
		menu.draw(window);
		if (pagenum == -1) {
			window.close();
		}
		sprite.setPosition(332, 100 + 10 * 40);
		clock.restart();
		clock_of_ghosts.restart();
		window.draw(sound);
		Ghost[0].setPosition(boardrect[7][9].getPosition());

	}
	else if (pagenum == 0)
	{
		Background();
		window.draw(sprite);
		eating();
		window.draw(Area);
		window.draw(gate);
		eating_big(big_eat);
		Ghosts();
		Ghosts_Begin();
		window.draw(sound);
		Pacman_lives(Pacman);
		if (Clock_of_hmbrgr.getElapsedTime().asSeconds() < 10 && intersection_of_pacman_with_big_food == true)
			for (int i = 0; i < 4; i++) {

				Ghost_hmbrgr[i].setTexture(&hmbrgr);
				if (ana_zhgt[i] == 0)
				{
					Ghost_hmbrgr[i].setScale(1, 1);
					ana_zhgt[i]++;
				}
				Ghost_hmbrgr[i].setPosition(Ghost[i].getPosition());
				window.draw(Ghost_hmbrgr[i]);
				if (sprite.getGlobalBounds().intersects(Ghost_hmbrgr[i].getGlobalBounds())) {
					_score += 10;
					Ghost[i].setPosition(boardrect[7][9].getPosition());
					Ghost_hmbrgr[i].setPosition(boardrect[7][9].getPosition());
					Direction_of_ghosts[i] = 0;
					Ghost_hmbrgr[i].setScale(0, 0);
				}
			}
		else {
			for (int i = 0; i < 4; i++) {
				ana_zhgt[i] = 0;
				Ghost_hmbrgr[i].setScale(0, 0);
				intersection_of_pacman_with_big_food = false;
			}
		}
	}
}
RectangleShape Game_over(Texture& gameover) {

	gameover.loadFromFile("game_over.png");
	RectangleShape Go;
	Go.setTexture(&gameover);
	Go.setPosition(boardrect[8][7].getPosition());
	Go.setSize(Vector2f(36 * 5, 80));
	return Go;
}
void pacman_death(Clock& clock_of_death, Texture& pacman) {
	Texture gameover;
	RectangleShape Go = Game_over(gameover);
	PacmanDeath.buffer.loadFromFile("pacman_death.wav");
	PacmanDeath.sound.setBuffer(PacmanDeath.buffer);
	Texture pacman_Death;
	pacman_Death.loadFromFile("PacmanDeath16.png");
	for (int i = 0; i < 4; i++) {
		int c = 0;
		if (Ghost[i].getGlobalBounds().intersects(sprite.getGlobalBounds()) && intersection_of_pacman_with_big_food == false) {
			sprite.setTexture(&pacman_Death);
			while (true) {
				if (c == 0 && q == 0) {
					c++;
					PacmanDeath.sound.play();
				}
				if (_score > highScore)
					highScore = _score;
				if (clock_of_death.getElapsedTime().asSeconds() < 0.15)
					sprite.setTextureRect(IntRect(0, 0, 16, 16));
				else if (clock_of_death.getElapsedTime().asSeconds() < 2 * 0.15)
					sprite.setTextureRect(IntRect(1 * 16, 0, 16, 16));
				else if (clock_of_death.getElapsedTime().asSeconds() < 3 * 0.15)
					sprite.setTextureRect(IntRect(2 * 16, 0, 16, 16));
				else if (clock_of_death.getElapsedTime().asSeconds() < 4 * 0.15)
					sprite.setTextureRect(IntRect(3 * 16, 0, 16, 16));
				else if (clock_of_death.getElapsedTime().asSeconds() < 5 * 0.15)
					sprite.setTextureRect(IntRect(4 * 16, 0, 16, 16));
				else if (clock_of_death.getElapsedTime().asSeconds() < 6 * 0.15)
					sprite.setTextureRect(IntRect(5 * 16, 0, 16, 16));
				else if (clock_of_death.getElapsedTime().asSeconds() < 7 * 0.15)
					sprite.setTextureRect(IntRect(6 * 16, 0, 16, 16));
				else if (clock_of_death.getElapsedTime().asSeconds() < 8 * 0.15)
					sprite.setTextureRect(IntRect(7 * 16, 0, 16, 16));
				else if (clock_of_death.getElapsedTime().asSeconds() < 9 * 0.15)
					sprite.setTextureRect(IntRect(8 * 16, 0, 16, 16));
				else if (clock_of_death.getElapsedTime().asSeconds() < 10 * 0.15)
					sprite.setTextureRect(IntRect(9 * 16, 0, 16, 16));
				else if (clock_of_death.getElapsedTime().asSeconds() < 11 * 0.15)
					sprite.setTextureRect(IntRect(10 * 16, 0, 16, 16));
				else if (clock_of_death.getElapsedTime().asSeconds() < 12 * 0.15)
				{
					sprite.setTextureRect(IntRect(11 * 16, 0, 16, 16));
					if (u == 0)
						pacman_textures[1].setScale(0, 0);
					else if (u == 1)
						pacman_textures[0].setScale(0, 0);
					else if (u == 2)
					{
						pagenum = 1000;
						_score = 0;
						u = -1;
						pacman_textures[0].setScale(1, 1);
						pacman_textures[1].setScale(1, 1);
						for (int i = 0; i < 20; i++)
						{
							for (int j = 0; j < 19; j++) {

								eating_rects[i][j].setScale(1, 1);
								for (int i = 0; i < 4; i++) {
									Big_eating_circles[i].setScale(1, 1);
								}
							}
						}


					}

					z = 1;
					PacMan(pacman);
					Ghost[0].setPosition(boardrect[7][9].getPosition());
					Ghost[1].setPosition(boardrect[9][8].getPosition().x, boardrect[9][8].getPosition().y - 10);
					Ghost[2].setPosition(boardrect[9][9].getPosition().x, boardrect[9][9].getPosition().y - 10);
					Ghost[3].setPosition(boardrect[9][10].getPosition().x, boardrect[9][10].getPosition().y - 10);
					for (int i = 0; i < 4; i++)
						Direction_of_ghosts[i] = 0;
					k = 1; ghost_start = 0; clock_of_ghosts.restart();
					u++;
					break;
				}
				window.clear();
				Background();
				Area_Gate();
				window.draw(gate);
				window.draw(Area);
				eating();
				window.draw(Ghost[i]);
				Ghosts();
				window.draw(sprite);
				if (u == 2) {
					window.draw(Go);

				}

				window.display();
			}
		}
	}
}



int main() {
	for (int i = 0; i < 4; i++)
		Big_eating_circles[i].setScale(1.5, 1.5);
	Texture pacman, Ghost, readyy, big_eat, pauseTexture, lol, Pacman;
	big_eat.loadFromFile("big_eat.png");
	readyy.loadFromFile("ready.png");
	Ready(readyy);
	RectangleShape BG;
	Event ev;
	Clock clock, clock_of_death;
	Eating_sound.buffer.loadFromFile("pac-man---eating-By-Tuna.wav");
	Eating_sound.sound.setBuffer(Eating_sound.buffer);
	Pacman.loadFromFile("Pacman.svg.png");
	BG = Main_Menu();
	PacMan(pacman);
	Area_Gate();
	RectangleShape rec(Vector2f(5, 5));
	RectangleShape sound(Vector2f(50, 50));
	RectangleShape  pause(Vector2f(60, 60));
	sound.setPosition(620,20);
	pause.setPosition(560, 20);
	pauseTexture.loadFromFile("pause.png");
	pause.setTexture(&pauseTexture);
	lol.loadFromFile("sound on-off.png");
	sound.setTexture(&lol);
	sound.setTextureRect(IntRect(0, 0, 62, 41));
	bool intersection;
	bool pressed;
	int x = 0;
	int i = 1;
	int w = 0;
	int j = 1;
	Mouse ms;
	window.setFramerateLimit(120);
	while (window.isOpen())
	{
		Sound_On_Off(intersection, pressed, x, i, ms, sound, rec);
		Ghosts_movement();
		windowpollevent(ev);
		pacman_movement(clock);
		window.clear();
		pause_play(rec, pause, j, w, x, i, ms, intersection, pressed, sound);
		score();
		Drawing(clock, BG, sound, Pacman, big_eat);
		window.display();
		if (n == 2)
		{
			Beginning_Sound(readyy, big_eat);
		}
		n++;
		clock_of_death.restart();
		pacman_death(clock_of_death, pacman);
	}

}
/****************************************
*		pagenum		/		fun			*
*****************************************					
*		1000		/	     menu		*
*					/					*
*	      0			/	   gameplay	    *
*					/		            *
*                   /					*
*		 -1			/		close		*
*					/					*
*		  			/					*
*		  1			/	 instruction	*
*					/					*
*					/					*
*					/					*
*					/					*
*					/					*															
*					/					*
*****************************************
*/



