#include "difficulty.h"

using namespace Tess;

DifficultyScreen::DifficultyScreen(Game* game, Drawer* drawer, Keyboard* keyboard, Mouse* mouse)
{
	this->game = game;
	this->drawer = drawer;
	this->keyboard = keyboard;
	this->mouse = mouse;
	
	string* options = new string[4];
	options[0] = Lang::get(6);
	options[1] = Lang::get(7);
	options[2] = Lang::get(8);	
	options[3] = Lang::get(9);	
	menu = new Menu(options, 4, drawer, keyboard, mouse);
	
	gameReady = false;	
	end = false;
}

DifficultyScreen::~DifficultyScreen()
{
	
}

void DifficultyScreen::update()
{
	menu->update();
	
	if(menu->optionChosen)
	{
		if(menu->selected == DIFFICULTY_BACK)
		{
			end = true;
		}
		else
		{
			createdGame = new Stage(game, drawer, false, false, false, menu->selected);
			gameReady = true;
			return;
		}
	}
}

void DifficultyScreen::draw()
{
	drawer->clearScreen(AGL_COLOR_NAVY);
	IntVector2 v4(30, 50);
	drawer->drawText(Support::menuTitleFont, Lang::get(10), v4, Support::menuTitleTextColor);
	menu->draw();
}

