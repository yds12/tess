#include "namedialog.h"

using namespace Tess;

NameDialog::NameDialog(Drawer* drawer, Mouse* mouse)
{
	this->drawer = drawer;
	this->mouse = mouse;
	keyboard = new Keyboard(true, true, false, true, false, true); // letras, números, especiais e outras
	keyboard->heldRepeat = 1;
	keyboard->heldDelay = 15;
	chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	name = "";
	fgImg = Support::getAlphaFrame();
	txtImg = Support::loadImage("textbox");
	exit = false;
	font = Support::defaultFont;
	timer = new Timer();
	timer->start();
}

NameDialog::~NameDialog()
{
	delete fgImg;
}

void NameDialog::update()
{
	keyboard->update();

	for(int i = 0; i < 36; i++)
	{
		if((keyboard->isKeyPressed(i) || keyboard->isKeyHeld(i))
			&& name.length() < 15) name += chars[i];
	}
	
	if((keyboard->isKeyPressed(keyboard->getIndex(KEY_SPACE)) || keyboard->isKeyHeld(keyboard->getIndex(KEY_SPACE)))
			&& name.length() < 15 && name.length() > 0) name += " ";
	
	if((keyboard->isKeyPressed(keyboard->getIndex(KEY_BACKSPACE)) || keyboard->isKeyHeld(keyboard->getIndex(KEY_BACKSPACE)))
		&& name.length() > 0) name = name.substr(0, name.length() - 1);
		
	if(keyboard->isKeyPressed(keyboard->getIndex(KEY_ENTER)) 
		|| mouse->isLeftButtonPressed()) exit = true;
}

void NameDialog::draw()
{
	IntVector2 zero(0, 0);
	drawer->draw(fgImg, zero);

	string text = name;
	if(((int)(timer->stop() * 10)) % 4 < 2) text += "_";

	IntVector2 v1(240, 250);
	drawer->draw(txtImg, v1);
	
	IntVector2 v(273, 272);
	drawer->drawText(font, text.c_str(), v, AGL_COLOR_BLACK);
	IntVector2 v2(200, 235);
	drawer->drawText(font, Lang::get(11), v2, AGL_COLOR_WHITE);
}

