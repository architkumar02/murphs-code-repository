# Example makefile for Player client programs

PLAYERDIR = /usr

CXX = g++
INCLUDES = -I$(PLAYERDIR)/include/player-3.0/ -I$(PLAYERDIR)/include/boost/ -I libplayerc++
CFLAGS = -Wall -g $(INCLUDES)

robotAgent:   
	$(CXX) $(CFLAGS) -o robotAgent `pkg-config --cflags playerc++` robotAgent.cc `pkg-config --libs playerc++`

goto: 
	$(CXX) $(CFLAGS) -o goto `pkg-config --cflags playerc++` goto.cc `pkg-config --libs playerc++`

clean:
	rm -f a.out core robotAgent *.o
