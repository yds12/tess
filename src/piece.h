#ifndef _TETRIS_PIECE
#define _TETRIS_PIECE

#include "support.h"

namespace Tess
{
	class Piece
	{
		private:
		
		public:
			int** pieceMatrix;
			float shift;
			int hShift;
			bool stopped;
			int color;
			int type;
			int rotation;
			bool item;
		
			Piece();
			Piece(int skinColors);
			void generateRandomPiece(int skinColors);
			void generatePiece(int type, int color);
			void generatePiece(int type, int color, int rotation);
			void generateItemPiece();
			void rotate();
			bool tryRotate(int** stage);
			void allocate(int** stage);
			void checkCollisions(int** stage);
			void tryMoveLeft(int** stage);
			void tryMoveRight(int** stage);
			void stop(int** stage);
			int left();
			int right();
			int top();
			int bottom();
	};
}

#endif

