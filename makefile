EXEC	=game
CC		=g++
FLAGS	=-Wall -Wextra -Wpedantic
LIB		=-lsfml-graphics -lsfml-window -lsfml-system
OBJ		=$(patsubst %.cpp, %.o, $(wildcard *.cpp))
DEPS	=font.h window.h

game:	$(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LIB)

%.o:	%.cpp %.h
	$(CC) -c $(FLAGS) $<

clean:
	rm $(EXEC) *.o
