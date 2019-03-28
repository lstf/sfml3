#debug logger off
DEBUG=
#debug logger on
DEBUG=-DDEBUG_GAME

EXEC		=game
CC			=g++
FLAGS		=-Wall -Wextra -Wpedantic $(DEBUG)
LIB			=-lsfml-graphics -lsfml-window -lsfml-system -pthread
SRC			=$(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJ			=$(patsubst %.cpp, %.o, $(SRC))
DEP			=$(patsubst %.cpp, %.d, $(SRC))

$(EXEC):	$(OBJ) $(DEP)
	rm -f *.d.* */*.d.* */*/*.d.*
	$(CC) $(OBJ) -o $(EXEC) $(LIB)

include		$(DEP)

%.d:		%.cpp
	@set -e; rm -f $@; \
	$(CC) -M $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o:		%.cpp
	$(CC) -c $(FLAGS) $< -o $@

clean:
	rm -f $(EXEC) $(OBJ) $(DEP)
