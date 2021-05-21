BIN_DIR=bin
# Compilation settings
PROGRAM=prog
CC=gcc
FLAGS=-Wall
DFLAGS= -Wall -g

# Files names
LIB_FILES= lib/lodepng.c
SRC_FILES= LPE/ImageGris.c LPE/ListeChainee.c LPE/PairUintMap_u8.c LPE/main.c

all:
	$(CC) $(FLAGS) $(SRC_FILES) $(LIB_FILES) -o $(BIN_DIR)/$(PROGRAM)

debug:
	$(CC) $(DFLAGS) $(SRC_FILES) $(LIB_FILES) -o $(BIN_DIR)/$(PROGRAM)

run:
	./$(BIN_DIR)/$(PROGRAM)
