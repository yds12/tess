#ifndef _TETRIS_NETWORK
#define _TETRIS_NETWORK

#include "agl/net.h"

namespace Tess
{
	class Network
	{
		private:
			bool server;
			Game* game;
		
		public:
			bool start;
		
			Network(Game* game, bool server);
			~Network();
			void tryConnect();
	};
}

#endif

