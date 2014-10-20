#ifndef _TETRIS_NETWORK
#define _TETRIS_NETWORK

#include "agl/game.h"
#include "agl/net/net.h"
#include "support.h"
#include "piece.h"

#define MSG_GAMEOVER "OVR"
#define MSG_LINE "LIN"
#define MSG_MATRIX "MTX"
#define MSG_NEWPIECE "PCE"
#define MSG_ROTATE "ROT"
#define MSG_PIECEPOS "POS"
#define MSG_ITEM "ITM"

using namespace AGL;
		
namespace Tess
{ 
	class Network
	{
		private:
			int messageSize;
			bool server;
			int linesReceived;
			Game* game;
			int enemyItem;
		
		public:
			bool connected;
			bool heLost;
			int** enemyMatrix;
			Piece* enemyPiece;
		
			Network(Game* game, bool server);
			~Network();
			void tryConnect();		
			void handleMessages();
			void sendMessage(char* msg);
			void sendMessage(string type, string msg);
			int getLinesReceived();
			int getEnemyItem();
	};
}

#endif

