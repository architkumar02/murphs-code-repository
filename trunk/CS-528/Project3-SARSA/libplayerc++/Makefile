# Example makefile for Player client programs

PLAYERDIR = /usr

CXX = g++
INCLUDES = -I$(PLAYERDIR)/include/player-3.0/ -I$(PLAYERDIR)/include/boost/
CFLAGS = -Wall -g $(INCLUDES)

laserobstacleavoid:   
	$(CXX) $(CFLAGS) -o laserobstacleavoid `pkg-config --cflags playerc++` laserobstacleavoid.cc `pkg-config --libs playerc++`

sonarobstacleavoid:   
	$(CXX) $(CFLAGS) -o sonarobstacleavoid `pkg-config --cflags playerc++` sonarobstacleavoid.cc `pkg-config --libs playerc++`

randomwalk:   
	$(CXX) $(CFLAGS) -o randomwalk `pkg-config --cflags playerc++` randomwalk.cc `pkg-config --libs playerc++`

example0: 
	$(CXX) $(CFLAGS) -o example0 `pkg-config --cflags playerc++` example0.cc `pkg-config --libs playerc++`

goto: 
	$(CXX) $(CFLAGS) -o goto `pkg-config --cflags playerc++` goto.cc `pkg-config --libs playerc++`

clean:
	rm -f a.out core laserobstacleavoid goto test fidRand test1 sonarobstacleavoid randomwalk example0 *.o
