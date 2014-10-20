#include "support.h"

using namespace Tess;

Font* Support::menuTitleFont;
Color Support::menuTitleTextColor;
Font* Support::defaultFont;
Color Support::defaultTextColor;
Font* Support::menuFont;
Color Support::menuTextColor;
Color Support::menuSelectedTextColor;
int Support::menuFontSize;
int Support::menuLineSpacing;
bool Support::debug;
Font* Support::presentationFont;
Font* Support::presentationSiteFont;
Font* Support::tetrisFont;
Font* Support::panelFont;
Font* Support::rankingFont;
Font* Support::cancelFont;

float Support::musicVolume;
float Support::soundVolume;
Music* Support::currentMusic;
short Support::currentMusicId;
Sound** Support::sounds;

Settings Support::settings;

void Support::loadSupportContent()
{
	musicVolume = 1.0f;
	soundVolume = 1.0f;
	currentMusicId = MUSIC_NONE;
	currentMusic = NULL;
	
	sounds = new Sound*[3];
	sounds[SOUND_MENUMOVE] = new Sound(AGL_SOUND, "data/sfx/menu-move.ogg");
	sounds[SOUND_MENUCHANGE] = new Sound(AGL_SOUND, "data/sfx/menu-change.ogg");
	sounds[SOUND_MENUSELECT] = new Sound(AGL_SOUND, "data/sfx/menu-select.ogg");
	
	presentationFont = new Font("data/font/tetris.ttf", 40);
	presentationSiteFont = new Font("data/font/default.ttf", 25);
	panelFont = new Font("data/font/default.ttf", 48);
	tetrisFont = new Font("data/font/tetris.ttf", 48);
	rankingFont = new Font("data/font/default.ttf", 28);
	cancelFont = new Font("data/font/default.ttf", 40);

	menuTitleFont = new Font("data/font/tetris.ttf", 55);
	menuTitleTextColor = AGL_COLOR_WHITE;
	
	defaultFont = new Font("data/font/default.ttf", 24);
	defaultTextColor = AGL_COLOR_WHITE;
	
	menuLineSpacing = 0;
	menuFontSize = 40;
	menuFont = new Font("data/font/default.ttf", menuFontSize);
	menuTextColor = AGL_COLOR_LIGHTBLUE;
	menuSelectedTextColor = AGL_COLOR_ORANGE;
	
	settings.fullscreen = false;
	settings.skinName = "round";
	settings.bgSkinName = "default";
	settings.musicOn = true;
	settings.soundOn = true;
	settings.language = LANG_ENGLISH;
}

Image* Support::loadImage(string path)
{
	string path2 = "data/img/" + path + ".png";
	return new Image(path2.c_str());
}

void Support::playMusic(short music)
{	
	if(!settings.musicOn || music == currentMusicId) return;
	
	string path;
		
	if(music == MUSIC_DEFAULT)
		path = "teste";
	
	string path2 = "data/music/" + path + ".ogg";
	currentMusic = new Music(AGL_MUSIC, path2.c_str());
	currentMusic->play(musicVolume);
	
	currentMusicId = music;
}

void Support::stopMusic()
{
	if(currentMusic != NULL)
	{
		currentMusic->stop();
		currentMusic = NULL;
		currentMusicId = MUSIC_NONE;
	}
}

void Support::playSound(short sound)
{	
	if(!settings.soundOn) return;
			
	sounds[sound]->play(soundVolume);
}

void Support::drawShadowedText(Drawer* drawer, Font* font, const char* text, IntVector2 position, Color textColor, Color shadowColor)
{
	IntVector2 shadowPosition(position.x - 1, position.y - 1);
	drawer->drawText(font, text, shadowPosition, shadowColor);
	drawer->drawText(font, text, position, textColor);
}

Image* Support::getAlphaFrame()
{
	return Graphics::getRectangle(800, 600, AGL_COLOR_BLACK, 200);
}

