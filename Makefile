SRC=source/*

LIB=-llua5.1 -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lm -lxml2
INC=-Iinclude
OPT=-O3 -std=c89
DBG=-ggdb -Werror -Wall -Wpedantic

wanderlust-testing: ${SRC}
	gcc -o $@ ${OPT} ${DBG} ${INC} ${LIB} ${SRC}

wanderlust-release: ${SRC}
	gcc -o $@ ${OPT} ${INC} ${LIB} ${SRC}

wanderlust-minimal: ${SRC}
	gcc -o $@ ${OPT} ${INC} ${LIB} ${SRC}
