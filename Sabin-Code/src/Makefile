# declare variables
CC=g++

CFLAGS=-c -Wall

MRAALIB= -lmraa

GGDB=-ggdb

# Compile
all: drone

# Create drone object executable
drone: main.o protocol.o control.o request.o parsers.o types.o
	$(CC) $(MRAALIB) main.o protocol.o control.o request.o parsers.o types.o -o drone

# Create main object
main.o: main.cpp
	$(CC) $(CFLAGS) $(GGDB) -c main.cpp

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