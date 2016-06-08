

# ****************************************************************
# MakeFile for Done Control Program
# --------------------------------------------
# Capstone Project: Object Following Drone
# Sponsor: Benjamin Huntsman
# Advisor: Dr. McNames, James
# Written By: Sabin Maharjan
# Date: May 2, 2016
# Version: 1.0
# Github: https://github.com/bhunt2/QC1.0/tree/master/Src/Control
# *******************************************************************/


# declare variables
CC=g++

CFLAGS=-c -Wall

LIBS= -lmraa -lncurses -ltinfo

GGDB=-ggdb

# Compile
all: drone

# Create drone object executable
drone: main.o protocol.o control.o request.o parsers.o types.o
	$(CC) $(LIBS) main.o protocol.o control.o request.o parsers.o types.o -o drone

# Create main object
main.o: main.cpp
	$(CC) -std=c++11 $(CFLAGS) $(GGDB) -c main.cpp

# Create Protocol object
protocol.o: protocol.cpp
	$(CC) $(CFLAGS) $(GGDB) -c protocol.cpp

# Create Control object
control.o: control.cpp
	$(CC) $(CFLAGS) $(GGDB) -c control.cpp

# Create Request object
request.o: request.cpp
	$(CC) $(CFLAGS) $(GGDB) -c request.cpp

# Create parsers object
parsers.o: parsers.cpp
	$(CC) $(CFLAGS) $(GGDB) -c parsers.cpp

# Create types object
types.o: types.cpp
	$(CC) $(CFLAGS) $(GGDB) -c types.cpp

# Clean all the objects
clean:
	rm -rf *.o drone