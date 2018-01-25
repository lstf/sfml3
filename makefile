EXEC	=game
CC		=g++
LIB		=-lsfml-graphics -lsfml-window -lsfml-system
OBJ		=$(patsubst %.cpp, %.o, $(wildcard *.cpp))
DEPS	=font.h window.h

game:	$(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LIB)

%.o:	%.cpp %.h
	$(CC) -c $<

clean:
	rm $(EXEC) *.o
