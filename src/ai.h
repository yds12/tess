#ifndef _TETRIS_AI
#define _TETRIS_AI

#include "piece.h"

namespace Tess
{
	class AI
	{
		private:
			Piece* workingPiece;
		
			int getColumnHeight(int** stage, int columnIndex);
			int getLowestColumn(int** stage);
			int getHighestColumn(int** stage);
			int getStageHeight(int** stage);
			bool tryConfiguration(int** stage, Piece* piece, int rotation, int shift);
			int countLines(int** stage);
			int countGaps(int** stage);
			int getPitArea(int** stage);
			void clearLine(int** stage, int line);
			int getFlatness(int** stage);
			void printMatrix(int** stage);
		
			// Utility function parameters
			float linesWeight;
			float sizeWeight;
			float gapsWeight;
			float areaWeight;
			float flatnessWeight;
		public:
			int hShift;
			int rotation;
			int speed;
			int allocationSpeed;
			float newPieceDelay;
		
			AI();
			~AI();
			void calculate(int** stage, Piece* piece);
			void printUtilityFunctionParameters();
	};
}
#endif

