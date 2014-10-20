#include "presentation.h"

using namespace Tess;

Presentation::Presentation(Drawer* drawer, Keyboard* keyboard, Mouse* mouse)
{		
	end = false;
	ok = false;
	finishedText = false;
	
	this->drawer = drawer;
	this->keyboard = keyboard;
	this->mouse = mouse;
	
	logo = Support::loadImage("aleva");
	font = Support::presentationFont;
	font2 = Support::presentationSiteFont;
	text = "";
	timer = new Timer();
		
	bg = Graphics::getGradientRectangle(800, 600, AGL_COLOR_BLACK, AGL_COLOR_NAVY, 255, 255, true);
}

Presentation::~Presentation()
{
	delete logo;
	delete bg;
}

void Presentation::addLetter()
{
	string totalText = Lang::get(35);	
	if(text.length() == totalText.length()) 
	{
		finishedText = true;
		return;
	}
	text += totalText[text.length()];
}

void Presentation::update()
{
	if(keyboard->isKeyPressed(keyboard->getIndex(AGL::KEY_ENTER))
		|| mouse->isLeftButtonPressed()) end = true;
	
	if(timer->stop() > 0.7) ok = true;
	
	if(finishedText)
	{
		if(timer->stop() > 2) 
			end = true;
	}
	else if (ok && timer->stop() > 0.1) 
	{ 
		addLetter(); 
		timer->start(); 
	}	
}

void Presentation::draw()
{
	IntVector2 zero(0, 0);
	drawer->draw(bg, zero);
	IntVector2 v(200, 100);
	drawer->draw(logo, v);
	
	IntVector2 v1(300, 350);
	drawer->drawText(font, text.c_str(), v1, AGL_COLOR_WHITE);
	
	IntVector2 v2(280, 560);
	drawer->drawText(font2, "games.aleva.com.br", v2, AGL_COLOR_GRAY);
}


