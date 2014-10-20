#ifndef _TETRIS_NAMEDIALOG
#define _TETRIS_NAMEDIALOG

#include "agl/keyboard.h"
#include "agl/mouse.h"
#include "agl/drawer.h"
#include "agl/timer.h"
#include "agl/intvector2.h"
#include "support.h"

using namespace AGL;

namespace Tess
{
	class NameDialog
	{
		private:
			Drawer* drawer;
			Keyboard* keyboard;
			Timer* timer;
			Mouse* mouse;
			string chars;
			Image* fgImg;
			Image* txtImg;
			Font* font;
	
		public:
			bool exit;
			string name;
		
			NameDialog(Drawer* drawer, Mouse* mouse);
			~NameDialog();		
			void update();
			void draw();
	};
}

#endif

