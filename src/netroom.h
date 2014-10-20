#ifndef _TETRIS_NETROOM
#define _TETRIS_NETROOM

#include "agl/game.h"
#include "agl/drawer.h"
#include "agl/keyboard.h"
#include "agl/mouse.h"
#include "agl/font.h"
#include "agl/animation.h"
#include "menu.h"
#include "network.h"
#include "stage.h"
#include "lang.h"

#define NETROOM_VSCPU	0
#define NETROOM_CREATE	1
#define NETROOM_JOIN		2
#define NETROOM_BACK		3

namespace Tess
{
	class NetRoom
	{
		private:
			Game* game;
			Drawer* drawer;
			Keyboard* keyboard;
			Mouse* mouse;
		
			Menu* menu;
			Network* net;
		
			Font* cancelFont;
			AGL::Animation* loadingAnim;
		
			bool netInitialized;
		public:
			bool gameReady;
			int decision;
			Stage* createdGame;
		
			NetRoom(Game* game, Drawer* drawer, Keyboard* keyboard, Mouse* mouse);
			~NetRoom();
			void update();
			void draw();
	};
}

#endif

