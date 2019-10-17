CC=clang++

SRC := src
OBJ := obj

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))

FLAG = -I./linenoise/linenoise.h

.PHONY:hdb

hdb: ${OBJECTS} obj/linenoise.o
	${CC} ${FLAG} -o $@ $^
	
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(FLAG) -c $< -o $@


obj/linenoise.o: linenoise/linenoise.c linenoise/linenoise.h
	clang -c -g -o obj/linenoise.o linenoise/linenoise.c

clean:
	rm -rf obj
	mkdir obj
