#include "skin.h"

using namespace Tess;

Skin::Skin(Drawer* drawer, string name, string bgSkinName)
{	
	itemBoxImg = Support::loadImage("itembox");
	
	this->bgSkinName = bgSkinName;
	this->name = name;
	this->drawer = drawer;
	animating = false;
	animationSize = 5;
	animationWait = 15;
	
	if(name == "black")
	{
		animated = true;
		colors = 1;
		bgColor = AGL_COLOR_GAINSBORO;
	}
	else if(name == "color")
	{
		animated = false;
		colors = 8;
		bgColor = AGL_COLOR_SILVER;
	}
	else if(name == "round")
	{
		animated = false;
		colors = 5;
		bgColor = AGL_COLOR_GAINSBORO;
	}
	else if(name == "roundblack")
	{
		animated = false;
		colors = 1;
		bgColor = AGL_COLOR_GAINSBORO;
	}
	else if(name == "jewel")
	{
		animated = false;
		colors = 5;
		bgColor = AGL_COLOR_BLACK;
	}
	
	blockImgs = new Image*[colors];
	
	for(int i = 0; i < colors; i++)
	{		
		char str[2];
		sprintf(str, "%d", (i + 1));	
		string blockFile = "block_" + name + str;
				
		blockImgs[i] = Support::loadImage(blockFile);
	}
		
	if(animated) 
	{
		blockAnims = new AGL::Animation*[colors];
	
		for(int i = 0; i < colors; i++)
		{		
			char str[2];
			sprintf(str, "%d", (i + 1));	
			string blockFile = "anim_" + name + str;
				
			blockAnims[i] = new AGL::Animation(Support::loadImage(blockFile), drawer, animationSize, 1);
		}
		
		timer = new Timer();
		timer->start();
	}
}

Skin::~Skin()
{
	for(int i = 0; i < colors; i++)
	{
		if(animated) delete blockAnims[i];
		
		delete blockImgs[i];
	}
	
	delete[] blockImgs;
	if(animated) delete[] blockAnims;
}

Image* Skin::getForeground()
{
	string str = "fg_" + bgSkinName;
	return Support::loadImage(str);
}

Image* Skin::getMultiplayerForeground()
{
	string str = "fgm_" + bgSkinName;	
	return Support::loadImage(str);
}

void Skin::update()
{	
	if(!animated) return;
	
	if(!animating)
	{
		float time = timer->stop();
		int calc = (int)(time * 100) % (animationWait * 100);
		if(calc > 0 && calc < 3) animating = true;
		return;
	}
	
	for(int i = 0; i < colors; i++)
	{
		int prev = blockAnims[i]->index;
		blockAnims[i]->update(6);
		int next = blockAnims[i]->index;
		
		if(prev != next && next == 0) animating = false;
	}
}

void Skin::draw(int color, IntVector2 position)
{
	if(color == 9) drawer->draw(itemBoxImg, position);
	else if(animated && animating) blockAnims[(color - 1) % colors]->draw(position);
	else drawer->draw(blockImgs[(color - 1) % colors], position);
}

Tess::Animation* Skin::getBlockClearingAnimation(Game* game, int color, IntVector2 &position)
{
	char str[2];
	sprintf(str, "%d", (color + 1));	
	string blockFile = "block_" + name + str;

	return new Tess::Animation(game, drawer, Support::loadImage(blockFile), ANIMATION_TYPE_FADEOUT, position);
}

