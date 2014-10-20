# Makefile
# Author: Yuri David Santos
# Since: 14/05/2012
# Modified: 14/04/2013
# Version: 0.8

# Variables
Codes=game.cpp support.cpp stage.cpp piece.cpp skin.cpp presentation.cpp menu.cpp ranking.cpp namedialog.cpp network.cpp \
	netroom.cpp difficulty.cpp rankingscreen.cpp options.cpp lang.cpp item.cpp ai.cpp animation.cpp
Objs=game.o support.o stage.o piece.o skin.o presentation.o menu.o ranking.o namedialog.o network.o netroom.o difficulty.o \
	rankingscreen.o options.o lang.o item.o ai.o animation.o
Headers=support.h stage.h piece.h skin.h presentation.h menu.h ranking.h namedialog.h network.h netroom.h difficulty.h \
	rankingscreen.h options.h lang.h item.h ai.h animation.h
DebugObjs=game.d.o support.d.o stage.d.o piece.d.o skin.d.o presentation.d.o menu.d.o ranking.d.o namedialog.d.o \
	network.d.o netroom.d.o	difficulty.d.o rankingscreen.d.o options.d.o lang.d.o item.d.o ai.d.o animation.d.o
Libs=-lagl -lagl-graphics -lagl-net

# Build the game
game: $(Objs)
	g++ -o tess $(Objs) $(Libs)

# Build the game in debug mode
debug: $(DebugObjs)
	g++ -g -o tess $(Objs) $(Libs)
	
# Build the objects
game.o: game.cpp
	g++ -fPIC -c game.cpp
support.o: support.cpp
	g++ -fPIC -c support.cpp
stage.o: stage.cpp
	g++ -fPIC -c stage.cpp
piece.o: piece.cpp
	g++ -fPIC -c piece.cpp
skin.o: skin.cpp
	g++ -fPIC -c skin.cpp
presentation.o: presentation.cpp
	g++ -fPIC -c presentation.cpp
menu.o: menu.cpp
	g++ -fPIC -c menu.cpp
ranking.o: ranking.cpp
	g++ -fPIC -c ranking.cpp
namedialog.o: namedialog.cpp
	g++ -fPIC -c namedialog.cpp
network.o: network.cpp
	g++ -fPIC -c network.cpp
netroom.o: netroom.cpp
	g++ -fPIC -c netroom.cpp
difficulty.o: difficulty.cpp
	g++ -fPIC -c difficulty.cpp
rankingscreen.o: rankingscreen.cpp
	g++ -fPIC -c rankingscreen.cpp
options.o: options.cpp
	g++ -fPIC -c options.cpp
lang.o: lang.cpp
	g++ -fPIC -c lang.cpp
item.o: item.cpp
	g++ -fPIC -c item.cpp
ai.o: ai.cpp
	g++ -fPIC -c ai.cpp
animation.o: animation.cpp
	g++ -fPIC -c animation.cpp
	
# Build the debug objects
game.d.o: game.cpp
	g++ -g -fPIC -c game.cpp
support.d.o: support.cpp
	g++ -g -fPIC -c support.cpp
stage.d.o: stage.cpp
	g++ -g -fPIC -c stage.cpp
piece.d.o: piece.cpp
	g++ -g -fPIC -c piece.cpp
skin.d.o: skin.cpp
	g++ -g -fPIC -c skin.cpp
presentation.d.o: presentation.cpp
	g++ -g -fPIC -c presentation.cpp
menu.d.o: menu.cpp
	g++ -g -fPIC -c menu.cpp
ranking.d.o: ranking.cpp
	g++ -g -fPIC -c ranking.cpp
namedialog.d.o: namedialog.cpp
	g++ -g -fPIC -c namedialog.cpp
network.d.o: network.cpp
	g++ -g -fPIC -c network.cpp
netroom.d.o: netroom.cpp
	g++ -g -fPIC -c netroom.cpp
difficulty.d.o: difficulty.cpp
	g++ -g -fPIC -c difficulty.cpp
rankingscreen.d.o: rankingscreen.cpp
	g++ -g -fPIC -c rankingscreen.cpp
options.d.o: options.cpp
	g++ -g -fPIC -c options.cpp
lang.d.o: lang.cpp
	g++ -g -fPIC -c lang.cpp
item.d.o: item.cpp
	g++ -g -fPIC -c item.cpp
ai.d.o: ai.cpp
	g++ -g -fPIC -c ai.cpp
animation.d.o: animation.cpp
	g++ -g -fPIC -c animation.cpp

# Cleans the directories, removing executables and objects
clean:
	rm *.o tess
	
# Runs the executable file
exec:
	./tess
