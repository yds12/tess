#ifndef _TETRIS_OPTIONS
#define _TETRIS_OPTIONS

#include "agl/game.h"
#include "agl/drawer.h"
#include "agl/keyboard.h"
#include "agl/mouse.h"
#include "agl/font.h"
#include "menu.h"
#include "ranking.h"
#include "support.h"

#define OPTIONS_SCREEN		0
#define OPTIONS_MUSIC		1
#define OPTIONS_SOUND		2
#define OPTIONS_LANGUAGE	3
#define OPTIONS_SKIN			4
#define OPTIONS_BGSKIN		5
#define OPTIONS_BACK			6

#define OPTIONS_FILE			"cfgopt"

namespace Tess
{
	class Options
	{
		private:
			Game* game;
			Drawer* drawer;
			Keyboard* keyboard;
			Mouse* mouse;
		
			Menu* menu;
			Image* bg;
		
			int numberOfOptions;
			int* currentOption;
			int* optionSize;
			string** optionName;
		
			void nextOption(int option);
			void previousOption(int option);
			void loadInMemory();
		public:
			bool end;
				
			Options(Game* game, Drawer* drawer, Keyboard* keyboard, Mouse* mouse);
			~Options();
			void update();
			void draw();
			static void load();
			void save();
	};
}

#endif

