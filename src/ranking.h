#ifndef _TETRIS_RANKING
#define _TETRIS_RANKING

#include "agl/drawer.h"
#include "agl/filemanipulator.h"
#include "agl/color.h"
#include "agl/graphics.h"
#include "agl/timer.h"
#include "support.h"

using namespace AGL;

namespace Tess
{
	class Ranking
	{
		private:
			Image* bg;
			Timer* t;
			bool timerStarted;
			Font* font;
			string* rankingFiles;
	
		public:
			string* names;
			int* scores;
			int* lines;
			int size;
		
			Ranking();
			~Ranking();
			void load(int difficulty);
			void save(int difficulty);
			void clearAll();
			int addEntry(string name, int score, int lines);
			int checkEntry(int score);
			void show(Drawer* drawer);
			void show(Drawer* drawer, int position);
			void show(Drawer* drawer, int position, IntVector2 location);
	};
}

#endif

