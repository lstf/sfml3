EXEC	=game
CC		=g++
FLAGS	=-Wall -Wextra -Wpedantic
LIB		=-lsfml-graphics -lsfml-window -lsfml-system -pthread
OBJ		=$(patsubst %.cpp, %.o, $(wildcard *.cpp))
HDR		=$(wildcard *.h)
DEPS	=font.h window.h

game:	$(OBJ) $(HDR)
	$(CC) $(OBJ) -o $(EXEC) $(LIB)

%.o:	%.cpp $(HDR)
	$(CC) -c $(FLAGS) $<

clean:
	rm $(EXEC) *.o
