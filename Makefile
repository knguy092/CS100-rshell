MOVE = cd src;
COMPILE = g++
FLAGS = -Wall -Werror -ansi -pedantic

all:
	rm -rf bin
	mkdir -p bin
	$(COMPILE) $(FLAGS) ./src/terminal.cpp -o ./bin/rshell
	
rshell:
	rm -rf bin
	mkdir -p bin
	$(COMPILE) $(FLAGS) ./src/terminal.cpp -o ./bin/rshell
