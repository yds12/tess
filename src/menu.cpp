#include "menu.h"

using namespace Tess;

Menu::Menu(string* options, int numberOfOptions, Drawer* drawer, Keyboard* keyboard, Mouse* mouse)
{
	this->options = options;
	this->numberOfOptions = numberOfOptions;
	this->drawer = drawer;
	this->keyboard = keyboard;
	this->mouse = mouse;
		
	optionSpacing = 0;
	selected = 0;
	x = 200;
	y = 300;
	width = 200;
	
	area.x = x;
	area.y = y;
	area.width = width;
	area.height = numberOfOptions * Support::menuFontSize + (numberOfOptions - 1) * Support::menuLineSpacing;

	optionChosen = false;
}

void Menu::update()
{	
	if(keyboard->isKeyPressed(keyboard->getIndex(KEY_ENTER))
		|| (mouse->isLeftButtonPressed() && mouse->isCursorOver(area))) 
	{
		optionChosen = true;
		Support::playSound(SOUND_MENUSELECT);
	}
	
	if(optionChosen) return;
	
	if(keyboard->isKeyPressed(keyboard->getIndex(KEY_DOWN))
		|| keyboard->isKeyHeld(keyboard->getIndex(KEY_DOWN)))
	{
		if(selected < numberOfOptions - 1) selected++;		
		Support::playSound(SOUND_MENUMOVE);
	}
	else if(keyboard->isKeyPressed(keyboard->getIndex(KEY_UP))
		|| keyboard->isKeyHeld(keyboard->getIndex(KEY_UP)))
	{
		if(selected > 0) selected--;		
		Support::playSound(SOUND_MENUMOVE);
	}
	else if(mouse->position->x >= x && mouse->position->x <= x + width)
	{
		for(int i = 0; i < numberOfOptions; i++)
		{
			if(mouse->position->y > y + i * (Support::menuFontSize + optionSpacing) 
				&& mouse->position->y < y + i * (Support::menuFontSize + optionSpacing) + Support::menuFontSize)
			{
				if(selected != i) 
					Support::playSound(SOUND_MENUMOVE);
				
				selected  = i;
			}
		}
	}
}

void Menu::set(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Menu::set(int x, int y, int width)
{
	this->x = x;
	this->y = y;
	this->width = width;
	area.width = width;
}

void Menu::reset()
{
	optionChosen = false;
	selected = 0;
}

void Menu::draw()
{
	for(int i = 0; i < numberOfOptions; i++)
	{
		IntVector2 v(x, y + i * (Support::menuFontSize + optionSpacing));
		
		if(selected == i)	
			drawer->drawText(Support::menuFont, Format::stringToChar(&(options[i])), v, Support::menuSelectedTextColor);
		else
			drawer->drawText(Support::menuFont, Format::stringToChar(&(options[i])), v, Support::menuTextColor);
	}
}

