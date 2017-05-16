#ifndef _TETRIS_ANIMATION
#define _TETRIS_ANIMATION

#include "agl/animation.h"
#include "agl/color.h"
#include "agl/graphics.h"
#include "support.h"

#define ANIMATION_TYPE_FADEOUT		0

using namespace AGL;

namespace Tess
{
	class Animation
	{
		private:
			Game* game;
			Drawer* drawer;
			AGL::Animation* animation;
			IntVector2* position;
		
		public:	
			bool finished;
		
			Animation(Game* game, Drawer* drawer, Image* image, char animationType, IntVector2 &_position);
			~Animation();
			
			void draw();
			void update();
	};
}

#endif

