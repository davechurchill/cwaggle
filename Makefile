CC=g++
CFLAGS=-O3 -std=c++14
LDFLAGS=-O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
INCLUDES=-I./include/
SRC_EXAMPLE=$(wildcard src/example/*.cpp) 
OBJ_EXAMPLE=$(SRC_EXAMPLE:.cpp=.o)
SRC_ORBITAL=$(wildcard src/orbital/*.cpp) 
OBJ_ORBITAL=$(SRC_ORBITAL:.cpp=.o)
SRC_RL=$(wildcard src/rl/*.cpp) 
OBJ_RL=$(SRC_RL:.cpp=.o)

all:cwaggle_example cwaggle_orbital cwaggle_rl

cwaggle_example:$(OBJ_EXAMPLE) Makefile
	$(CC) $(OBJ_EXAMPLE) -o ./bin/$@ $(LDFLAGS)

cwaggle_orbital:$(OBJ_ORBITAL) Makefile
	$(CC) $(OBJ_ORBITAL) -o ./bin/$@ $(LDFLAGS)
	
cwaggle_rl:$(OBJ_RL) Makefile
	$(CC) $(OBJ_RL) -o ./bin/$@ $(LDFLAGS)

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm $(OBJ_EXAMPLE) $(OBJ_ORBITAL) $(OBJ_RL) bin/cwaggle_example bin/cwaggle_orbital bin/cwaggle_rl
