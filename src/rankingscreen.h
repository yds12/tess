#ifndef _TETRIS_RANKINGSCREEN
#define _TETRIS_RANKINGSCREEN

#include "agl/game.h"
#include "agl/drawer.h"
#include "agl/keyboard.h"
#include "agl/mouse.h"
#include "agl/font.h"
#include "menu.h"
#include "ranking.h"
#include "support.h"

namespace Tess
{
	class RankingScreen
	{
		private:
			Game* game;
			Drawer* drawer;
			Keyboard* keyboard;
			Mouse* mouse;
		
			Menu* menu;
			Menu* confirmMenu;
			Ranking* ranking;
			int currentDifficulty;
		
			bool clearConfirmation;
			bool rankingDeleted;
		public:
			bool end;
		
			RankingScreen(Game* game, Drawer* drawer, Keyboard* keyboard, Mouse* mouse);
			~RankingScreen();
			void update();
			void draw();
	};
}

#endif

