#ifndef _TETRIS_SUPPORT
#define _TETRIS_SUPPORT

#include "agl/image.h"
#include "agl/format.h"
#include "agl/drawer.h"
#include "agl/font.h"
#include "agl/support.h"
#include "agl/color.h"
#include "agl/music.h"
#include "agl/sound.h"
#include "agl/graphics/graphics.h"
#include "lang.h"

#define STAGE_WIDTH	10
#define STAGE_HEIGHT	24
#define BLOCK_SIZE	24

#define DIFFICULTY_EASY			0
#define DIFFICULTY_MODERATE	1
#define DIFFICULTY_HARD			2
#define DIFFICULTY_BACK			3

#define LANG_ENGLISH			0
#define LANG_PORTUGUESE		1
#define LANG_SPANISH			2

#define MUSIC_NONE			0
#define MUSIC_DEFAULT			1

#define SOUND_NONE			0
#define SOUND_MENUMOVE		1
#define SOUND_MENUCHANGE		2
#define SOUND_MENUSELECT		3

using namespace AGL;

namespace Tess
{
	struct Settings
	{
		bool fullscreen;
		const char* skinName;
		const char* bgSkinName;
		int language;
		bool musicOn;
		bool soundOn;
	};

	class Support
	{
		private:
			static short currentMusicId;
			static Music* currentMusic;
			static float musicVolume;
			static float soundVolume;
			static Sound** sounds;
			
		public:			
			static Font* menuTitleFont;
			static Color menuTitleTextColor;
			static Font* defaultFont;
			static Color defaultTextColor;
			static Font* menuFont;
			static Color menuTextColor;
			static Color menuSelectedTextColor;
			static int menuFontSize;
			static int menuLineSpacing;
			static Font* presentationFont;
			static Font* presentationSiteFont;
			static Font* tetrisFont;
			static Font* panelFont;
			static Font* rankingFont;
			static Font* cancelFont;
			static bool debug;
		
			static Settings settings;
	
			static void loadSupportContent();
			static Image* loadImage(string path);
			static void playMusic(short music);
			static void stopMusic();
			static void playSound(short sound);
			static void drawShadowedText(Drawer* drawer, Font* font, const char* text, IntVector2 position,  Color textColor, Color shadowColor);
			static Image* getAlphaFrame();
	};
}

#endif

