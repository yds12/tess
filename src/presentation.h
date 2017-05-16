#ifndef _TETRIS_PRESENTATION
#define _TETRIS_PRESENTATION

#include "agl/image.h"
#include "agl/font.h"
#include "agl/drawer.h"
#include "agl/keyboard.h"
#include "agl/mouse.h"
#include "agl/timer.h"
#include "agl/color.h"
#include "agl/graphics.h"

#include "support.h"

using namespace AGL;

namespace Tess
{
	class Presentation
	{
		private:
			Image* logo;
			Image* bg;
			Font* font;
			Font* font2;
			Drawer* drawer;
			Keyboard* keyboard;
			Mouse* mouse;
			string text;
			Timer* timer;
			bool ok;
			bool finishedText;
		
			void addLetter();
		public:
			bool end;
		
			Presentation(Drawer* drawer, Keyboard* keyboard, Mouse* mouse);
			~Presentation();
			void update();
			void draw();
	};
}

#endif

