#ifndef _TETRIS_ITEM
#define _TETRIS_ITEM

#include "agl/game.h"
#include "agl/drawer.h"
#include "agl/timer.h"
#include "support.h"

using namespace AGL;

#define ITEM_THIEF	0
#define ITEM_CLOUD	1
#define ITEM_BOMB		2
#define ITEM_TURTLE	3
	
namespace Tess
{
	class Item
	{
		private:
			int itemCount;
			string* itemNames;
			Image* icon;
			Timer* timer;
			bool used;
	
		public:	
			Image* image;
			int type;
			bool finished;
		
			Item();
			Item(int type);
			~Item();
			void update();
			void stop(int** stageMatrix, int itemX, int itemY);
			void drawIcon(Drawer* drawer, IntVector2 position);
			void use();
			void drawEnemyEffect(Drawer* drawer, IntVector2 stagePosition);
			void drawEffect(Drawer* drawer, IntVector2 stagePosition);
	};
}

#endif

