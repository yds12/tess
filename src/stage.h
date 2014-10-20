#ifndef _TETRIS_STAGE
#define _TETRIS_STAGE

#include <vector>
#include "agl/color.h"
#include "agl/game.h"
#include "agl/drawer.h"
#include "support.h"
#include "piece.h"
#include "skin.h"
#include "ranking.h"
#include "namedialog.h"
#include "network.h"
#include "item.h"
#include "menu.h"
#include "ai.h"
#include "animation.h"

using namespace AGL;

namespace Tess
{
	class Stage
	{
		private:
			int stageX, stageY;
			int enemyStageX, enemyStageY;
			Game* game;
			Drawer* drawer;
			Image* gridImg;
			Image* areaImg;
			Image* fgImg;
			Image* alphaImg;
			Font* defaultFont;
			Font* panelFont;
			Font* tetrisFont;
			float pieceSpeed;
			bool pieceIsFalling;
			Timer* timer;
			int** stageMatrix;
			Piece* currentPiece;
			Piece* nextPiece;
			int lines;
			int score;
			int scoreMultiplier;
			bool paused;
			bool quitConfirmation;
			bool inputName;
			bool displayGrid;
			Ranking* ranking;
			NameDialog* dialog;
			int rank;
			bool multiplayer;
			bool netGame;
			bool win;
			bool cpu;
			int difficulty;
			int speedProgression;
			int linesToSpeedUp;
			int linesToSend;		
			int linesPanel;
			int scorePanel;
			int timeToAnimateScorePanel;
			float totalEllapsedTime;
			float lastPieceTime;
			int itemChance;
			bool drawActiveArea;
			Item* item;	
			Item* activeItem;
			Item* enemyItem;
			Menu* pauseMenu;
			Menu* quitMenu;
			AI* ai;
			vector<Tess::Animation*> blockClearingAnimations;
				
			void drawMultiplayer();
			void checkLines();
			void clearLine(int line);
			void insertLines(int lines);
			void sendMatrix();
			bool gameIsOver();
			void createItens();
			void getItem();
			void useItem();
			void finishItem();
			void notifyItem();
			void enemyUseItem(int type);
			void newPiece();
			void notifyPiece();
			void pauseOrResume(Keyboard* keyboard, Mouse* mouse);
			void drawPauseMenu();
			void animateLineClearing(int line);
		public:
			bool gameOver;
			Stage* enemyStage;
			Network* net;
			Skin* skin;
			bool end;
		
			Stage(Game* game, Drawer* drawer, bool multiplayer, bool netGame, bool cpu, int difficulty);
			~Stage();
			void update();
			void update(Keyboard* kb, Mouse* mouse);
			void draw();
			int getLinesToSend();
	};
}

#endif

