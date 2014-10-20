#include "ai.h"

using namespace Tess;

AI::AI()
{
	workingPiece = NULL;
	
	//if(Support::debug) cout << "AI initialized" << endl;
}

AI::~AI()
{
	//if(Support::debug) cout << "AI destroyed" << endl;
}

void AI::printUtilityFunctionParameters()
{
	cout << "Utility Function Parameters:" << endl;
	cout << "linesWeight: " << linesWeight << endl;
	cout << "sizeWeight: " << sizeWeight << endl;
	cout << "gapsWeight: " << gapsWeight << endl;
	cout << "areaWeight: " << areaWeight << endl;
	cout << "flatnessWeight: " << flatnessWeight << endl;
	cout << "speed: " << speed << endl;
	cout << "allocationSpeed: " << allocationSpeed << endl;
}

void AI::calculate(int** stage, Piece* piece)
{
	//if(Support::debug) cout << "AI calculating for piece type " << piece->type << endl;
	
	int currentHeight = getStageHeight(stage);
	
	int bestLines = 0;
	int bestLinesR = 0;
	int bestLinesS = 0;
	int bestSize = 1000;
	int bestSizeR = 0;
	int bestSizeS = 0;
	int bestGaps = 1000;
	int bestGapsR = 0;
	int bestGapsS = 0;
	int bestArea = 1000;
	int bestAreaR = 0;
	int bestAreaS = 0;
	int bestFlatness = 1000;
	int bestFlatnessR = 0;
	int bestFlatnessS = 0;
	
	float bestUt = -1000;
	int bestUtR = 0;
	int bestUtS = 0;
	
	// Utility function configuration	
	linesWeight = 4;	//4
	sizeWeight = 0.1;	//0.5
	gapsWeight = 1.95;		//0.3
	areaWeight = 0.08;	//0.05
	flatnessWeight = 0.2;	//0.05
	
	speed = 9;//9
	allocationSpeed = 25;//25
	newPieceDelay = 1.8;//1.8
	
	if(currentHeight > 11)
	{
		speed = 20;
		newPieceDelay = 0.5;
	}
	
	for(int r = 0; r < 4; r++)
	{
		for(int s = -3; s < 10; s++)
		{
			int** stage2 = new int*[STAGE_WIDTH];
			
			for(int i = 0; i < STAGE_WIDTH; i++) stage2[i] = new int[STAGE_HEIGHT];
			
			for(int i = 0; i < STAGE_WIDTH; i++)
				for(int j = 0; j < STAGE_HEIGHT; j++)
					stage2[i][j] = stage[i][j];
			
			if(tryConfiguration(stage2, piece, r, s))
			{
				int lines = countLines(stage2);
				int size = getStageHeight(stage2) - getStageHeight(stage);
				int gaps = countGaps(stage2) - countGaps(stage);
				int area = getPitArea(stage2) - getPitArea(stage);
				int flatness = getFlatness(stage2) - getFlatness(stage);
				float ut = lines * linesWeight - ((float)size * sizeWeight) 
								- ((float)gaps * gapsWeight) - ((float)area * areaWeight)
								- ((float)flatness * flatnessWeight);
			
				if(lines > bestLines)
				{
					bestLines = lines;
					bestLinesR = r;
					bestLinesS = s;
				}
			
				if(size < bestSize)
				{
					bestSize = size;
					bestSizeR = r;
					bestSizeS = s;
				}
			
				if(gaps < bestGaps)
				{
					bestGaps = gaps;
					bestGapsR = r;
					bestGapsS = s;
				}
			
				if(area < bestArea)
				{
					bestArea = area;
					bestAreaR = r;
					bestAreaS = s;
				}
			
				if(flatness < bestFlatness)
				{
					bestFlatness = flatness;
					bestFlatnessR = r;
					bestFlatnessS = s;
				}
			
				if(ut > bestUt)
				{
					bestUt = ut;
					bestUtR = r;
					bestUtS = s;
				}
			}
			
			for(int i = 0; i < STAGE_WIDTH; i++)
				delete[]stage2[i];
				
			delete[]stage2;
		}
	}
//	if(bestLines > 0)
//	{
//		rotation = bestLinesR;
//		hShift = bestLinesS;
//	}
//	else
//	{
		rotation = bestUtR;
		hShift = bestUtS;
//	}
		
//	if(debug) cout << "AI calculated: bestLines = " << bestLines << ", bestSize = " << bestSize 
//						<< ", bestGaps = " << bestGaps << ", bestUt = " << bestUt << endl;
//	if(debug) cout << "AI calculated: H shift = " << hShift << ", rotation = " << rotation << endl;
}

int AI::countLines(int** stage)
{
	int turnLines = 0;
	for(int j = STAGE_HEIGHT - 1; j >= 0; j--)
	{
		bool line = true;
		for(int i = 0; i < STAGE_WIDTH; i++)
		{
			if(stage[i][j] == 0) 
			{
				line = false;
				break;
			}
		}
		
		if(line) 
		{
			turnLines++;
			clearLine(stage, j);
			j++;
		}
	}
	
	return turnLines;
}

void AI::clearLine(int** stage, int line)
{	
	for(int j = line; j > 0; j--)
	{
		for(int i = 0; i < STAGE_WIDTH; i++)
			stage[i][j] = stage[i][j - 1];
	}
	
	for(int i = 0; i < STAGE_WIDTH; i++) stage[i][0] = 0;
}

bool AI::tryConfiguration(int** stage, Piece* piece, int rotation, int shift)
{
	//if(debug) cout << "AI testing rotation = " << rotation << " and shift = " << shift << endl;

	if(workingPiece != NULL) delete workingPiece;
	workingPiece = new Piece();
	workingPiece->generatePiece(piece->type, piece->color, rotation);
	
	if(workingPiece->left() + shift < 0) return false;
	if(workingPiece->right() + shift >= STAGE_WIDTH) return false;
	
	workingPiece->hShift = shift;	
	workingPiece->allocate(stage);
	
	return true;
}

int AI::getLowestColumn(int** stage)
{
	int lowestCol = 0;
	int lowestColVal = -1;
	for(int i = 0; i < STAGE_WIDTH; i++)
	{
		bool empty = true;
		for(int j = 0; j < STAGE_HEIGHT; j++)
		{
			if(stage[i][j] > 0)
			{
				empty = false;
				if(j > lowestColVal) 
				{
					lowestColVal = j;
					lowestCol = i;
				}
				break;
			}
		}
		
		if(empty)
		{
			lowestColVal = 0;
			lowestCol = i;
			break;
		}
	}

	//if(debug) cout << "AI: lowest column: " << lowestCol << endl;
	return lowestCol;
}

int AI::getHighestColumn(int** stage)
{
	int highestCol = 0;
	int highestColVal = 1000;
	for(int i = 0; i < STAGE_WIDTH; i++)
	{
		bool empty = true;
		for(int j = 0; j < STAGE_HEIGHT; j++)
		{
			if(stage[i][j] > 0)
			{
				empty = false;
				if(j < highestColVal) 
				{
					highestColVal = j;
					highestCol = i;
				}
				break;
			}
		}
		
		if(empty && STAGE_HEIGHT - 1 < highestColVal)
		{
			highestColVal = STAGE_HEIGHT - 1;
			highestCol = i;
		}
	}

	//if(debug) cout << "AI: highest column: " << highestCol << endl;
	return highestCol;
}

int AI::countGaps(int** stage)
{
	int gaps = 0;
	for(int i = 0; i < STAGE_WIDTH; i++)
	{
		bool found = false;
		for(int j = 0; j < STAGE_HEIGHT; j++)
		{
			if(found && stage[i][j] == 0) gaps++;
			if(stage[i][j] > 0) found = true;
		}
	}
	
	return gaps;
}

int AI::getStageHeight(int** stage)
{
	int highestCol = 0;
	int highestColVal = 1000;
	for(int i = 0; i < STAGE_WIDTH; i++)
	{
		bool empty = true;
		for(int j = 0; j < STAGE_HEIGHT; j++)
		{
			if(stage[i][j] > 0)
			{
				empty = false;
				if(j < highestColVal) 
				{
					highestColVal = j;
					highestCol = i;
				}
				break;
			}
		}
		
		if(empty && STAGE_HEIGHT - 1 < highestColVal)
		{
			highestColVal = STAGE_HEIGHT;
			highestCol = i;
		}
	}

	//if(debug) cout << "AI: stage height: " << highestCol << endl;
	return STAGE_HEIGHT - highestColVal;
}

int AI::getFlatness(int** stage)
{
	int flatness = 0;
	int lastLevel = 0;

	for(int i = 0; i < STAGE_WIDTH; i++)
	{
		int colHeight = getColumnHeight(stage, i);
	
		if(i > 0) flatness += abs(colHeight - lastLevel);
		
		//if(Support::debug) cout << "lastLevel: " << lastLevel << " colHeight: " << colHeight << endl;
		
		lastLevel = colHeight;
	}
	
	/*if(Support::debug)
	{
		printMatrix(stage);
		cout << "flatness: " << flatness << endl << endl;
	}*/
	
	return flatness;
}

int AI::getPitArea(int** stage)
{
	int height = getStageHeight(stage);
	int area = 0;
	
	for(int i = 0; i < STAGE_WIDTH; i++)
	{
		for(int j = STAGE_HEIGHT - height; j < STAGE_HEIGHT; j++)
		{
			if(stage[i][j] > 0) break;
			else area++;
		}
	}
	
	return area;
}

int AI::getColumnHeight(int** stage, int columnIndex)
{
	int height = 0;
	
	for(int j = 0; j < STAGE_HEIGHT; j++)
	{
		if(stage[columnIndex][j] > 0) 
		{
			height = STAGE_HEIGHT - j;
			break;
		}
	}
	
	return height;
}

void AI::printMatrix(int** stage)
{	
	for(int j = 0; j < STAGE_HEIGHT; j++)
	{
		for(int i = 0; i < STAGE_WIDTH; i++)
		{
			if(stage[i][j] > 0) cout << "X";
			else cout << "O";
		}
		cout << endl;
	}
}

