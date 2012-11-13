# Example makefile for Player client programs

PLAYERDIR = /usr

CXX = g++
INCLUDES = -I$(PLAYERDIR)/include/player-3.0/ -I$(PLAYERDIR)/include/boost/ -I libplayerc++ -I .
CFLAGS = -Wall -g $(INCLUDES)

robotAgent:   
	$(CXX) $(CFLAGS) -o robotAgent `pkg-config --cflags playerc++` robotAgent.cc satable.cc state.cc action.cc utility.cc `pkg-config --libs playerc++`

testControl:
	$(CXX) $(CFLAGS) -o testControl tests/test_control.cc state.cc action.cc utility.cc -I .
goto: 
	$(CXX) $(CFLAGS) -o goto `pkg-config --cflags playerc++` goto.cc `pkg-config --libs playerc++`

clean:
	rm -f a.out core robotAgent *.o
