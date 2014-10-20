#include "options.h"

using namespace Tess;

Options::Options(Game* game, Drawer* drawer, Keyboard* keyboard, Mouse* mouse)
{
	this->game = game;
	this->drawer = drawer;
	this->keyboard = keyboard;
	this->mouse = mouse;
	
	numberOfOptions = 6;
	
	string* options = new string[numberOfOptions + 1];
	options[OPTIONS_SCREEN] = Lang::get(19);
	options[OPTIONS_MUSIC] = Lang::get(20);
	options[OPTIONS_SOUND] = Lang::get(21);
	options[OPTIONS_LANGUAGE] = Lang::get(22);
	options[OPTIONS_SKIN] = Lang::get(23);
	options[OPTIONS_BGSKIN] = Lang::get(58);
	options[OPTIONS_BACK] = Lang::get(24);
	menu = new Menu(options, numberOfOptions + 1, drawer, keyboard, mouse);
	menu->set(150, 300);

	currentOption = new int[numberOfOptions];
	loadInMemory();
	
	optionSize = new int[numberOfOptions];
	optionName = new string*[numberOfOptions];
	
	optionSize[OPTIONS_SCREEN] = 2;	
	optionName[OPTIONS_SCREEN] = new string[optionSize[OPTIONS_SCREEN]];
	optionName[OPTIONS_SCREEN][0] = Lang::get(25);
	optionName[OPTIONS_SCREEN][1] = Lang::get(26);
	
	optionSize[OPTIONS_MUSIC] = 2;	
	optionName[OPTIONS_MUSIC] = new string[optionSize[OPTIONS_MUSIC]];
	optionName[OPTIONS_MUSIC][0] = Lang::get(27);
	optionName[OPTIONS_MUSIC][1] = Lang::get(28);
	
	optionSize[OPTIONS_SOUND] = 2;	
	optionName[OPTIONS_SOUND] = new string[optionSize[OPTIONS_SOUND]];
	optionName[OPTIONS_SOUND][0] = Lang::get(27);
	optionName[OPTIONS_SOUND][1] = Lang::get(28);
	
	optionSize[OPTIONS_LANGUAGE] = 3;	
	optionName[OPTIONS_LANGUAGE] = new string[optionSize[OPTIONS_LANGUAGE]];
	optionName[OPTIONS_LANGUAGE][0] = "english";
	optionName[OPTIONS_LANGUAGE][1] = "português";
	optionName[OPTIONS_LANGUAGE][2] = "español";
	
	optionSize[OPTIONS_SKIN] = 5;	
	optionName[OPTIONS_SKIN] = new string[optionSize[OPTIONS_SKIN]];
	optionName[OPTIONS_SKIN][0] = Lang::get(29);
	optionName[OPTIONS_SKIN][1] = Lang::get(30);
	optionName[OPTIONS_SKIN][2] = Lang::get(31);
	optionName[OPTIONS_SKIN][3] = Lang::get(32);
	optionName[OPTIONS_SKIN][4] = Lang::get(33);
	
	optionSize[OPTIONS_BGSKIN] = 3;	
	optionName[OPTIONS_BGSKIN] = new string[optionSize[OPTIONS_BGSKIN]];
	optionName[OPTIONS_BGSKIN][0] = Lang::get(59);
	optionName[OPTIONS_BGSKIN][1] = Lang::get(60);
	optionName[OPTIONS_BGSKIN][2] = Lang::get(61);

	end = false;	
	bg = Support::getAlphaFrame();
}

Options::~Options()
{
	
}

void Options::update()
{
	menu->update();
	
	if(menu->optionChosen)
	{
		if(menu->selected == OPTIONS_BACK) 
		{
			end = true;
			save();
		}
		else nextOption(menu->selected);
	}
	else
	{
		if(menu->selected != OPTIONS_BACK)
		{
			if(keyboard->isKeyPressed(keyboard->getIndex(KEY_RIGHT))
				|| keyboard->isKeyHeld(keyboard->getIndex(KEY_RIGHT)))
				nextOption(menu->selected);
			if(keyboard->isKeyPressed(keyboard->getIndex(KEY_LEFT))
				|| keyboard->isKeyHeld(keyboard->getIndex(KEY_LEFT)))
				previousOption(menu->selected);
		}
	}
}

void Options::nextOption(int option)
{
	if(currentOption[option] < optionSize[option] - 1)
	{
		currentOption[option]++;
	}
	else currentOption[option] = 0;

	menu->reset();
	menu->selected = option;
}

void Options::previousOption(int option)
{
	if(currentOption[option] > 0)
	{
		currentOption[option]--;
	}
	else currentOption[option] = optionSize[option] - 1;

	menu->reset();
	menu->selected = option;
}

void Options::loadInMemory()
{
	currentOption[OPTIONS_SCREEN] = (Support::settings.fullscreen ? 1 : 0);
	currentOption[OPTIONS_MUSIC] = (Support::settings.musicOn ? 0 : 1);
	currentOption[OPTIONS_SOUND] = (Support::settings.soundOn ? 0 : 1);
	currentOption[OPTIONS_LANGUAGE] = Support::settings.language;
	
	if(Support::settings.skinName == "round") currentOption[OPTIONS_SKIN] = 0;
	else if(Support::settings.skinName == "roundblack") currentOption[OPTIONS_SKIN] = 1;
	else if(Support::settings.skinName == "color") currentOption[OPTIONS_SKIN] = 2;
	else if(Support::settings.skinName == "black") currentOption[OPTIONS_SKIN] = 3;
	else if(Support::settings.skinName == "jewel") currentOption[OPTIONS_SKIN] = 4;
	
	if(Support::settings.bgSkinName == "default") currentOption[OPTIONS_BGSKIN] = 0;
	else if(Support::settings.bgSkinName == "dark") currentOption[OPTIONS_BGSKIN] = 1;
	else if(Support::settings.bgSkinName == "female") currentOption[OPTIONS_BGSKIN] = 2;
}

void Options::load()
{
	int opt_screen = 0;
	int opt_music = 0;
	int opt_sound = 0;
	int opt_lang = 0;
	int opt_skin = 0;
	int opt_bgskin = 0;
	
	if(FileManipulator::exists(OPTIONS_FILE))
	{
		FileManipulator f(OPTIONS_FILE);
		f.openReader();
	
		if(f.ready())
		{
			opt_screen = Format::stringToInt(f.readLine());
			opt_music = Format::stringToInt(f.readLine());
			opt_sound = Format::stringToInt(f.readLine());
			opt_lang = Format::stringToInt(f.readLine());
			opt_skin = Format::stringToInt(f.readLine());
			opt_bgskin = Format::stringToInt(f.readLine());
		}
	}

	Support::settings.fullscreen = (opt_screen == 0 ? false : true);
	Support::settings.language = opt_lang;
	Support::settings.musicOn = (opt_music == 0 ? true : false);
	Support::settings.soundOn = (opt_sound == 0 ? true : false);
	
	if(opt_skin == 0) Support::settings.skinName = "round";
	else if(opt_skin == 1) Support::settings.skinName = "roundblack";
	else if(opt_skin == 2) Support::settings.skinName = "color";
	else if(opt_skin == 3) Support::settings.skinName = "black";
	else if(opt_skin == 4) Support::settings.skinName = "jewel";
	
	if(opt_bgskin == 0) Support::settings.bgSkinName = "default";
	else if(opt_bgskin == 1) Support::settings.bgSkinName = "dark";
	else if(opt_bgskin == 2) Support::settings.bgSkinName = "female";
		
	if(Support::settings.musicOn)
		Support::playMusic(MUSIC_DEFAULT);
	else
		Support::stopMusic();
}

void Options::save()
{
	FileManipulator f(OPTIONS_FILE);	
	f.openWriter();
	
	if(f.ready())
	{	
		for(int i = 0; i < numberOfOptions; i++)
		{			
			char str1[6];
			sprintf(str1, "%d", currentOption[i]);		
			f.writeLine(str1);
		}
	}	
	
	f.closeWriter();
}

void Options::draw()
{
	drawer->clearScreen(AGL_COLOR_NAVY);
	IntVector2 zero(0, 0);
	drawer->draw(bg, zero);

	IntVector2 v(30, 30);
	drawer->drawText(Support::menuTitleFont, Lang::get(34), v, Support::menuTitleTextColor);
	
	for(int i = 0; i < numberOfOptions; i++)
	{
		IntVector2 v2(450, 300 + i * (Support::menuFontSize + Support::menuLineSpacing));
		drawer->drawText(Support::menuFont, (optionName[i][currentOption[i]]).c_str(), v2, AGL_COLOR_GREEN);
	}
	
	menu->draw();
}
