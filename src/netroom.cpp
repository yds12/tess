#include "netroom.h"

using namespace Tess;

NetRoom::NetRoom(Game* game, Drawer* drawer, Keyboard* keyboard, Mouse* mouse)
{
	decision = -1;
	this->game = game;
	this->drawer = drawer;
	this->keyboard = keyboard;
	this->mouse = mouse;
	
	string* options = new string[4];
	options[0] = Lang::get(12);
	options[1] = Lang::get(13);
	options[2] = Lang::get(14);
	options[3] = Lang::get(9);	
	menu = new Menu(options, 4, drawer, keyboard, mouse);
	
	netInitialized = false;
	gameReady = false;
	
	cancelFont = Support::cancelFont;
	
	Image* image = Support::loadImage("loading");
	loadingAnim = new AGL::Animation(image, drawer, 4, 1);
}

NetRoom::~NetRoom()
{
	
}

void NetRoom::update()
{
	if(gameReady) return;
	
	menu->update();
	if(menu->optionChosen) decision = menu->selected;
	else decision = -1;
	
	if(!netInitialized)
	{
		if(decision == NETROOM_CREATE)
		{
			net = new Network(game, true);
			netInitialized = true;
		}
		else if(decision == NETROOM_JOIN)
		{
			net = new Network(game, false);
			netInitialized = true;
		}
		else if(decision == NETROOM_VSCPU)
		{
			createdGame = new Stage(game, drawer, true, false, false, DIFFICULTY_EASY);
			gameReady = true;
			return;
		}
	}
	else
	{
		if(net->connected) 
		{
			createdGame = new Stage(game, drawer, true, true, false, DIFFICULTY_EASY);
			createdGame->net = net;
			gameReady = true;
			return;
		}
		else 
		{
			net->tryConnect();
			loadingAnim->update(30);
			
			if(keyboard->isKeyPressed(keyboard->getIndex(KEY_ENTER))
				|| mouse->isLeftButtonPressed())
			{
				menu->reset();
				netInitialized = false;
				delete net;
				net = NULL;
			}
		}
	}
}

void NetRoom::draw()
{
	drawer->clearScreen(AGL_COLOR_NAVY);
	IntVector2 v4(30, 50);
	drawer->drawText(Support::menuTitleFont, Lang::get(15), v4, Support::menuTitleTextColor);
	
	if(menu->optionChosen)
	{
		if(decision != NETROOM_VSCPU)
		{
			IntVector2 v3(400, 286);
			loadingAnim->draw(v3);
		
			if(menu->selected == NETROOM_JOIN)
			{
				IntVector2 v(200, 220);
				drawer->drawText(Support::defaultFont, Lang::get(16), v, Support::defaultTextColor);
			}
			else
			{
				IntVector2 v(200, 220);
				drawer->drawText(Support::defaultFont, Lang::get(17), v, Support::defaultTextColor);
			}
		
			IntVector2 v2(200, 300);
			drawer->drawText(cancelFont, Lang::get(18), v2, AGL_COLOR_ORANGE);
		}
	}
	else menu->draw();
}

