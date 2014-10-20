#include "piece.h"
#include <iostream>

#define CEIL(a) ((int)(a + 0.9999))

using namespace std;
using namespace Tess;

Piece::Piece(int skinColors)
{	
	shift = 0;
	hShift = 3;
	stopped = false;
	rotation = 0;
	type = -1;
	color = -1;
	generateRandomPiece(skinColors);
	item = false;
}

Piece::Piece()
{	
	shift = 0;
	hShift = 3;
	stopped = false;
	rotation = 0;
	type = -1;
	color = -1;
	item = false;
}

void Piece::generateRandomPiece(int skinColors)
{
	int type = rand() % 7;
	int thisColor = (rand() % skinColors) + 1;
	generatePiece(type, thisColor);
}

void Piece::generateItemPiece()
{	
	// piece structure
	int pieces[1][4][4] = {
		{ 	{ 1, 0, 0, 0}, 
			{ 0, 0, 0, 0},
			{ 0, 0, 0, 0}, 
			{ 0, 0, 0, 0} } };
	
	pieceMatrix = new int*[4];
	for(int i = 0; i < 4; i++) 
	{
		pieceMatrix[i] = new int[4];
		for(int j = 0; j < 4; j++)
			 pieceMatrix[i][j] = pieces[0][i][j];
	}
	
	color = 1;
	item = true;
}

void Piece::generatePiece(int type, int color)
{
	generatePiece(type, color, 0);
}

void Piece::generatePiece(int type, int color, int rotation)
{
	this->color = color;
	this->type = type;
	
	// piece structure
	int pieces[7][4][4] = {
		{ 	{ 0, 0, 0, 0}, 
			{ 1, 1, 1, 1},
			{ 0, 0, 0, 0}, 
			{ 0, 0, 0, 0} },
		{ 	{ 0, 0, 0, 0}, 
			{ 0, 1, 1, 0},
			{ 0, 1, 1, 0}, 
			{ 0, 0, 0, 0} },
		{	{ 0, 0, 1, 0}, 
			{ 0, 1, 1, 0},
			{ 0, 1, 0, 0}, 
			{ 0, 0, 0, 0} },
		{	{ 0, 1, 0, 0}, 
			{ 0, 1, 1, 0},
			{ 0, 0, 1, 0}, 
			{ 0, 0, 0, 0} },
		{	{ 0, 0, 0, 0}, 
			{ 0, 1, 0, 0},
			{ 1, 1, 1, 0}, 
			{ 0, 0, 0, 0} },
		{	{ 0, 1, 1, 0}, 
			{ 0, 1, 0, 0},
			{ 0, 1, 0, 0}, 
			{ 0, 0, 0, 0} },
		{	{ 0, 1, 1, 0}, 
			{ 0, 0, 1, 0},
			{ 0, 0, 1, 0}, 
			{ 0, 0, 0, 0} } };
	
	pieceMatrix = new int*[4];
	for(int i = 0; i < 4; i++) 
	{
		pieceMatrix[i] = new int[4];
		for(int j = 0; j < 4; j++)
			 pieceMatrix[i][j] = pieces[type][i][j];
	}
	
	for(int i = 0; i < rotation; i++) rotate();
}

bool Piece::tryRotate(int** stage)
{
	if(item) return false;

	int** rotatedMatrix = new int*[4];	
	for(int i = 0; i < 4; i++)
	{
		rotatedMatrix[i] = new int[4];
		for(int j = 0; j < 4; j++)
			rotatedMatrix[i][j] = 0;
	}	
	
	bool canRotate = true;
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			rotatedMatrix[i][j] = pieceMatrix[j][3 - i];
			
			if(rotatedMatrix[i][j] > 0)
			{
				if(hShift + i < 0 || hShift + i >= STAGE_WIDTH) canRotate = false;
				else if(stage[hShift + i][CEIL(shift / BLOCK_SIZE) + j] > 0) canRotate = false;
			}
		}
		
	if(canRotate)
	{
		rotation++;
		rotation = rotation % 4;		
		delete pieceMatrix;
		pieceMatrix = rotatedMatrix;
	}
	
	return canRotate;
}

void Piece::rotate()
{
	int** rotatedMatrix = new int*[4];	
	for(int i = 0; i < 4; i++)
	{
		rotatedMatrix[i] = new int[4];
		for(int j = 0; j < 4; j++)
			rotatedMatrix[i][j] = 0;
	}	
	
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			rotatedMatrix[i][j] = pieceMatrix[j][3 - i];
		
	rotation++;
	rotation = rotation % 4;		
	delete pieceMatrix;
	pieceMatrix = rotatedMatrix;
}

void Piece::allocate(int** stage)
{
	while(!stopped)
	{
		shift += BLOCK_SIZE / 2;
		checkCollisions(stage);
	}
}

void Piece::checkCollisions(int** stage)
{
	for(int i = 0; i < 4; i++)
	{
		int x = (int)hShift + i;
		
		if(x < 0 || x >= STAGE_WIDTH) continue;
	
		int max = -1;
		for(int j = 0; j < 4; j++)
			if(pieceMatrix[i][j] > 0) max = j;
		
		if(max == -1) continue;
		
		float y = (shift / BLOCK_SIZE) + (max + 1);
		
		if(y >= STAGE_HEIGHT || stage[x][(int)y] > 0)
		{
			stop(stage);
			break;
		}
	}
}

void Piece::tryMoveLeft(int** stage)
{
	bool canMove = true;
	
	for(int j = 0; j < 4; j++)
	{
		int min = 4;
		for(int i = 3; i >= 0; i--)
			if(pieceMatrix[i][j] > 0) min = i;

		if(min == 4) continue;

		int x = (int)hShift + min;
		int y = (shift / BLOCK_SIZE) + j;
		
		if(x <= 0) canMove = false;		
		else 
		{
			if(stage[x - 1][y] > 0 && (int)shift % BLOCK_SIZE < 16) canMove = false;
			if(y < STAGE_HEIGHT - 1 && stage[x - 1][y + 1] > 0) canMove = false;
		}
	}
	
	if(canMove) hShift -= 1;
}

void Piece::tryMoveRight(int** stage)
{
	bool canMove = true;
	
	for(int j = 0; j < 4; j++)
	{
		int max = -1;
		for(int i = 0; i < 4; i++)
			if(pieceMatrix[i][j] > 0) max = i;

		if(max == -1) continue;

		int x = (int)hShift + (max + 1);
		int y = (shift / BLOCK_SIZE) + j;
		
		if(x >= STAGE_WIDTH) canMove = false;
		else 
		{
			if(stage[x][y] > 0 && (int)shift % BLOCK_SIZE < 16) canMove = false;
			if(y < STAGE_HEIGHT - 1 && stage[x][y + 1] > 0) canMove = false;
		}		
	}
	
	if(canMove) hShift += 1; 
}

void Piece::stop(int** stage)
{
	stopped = true;
	
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			if(pieceMatrix[i][j] > 0)
				stage[i + hShift][j + (int)(shift / BLOCK_SIZE)] = color;
}

int Piece::left()
{
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			if(pieceMatrix[i][j] > 0) return i;
}

int Piece::right()
{
	for(int i = 3; i >= 0; i--)
		for(int j = 0; j < 4; j++)
			if(pieceMatrix[i][j] > 0) return i;
}

int Piece::top()
{
	for(int j = 0; j < 4; j++)
		for(int i = 0; i < 4; i++)
			if(pieceMatrix[i][j] > 0) return j;
}

int Piece::bottom()
{
	for(int j = 3; j >= 0; j--)
		for(int i = 0; i < 4; i++)
			if(pieceMatrix[i][j] > 0) return j;
}

