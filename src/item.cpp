#include "item.h"

using namespace Tess;

Item::Item()
{
	itemCount = 4;
	itemNames = new string[itemCount];
	itemNames[ITEM_THIEF] = "Thief";
	itemNames[ITEM_CLOUD] = "Cloud";
	itemNames[ITEM_BOMB] = "Bomb";
	itemNames[ITEM_TURTLE] = "Turtle";

	int r = rand() % itemCount;
	type = r;
	
	char file[10];
	sprintf(file, "item%d", type);		
	icon = Support::loadImage(file);
	
	char file2[10];
	sprintf(file2, "item%d", type);		
	image = Support::loadImage(file2);
	
	used = false;
	finished = false;
}

Item::Item(int type)
{
	itemCount = 4;
	itemNames = new string[itemCount];
	itemNames[ITEM_THIEF] = "Thief";
	itemNames[ITEM_CLOUD] = "Cloud";
	itemNames[ITEM_BOMB] = "Bomb";
	itemNames[ITEM_TURTLE] = "Turtle";

	this->type = type;
	
	char file[10];
	sprintf(file, "item%d", type);		
	icon = Support::loadImage(file);
	
	char file2[10];
	sprintf(file2, "item%d", type);		
	image = Support::loadImage(file2);
	
	used = false;
	finished = false;
}

Item::~Item()
{
	delete icon;
	delete image;
}

void Item::use()
{
	switch(type)
	{ 
		case ITEM_TURTLE:
			timer = new Timer();
			timer->start();
			break;
			
		case ITEM_CLOUD:
			timer = new Timer();
			timer->start();
			break;
		
		default:
			break;
	}
	
	used = true;
}

void Item::update()
{
	if(used)
	{
		float time;
		
		switch(type)
		{
			case ITEM_TURTLE:
				time = timer->stop();
				if(time > 10) finished = true;
				break;
				
			case ITEM_CLOUD:
				time = timer->stop();
				if(time > 10) finished = true;
				break;
				
			case ITEM_BOMB:
				break;
				
			default:
				finished = true;
				break;
		}
	}
}

void Item::stop(int** stageMatrix, int itemX, int itemY)
{
	switch(type)
	{
		case ITEM_BOMB:
			finished = true;
			
			for(int i = 0; i < STAGE_WIDTH; i++)
				for(int j = 0; j < STAGE_HEIGHT; j++)
					if(sqrt((itemX - i)*(itemX - i) + (itemY - j)*(itemY - j)) < 5)
					{
						stageMatrix[i][j] = 0;
					}
			break;
			
		default:
			break;
	}
}

void Item::drawIcon(Drawer* drawer, IntVector2 position)
{
	drawer->draw(icon, position);
}

void Item::drawEffect(Drawer* drawer, IntVector2 stagePosition)
{
	switch(type)
	{						
		default:
			break;
	}	
}

void Item::drawEnemyEffect(Drawer* drawer, IntVector2 stagePosition)
{
	int r;
	
	switch(type)
	{
		case ITEM_CLOUD:
			r = rand() % 100;
			for(int i = 0; i < r; i++)
			{
				IntVector2 position(rand() % 240 + stagePosition.x, rand() % 480 + stagePosition.y);
				drawer->draw(icon, position);
			}
			break;
			
		default:
			break;
	}	
}
		
