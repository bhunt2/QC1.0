# declare variables
CC=g++

CFLAGS=-c -Wall

# Compile
all: drone

# Create drone object executable
drone: main.o protocol.o parsers.o
	$(CC) main.o protocol.o parsers.o -o drone

# Create main object
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

# Create Protocol object
protocol.o: protocol.cpp
	$(CC) $(CFLAGS) -c protocol.cpp

# Create parsers object
parsers.o: parsers.cpp
	$(CC) $(CFLAGS) -c parsers.cpp

# Clean all the objects
clean:
	rm -rf *o drone