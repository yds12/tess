#ifndef _TETRIS_DIFFICULTY
#define _TETRIS_DIFFICULTY

#include "agl/game.h"
#include "agl/drawer.h"
#include "agl/keyboard.h"
#include "agl/mouse.h"
#include "agl/font.h"
#include "menu.h"
#include "stage.h"
#include "lang.h"

namespace Tess
{
	class DifficultyScreen
	{
		private:
			Game* game;
			Drawer* drawer;
			Keyboard* keyboard;
			Mouse* mouse;
		
			Menu* menu;
		public:
			bool end;
			bool gameReady;
			Stage* createdGame;
		
			DifficultyScreen(Game* game, Drawer* drawer, Keyboard* keyboard, Mouse* mouse);
			~DifficultyScreen();
			void update();
			void draw();
	};
}

#endif

