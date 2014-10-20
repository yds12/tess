#ifndef _TETRIS_MENU
#define _TETRIS_MENU

#include "agl/drawer.h"
#include "agl/keyboard.h"
#include "agl/mouse.h"
#include "agl/color.h"
#include "agl/timer.h"
#include "support.h"

using namespace AGL;

namespace Tess
{
	class Menu
	{
		private:
			string* options;
			int numberOfOptions;
			Drawer* drawer;
			Keyboard* keyboard;
			Mouse* mouse;
		
			int x;
			int y;
			int width;
			int optionSpacing;
			Color textColor;
			Color selectedColor;
		
			Rectangle area;		
	
		public:
			bool optionChosen;
			int selected;
		
			Menu(string* options, int numberOfOptions, Drawer* drawer, Keyboard* keyboard, Mouse* mouse);
			void set(int x, int y);
			void set(int x, int y, int width);
			void update();
			void draw();
			void reset();
	};
}

#endif

