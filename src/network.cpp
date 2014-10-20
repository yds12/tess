#include "network.h"

using namespace Tess;

Network::Network(Game* game, bool server)
{
	this->server = server;
	this->game = game;
	connected = false;
	heLost = false;
	linesReceived = 0;
	messageSize = STAGE_WIDTH * STAGE_HEIGHT + 10;
	
	enemyPiece = NULL;
	enemyItem = -1;
	
	enemyMatrix = new int*[STAGE_WIDTH];
	for(int i = 0; i < STAGE_WIDTH; i++) 
	{
		enemyMatrix[i] = new int[STAGE_HEIGHT];
		
		for(int j = 0; j < STAGE_HEIGHT; j++)
			enemyMatrix[i][j] = 0;
	}
	
	if(server)
	{
		if(AGL::Net::initializeNetwork(false)) { }
		else printf("network initialization failed!\n");
	
		AGL::Net::players[0].connected = true;
		AGL::Net::players[0].server = true;
		AGL::Net::players[0].active = true;
		
		AGL::Net::enableInternalMessageHandling(false, true, false);
	}
	else
	{
		if(AGL::Net::initializeNetwork(true)) { }
		else printf("network initialization failed!\n");
	
		AGL::Net::enableInternalMessageHandling(true, false, true);
	}
}

Network::~Network()
{
	AGL::Net::closeNetwork();
}

void Network::tryConnect()
{
	char buf[messageSize];
	AGL::NetPlayer player;
	
	if(server)
	{
		if(AGL::Net::readMessage(buf, messageSize, &player)) AGL::Net::handleInternalMessage(buf, player);		
		if(AGL::Net::players[1].connected) connected = true;
	}
	else
	{
		AGL::Net::broadcastMessage(AGLNET_MSG_CLIENT);
		game->wait(50);
		if(AGL::Net::readMessage(buf, messageSize, &player)) AGL::Net::handleInternalMessage(buf, player);
		if(AGL::Net::players[0].connected) connected = true;
	}
	
	if(connected) AGL::Net::enableInternalMessageHandling(false, false, false);
}

void Network::handleMessages()
{
	char message[messageSize];
	AGL::NetPlayer player;
	
	while(AGL::Net::readMessage(message, messageSize, &player))
	{
		char type[20];
		sscanf(message, "%s", &type);

		if(AGL::Format::compareChars(type, (char*)MSG_GAMEOVER))
		{
			heLost = true;
		}
		else if(AGL::Format::compareChars(type, (char*)MSG_LINE))
		{
			int lin;
			sscanf(message, "%s %d", &type, &lin);
			linesReceived += lin;
		}
		else if(AGL::Format::compareChars(type, (char*)MSG_MATRIX))
		{
			char matrix[STAGE_WIDTH * STAGE_HEIGHT];
			sscanf(message, "%s %s", &type, &matrix);
			
			for(int i = 0; i < STAGE_WIDTH * STAGE_HEIGHT; i++)
			{
				char c = matrix[i];
				enemyMatrix[i % STAGE_WIDTH][i / STAGE_WIDTH] = c - '0';
			}
		}
		else if(AGL::Format::compareChars(type, (char*)MSG_NEWPIECE))
		{
			int ptype, color, item;
			sscanf(message, "%s %d %d %d", &type, &ptype, &color, &item);
			
			delete enemyPiece;
			enemyPiece = new Piece();
			if(item == 0) enemyPiece->generatePiece(ptype, color);
			else enemyPiece->generateItemPiece();
		}
		else if(AGL::Format::compareChars(type, (char*)MSG_PIECEPOS))
		{
			int x, y;
			sscanf(message, "%s %d %d", &type, &x, &y);
			
			if(enemyPiece != NULL)
			{
				enemyPiece->hShift = x;
				enemyPiece->shift = y;
			}
		}
		else if(AGL::Format::compareChars(type, (char*)MSG_ROTATE))
		{
			if(enemyPiece != NULL)
			{
				enemyPiece->rotate();
			}
		}
		else if(AGL::Format::compareChars(type, (char*)MSG_ITEM))
		{
			int itemType;
			sscanf(message, "%s %d", &type, &itemType);
			
			enemyItem = itemType;
		}
	}
}

void Network::sendMessage(char* msg)
{
	if(server) AGL::Net::sendMessage(msg, messageSize, 1);
	else AGL::Net::sendMessage(msg, messageSize, 0);
}

void Network::sendMessage(string type, string msg)
{	
	char buffer[messageSize];
	sprintf(buffer, "%s %s", type.c_str(), msg.c_str());
	sendMessage(buffer);
}

int Network::getLinesReceived()
{
	int lin = linesReceived;
	linesReceived = 0;
	return lin;
}

int Network::getEnemyItem()
{
	int item = enemyItem;
	enemyItem = -1;
	return item;
}

