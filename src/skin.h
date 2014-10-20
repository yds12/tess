#ifndef _TETRIS_SKIN
#define _TETRIS_SKIN

#include "agl/support.h"
#include "agl/color.h"
#include "agl/drawer.h"
#include "agl/intvector2.h"
#include "agl/timer.h"
#include "agl/animation.h"
#include "support.h"
#include "animation.h"

namespace Tess
{
	class Skin
	{
		private:
			AGL::Animation** blockAnims;
			int animationSize;
			int animationWait;
		
			Image** blockImgs;
			Image* itemBoxImg;
			bool animated;
			Timer* timer;
			Drawer* drawer;
			bool animating;
		
		public:
			int colors;
			string name;
			string bgSkinName;
			Color bgColor;
		
			Skin(Drawer* drawer, string name, string bgSkinName);
			~Skin();
			void update();
			void draw(int color, IntVector2 position);
			Tess::Animation* getBlockClearingAnimation(Game* game, int color, IntVector2 &position);
			Image* getForeground();
			Image* getMultiplayerForeground();
	};
}

#endif

