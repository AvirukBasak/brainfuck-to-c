NULL =
TAB  = $(NULL)    $(NULL)

# aliased options
all: b c mkdebug
o: options
d: debug
b: build
c: compiler
r: run
cl: clean
cf: cleanf

# options for make
options:
	$(info USAGE:)
	$(info $(TAB)make  o | options   -- display options)
	$(info $(TAB)make  d | debug     -- debug build)
	$(info $(TAB)make  b | build     -- optimised binary build)
	$(info $(TAB)make  c | compiler  -- brainfuck compiler build)
	$(info $(TAB)make cl | clean     -- clean ./bin/ directory)
	$(info $(TAB)make cf | cleanf    -- force clean ./bin/)
	@exit

# required stuff
CC		= gcc
DBG	   = gdb
DBG_FLAGS = -Wall -D DEBUG="(1)" -g3 -ggdb
REL_FLAGS = -Wall -Ofast
BFC_FLAGS = -Wall -Ofast -D COMPILE="(1)"

SRC_DIR   = ./src
BIN_DIR   = ./bin

SRC_PATH  = $(SRC_DIR)/*.c
DBG_PATH  = $(BIN_DIR)/dbg-binary
REL_PATH  = $(BIN_DIR)/binary
BFC_PATH  = $(BIN_DIR)/bfc

GOTO_HELL = 2> /dev/null

# clear screen
clscr:
	clear

# builds debug binary
mkdebug:
	mkdir -p $(BIN_DIR)
	$(CC) $(DBG_FLAGS) $(SRC_PATH) -o $(DBG_PATH)

# creates debug build and launches in gdb
debug: clscr mkdebug
	$(DBG) $(DBG_PATH)

# compile source to bin path
build:
	mkdir -p $(BIN_DIR)
	$(CC) $(REL_FLAGS) $(SRC_PATH) -o $(REL_PATH)

expression_provided:
ifndef exp
	$(info run expression exp= not provided)
	$(error failed)
endif

compiler:
	mkdir -p $(BIN_DIR)
	$(CC) $(BFC_FLAGS) $(SRC_PATH) -o $(BFC_PATH)

# run source
run: expression_provided
	@$(REL_PATH) $(exp)

# clear binaries
clean:
	mkdir -p $(BIN_DIR)
	rm -r $(BIN_DIR)

# force clear binaries
cleanf:
	rm -rf $(BIN_DIR)
