#include "network.h"

using namespace Tess;

Network::Network(Game* game, bool server)
{
	this->server = server;
	this->game = game;
	start = false;
	
	if(server)
	{
		if(AGL::Net::initializeNetwork(false)) printf("initialized\n");
		else printf("initialization failed!\n");
	
		AGL::Net::players[0].connected = true;
		AGL::Net::players[0].server = true;
		AGL::Net::players[0].active = true;
		
		AGL::Net::enableInternalMessageHandling(false, true, false);
	}
	else
	{
		if(AGL::Net::initializeNetwork(true)) printf("initialized\n");
		else printf("initialization failed!\n");
	
		AGL::Net::enableInternalMessageHandling(true, false, true);
	}
}

Network::~Network()
{
	
}

void Network::tryConnect()
{
	char buf[100];
	AGL::NetPlayer player;
	
	if(server)
	{
		if(AGL::Net::readMessage(buf, &player)) AGL::Net::handleInternalMessage(buf, player);		
		if(AGL::Net::players[1].connected) start = true;
	}
	else
	{
		AGL::Net::broadcastMessage(AGLNET_MSG_CLIENT);
		game->wait(50);
		if(AGL::Net::readMessage(buf, &player)) AGL::Net::handleInternalMessage(buf, player);
	}
}

