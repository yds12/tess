#include "rankingscreen.h"

using namespace Tess;

RankingScreen::RankingScreen(Game* game, Drawer* drawer, Keyboard* keyboard, Mouse* mouse)
{
	this->game = game;
	this->drawer = drawer;
	this->keyboard = keyboard;
	this->mouse = mouse;
	
	string* options = new string[2];
	options[0] = Lang::get(9);
	options[1] = Lang::get(39);
	menu = new Menu(options, 2, drawer, keyboard, mouse);
	menu->set(200, 500);
	
	string* options2 = new string[2];
	options2[0] = Lang::get(40);
	options2[1] = Lang::get(41);
	confirmMenu = new Menu(options2, 2, drawer, keyboard, mouse);
		
	clearConfirmation = false;
	rankingDeleted = false;
	end = false;
	currentDifficulty = DIFFICULTY_MODERATE;
	ranking = new Ranking();
	ranking->load(currentDifficulty);
}

RankingScreen::~RankingScreen()
{
	
}

void RankingScreen::update()
{
	if(rankingDeleted)
	{
		if(keyboard->isKeyPressed(keyboard->getIndex(KEY_ENTER))) end = true;
		return;
	}

	menu->update();
	
	if(clearConfirmation)
	{
		confirmMenu->update();
		
		if(confirmMenu->optionChosen)
		{
			if(confirmMenu->selected == 1)
			{
				// clear rankings
				ranking->clearAll();
				rankingDeleted = true;
			}
			
			clearConfirmation = false;
			menu->reset();
			confirmMenu->reset();
		}
	}
	
	if(menu->optionChosen)
	{
		if(menu->selected == 0) end = true;
		else clearConfirmation = true;
	}
	
	if(keyboard->isKeyPressed(keyboard->getIndex(KEY_LEFT))
		|| keyboard->isKeyHeld(keyboard->getIndex(KEY_LEFT)))
		if(currentDifficulty > 0)
		{
			currentDifficulty--;
			ranking->load(currentDifficulty);				
		}
		
	if(keyboard->isKeyPressed(keyboard->getIndex(KEY_RIGHT))
		|| keyboard->isKeyHeld(keyboard->getIndex(KEY_RIGHT)))
		if(currentDifficulty < DIFFICULTY_HARD)
		{ 
			currentDifficulty++;
			ranking->load(currentDifficulty);				
		}
}

void RankingScreen::draw()
{
	drawer->clearScreen(AGL_COLOR_NAVY);
		
	if(rankingDeleted)
	{
		IntVector2 v(30, 30);	
		drawer->drawText(Support::menuTitleFont, Lang::get(42), v, Support::menuTitleTextColor);
		
		IntVector2 v2(30, 220);	
		drawer->drawText(Support::defaultFont, Lang::get(43), v2, Support::defaultTextColor);
	}
	else if(clearConfirmation)
	{
		IntVector2 v(30, 30);	
		drawer->drawText(Support::menuTitleFont, Lang::get(44), v, Support::menuTitleTextColor);
		
		IntVector2 v2(30, 220);	
		drawer->drawText(Support::defaultFont, Lang::get(45), v2, Support::defaultTextColor);
		
		confirmMenu->draw();
	}
	else
	{
		IntVector2 v2(140, 130);
		ranking->show(drawer, -1, v2);
	
		IntVector2 v(30, 30);		
		const char* text;		
		if(currentDifficulty == DIFFICULTY_EASY) text = Lang::get(46);
		else if(currentDifficulty == DIFFICULTY_MODERATE) text = Lang::get(47);
		else if(currentDifficulty == DIFFICULTY_HARD) text = Lang::get(48);		
		drawer->drawText(Support::menuTitleFont, text, v, Support::menuTitleTextColor);
	
		menu->draw();
	}		
}

