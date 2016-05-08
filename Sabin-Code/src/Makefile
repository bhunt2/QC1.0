# Compile
all: drone

# Create drone object executable
drone: main.o protocol.o parsers.o
	g++ main.o protocol.o parsers.o -o drone

# Create main object
main.o: main.cpp
	g++ -c main.cpp

# Create Protocol object
protocol.o: protocol.cpp
	g++ -c protocol.cpp

# Create parsers object
parsers.o: parsers.cpp
	g++ -c parsers.cpp

# Clean all the objects
clean:
	rm -rf *o drone