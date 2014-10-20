#include "animation.h"

using namespace Tess;

Tess::Animation::Animation(Game* game, Drawer* drawer, Image* image, char animationType, IntVector2 &_position)
{
	finished = false;
	this->game = game;
	this->drawer = drawer;
	position = new IntVector2(_position);	
	
	if(animationType == ANIMATION_TYPE_FADEOUT)
	{
		Image* resultingImage = new Image(image);
		Image** images = new Image*[8];
		
		images[0] = image;
		
		for(int i = 1; i < 8; i++)
		{
			images[i] = new Image(image);
			Graphics::applyColorFilter(images[i], AGL_COLOR_WHITE, 255 - (32 * i));
			
			IntVector2 v(resultingImage->size->x, 0);	
			resultingImage = Graphics::overlapImages(resultingImage, images[i], v);
			
			delete images[i];
		}

		delete image;
		delete[] images;

		animation = new AGL::Animation(resultingImage, drawer, 8, 1);
	}	
}

Tess::Animation::~Animation()
{
	delete animation;
}

void Tess::Animation::update()
{
	if(finished) return;	
	animation->update(0.04f, game);
}

void Tess::Animation::draw()
{
	if(finished) return;
		
	if(!animation->finished)
	{
		animation->draw(*position);
	}
	else
		finished = true;
}

