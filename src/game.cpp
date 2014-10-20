#include <stdlib.h>
#include "agl/game.h"
#include "agl/drawer.h"

// ------------ Includes here ---------------
#include <iostream>
#include "agl/sound.h"
#include "agl/music.h"
#include "agl/joystick.h"
#include "agl/vector2.h"
#include "agl/intvector2.h"
#include "agl/gameobject.h"
#include "agl/support.h"
#include "agl/color.h"

#include "stage.h"
#include "presentation.h"
#include "menu.h"
#include "netroom.h"
#include "difficulty.h"
#include "rankingscreen.h"
#include "options.h"
#include "lang.h"

// ------------------------------------------

// ------------ Defines here ----------------
#define GAME_VERSION		"0.8.5"

// GAME STATES
#define GS_INITIALIZING	-1
#define GS_PRESENTATION	0
#define GS_TITLE			1
#define GS_SINGLE			2
#define GS_OPTIONS		3
#define GS_MULTIPLAYER	4
#define GS_RANKING		5
#define GS_QUIT			6
#define GS_NETROOM		7
#define GS_DIFFICULTY		8
#define GS_CREDITS		9
// ------------------------------------------

using namespace Tess;

AGL::Game* game = NULL;
AGL::Drawer* drawer = NULL;

// ------------ Declare variables here ------
AGL::Keyboard* keyboard = NULL;
AGL::Mouse* mouse = NULL;
AGL::Joystick* joystick = NULL;

int gameState;
Stage* currentStage = NULL;
Presentation* presentation = NULL;
Menu* titleMenu = NULL;
Ranking* ranking = NULL;
RankingScreen* rankingScreen = NULL;
Image* tessImg = NULL;
NetRoom* netRoom = NULL;
DifficultyScreen* difficultyScreen = NULL;
Options* options = NULL;

int currentDifficulty;
// ------------------------------------------

void initialize()
{
	// ------------ Initialize variables here ---
	char title[13];	
	sprintf(title, "TESS %s", GAME_VERSION);
	
	game = new AGL::Game(title);
	drawer = new AGL::Drawer;
	game->showMouse(true);

	keyboard = new AGL::Keyboard(true, true, true, true, false, false);
	keyboard->heldDelay = 15;
	keyboard->heldRepeat = 3;
	mouse = new AGL::Mouse;
	
	gameState = GS_INITIALIZING;
	// ------------------------------------------
}

void setScreen()
{
	if(Support::settings.fullscreen != drawer->fullScreen) drawer->setScreen(800, 600, Support::settings.fullscreen);
}

void loadContent()
{
	// ------------ Load content here -----------
	Support::loadSupportContent();
	Options::load();
	setScreen();
		
	tessImg = Support::loadImage("tess");	
	// ------------------------------------------
}

void createTitleMenu()
{
	string* options = new string[6];
	options[0] = Lang::get(0);
	options[1] = Lang::get(1);
	options[2] = Lang::get(2);
	options[3] = Lang::get(3);
	options[4] = Lang::get(4);
	options[5] = Lang::get(5);
	titleMenu = new Menu(options, 6, drawer, keyboard, mouse);
}

void update()
{
	game->update(keyboard, mouse, true);
	
	// ------------ Update the game here --------
	
	if(gameState == GS_INITIALIZING)
	{
		gameState = GS_PRESENTATION;
		Support::playMusic(MUSIC_DEFAULT);
		presentation = new Presentation(drawer, keyboard, mouse);
	}
	else if(gameState == GS_PRESENTATION)
	{
		presentation->update();	
		if(presentation->end) 
		{
			gameState = GS_TITLE;
			createTitleMenu();
		}
	}	
	else if(gameState == GS_TITLE)
	{
		titleMenu->update();
		
		if(titleMenu->optionChosen)
		{				
			if(titleMenu->selected == 0) // single
			{
				gameState = GS_DIFFICULTY;
				difficultyScreen = new DifficultyScreen(game, drawer, keyboard, mouse);
			}
			else if(titleMenu->selected == 1) // multi
			{
				gameState = GS_NETROOM;
				netRoom = new NetRoom(game, drawer, keyboard, mouse);
			}
			else if(titleMenu->selected == 2) // options
			{
				gameState = GS_OPTIONS;
				options = new Options(game, drawer, keyboard, mouse);
				Options::load();
			}
			else if(titleMenu->selected == 3) // ranking
			{
				rankingScreen = new RankingScreen(game, drawer, keyboard, mouse);
				gameState = GS_RANKING;
			}
			else if(titleMenu->selected == 4) // credits
			{
				gameState = GS_CREDITS;
				game->setQuit(true);
			}
			else if(titleMenu->selected == 5) // quit
			{
				gameState = GS_QUIT;
				game->setQuit(true);
			}
			
			delete titleMenu;
			titleMenu = NULL;
		}
	}	
	else if(gameState == GS_OPTIONS)
	{
		options->update();
		
		if(options->end)
		{
			Options::load();			
			setScreen();
			
			delete options;
			options = NULL;
			gameState = GS_TITLE;
			createTitleMenu();			
		}
	}	
	else if(gameState == GS_RANKING)
	{
		rankingScreen->update();
		
		if(rankingScreen->end)
		{
			delete rankingScreen;
			rankingScreen = NULL;
			gameState = GS_TITLE;
			createTitleMenu();
		}
	}	
	else if(gameState == GS_SINGLE)
	{
		currentStage->update(keyboard, mouse);
		if(currentStage->end)
		{ 
			gameState = GS_TITLE;
			createTitleMenu();
		}
	}	
	else if(gameState == GS_NETROOM)
	{
		netRoom->update();
		
		if(netRoom->decision > -1)
		{
			if(netRoom->decision == NETROOM_BACK)
			{
				delete netRoom;
				netRoom = NULL;
				gameState = GS_TITLE;
				createTitleMenu();				
			}
			else if(netRoom->gameReady)
			{
				gameState = GS_MULTIPLAYER;
				currentStage = netRoom->createdGame;
				delete netRoom;
				netRoom = NULL;
			}
		}
	}	
	else if(gameState == GS_MULTIPLAYER)
	{
		currentStage->update(keyboard, mouse);
		if(currentStage->end)
		{
			delete currentStage;
			currentStage = NULL;
			gameState = GS_TITLE;
			createTitleMenu();
		}
	}
	else if(gameState == GS_DIFFICULTY)
	{
		difficultyScreen->update();
		
		if(difficultyScreen->gameReady)
		{
			gameState = GS_SINGLE;
			currentStage = difficultyScreen->createdGame;
		}
		else if(difficultyScreen->end)
		{
			delete difficultyScreen;
			difficultyScreen = NULL;
			gameState = GS_TITLE;
			createTitleMenu();
		}
	}
	else if(gameState == GS_CREDITS)
	{
		
	}
	// ------------------------------------------
	
	//if(Support::debug && game->gameCounter == 0) cout << "FPS: " << game->getFps(1) << endl;
}

void draw()
{
	// ------------ Draw objects here -----------
	if(gameState == GS_SINGLE)
	{
		drawer->clearScreen(currentStage->skin->bgColor);
		currentStage->draw();
	}
	else if(gameState == GS_PRESENTATION)
	{
		presentation->draw();
	}
	else if(gameState == GS_TITLE)
	{
		drawer->clearScreen(AGL_COLOR_NAVY);
		
		IntVector2 v(100, 100);
		drawer->draw(tessImg, v);
		
		titleMenu->draw();
	}
	else if(gameState == GS_RANKING)
	{
		rankingScreen->draw();		
	}
	else if(gameState == GS_NETROOM)
	{
		netRoom->draw();
	}	
	else if(gameState == GS_DIFFICULTY)
	{
		difficultyScreen->draw();
	}	
	else if(gameState == GS_MULTIPLAYER)
	{
		currentStage->draw();
	}
	else if(gameState == GS_OPTIONS)
	{
		options->draw();
	}
	else if(gameState == GS_CREDITS)
	{
		
	}
	// ------------------------------------------
	
	drawer->updateScreen();
}

void unloadContent()
{
	// ------------ Unload objects here ---------
	
	// ------------------------------------------

	game->end();
}

/// Main method, initializes the program
int main(int argc, char *argv[])
{
	if(argc > 1 && AGL::Format::compareCharString(argv[1], "-d")) Support::debug = true;
	else Support::debug = false;
	
	srand(time(NULL));
	initialize();
	loadContent();
	game->runFixedFps(update, draw, 60);
	unloadContent();

	return 0;
}

