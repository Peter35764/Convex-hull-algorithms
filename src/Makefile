CC = gcc # clang
FLAGS = -Wall -Werror -Wextra -std=c11
FSANITIZER = -fsanitize=address -fsanitize=undefined # для -fsanitize=memory нужно сменить компилятор на clang
DEBUGGER_FLAGS = -ggdb -O0
CHECK = 2>&1 valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all #--leak-check=yes
SOURCE_FILES = main.c linked_list/linked_list.c

BIN_NAME := conv-hull
.DEFAULT_GOAL := test

.PHONY: all clean rebuild test clang clang_n

all : 
	$(CC) $(FLAGS) $(SOURCE_FILES) -o $(BIN_NAME)

clean :
	rm -f $(BIN_NAME)* vgcore.* output.txt ../output/*

rebuild : clean all

test : clean
	$(CC) $(DEBUGGER_FLAGS) $(SOURCE_FILES) -o $(BIN_NAME)_debug && $(CHECK) ./$(BIN_NAME)_debug
	$(CC) $(DEBUGGER_FLAGS) $(FSANITIZER) $(SOURCE_FILES) -o $(BIN_NAME)_fsanitize &&./$(BIN_NAME)_fsanitize

clang:
	clang-format -style=Google -i *.[ch]

clang_n:
	clang-format -style=Google -n *.[ch]
