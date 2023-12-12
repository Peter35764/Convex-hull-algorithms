CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11
FSANITIZER = #-fsanitize=address
DEBUGGER_FLAGS = -ggdb -O0
CHECK = 2>&1 valgrind --tool=memcheck --leak-check=full --track-origins=yes #--leak-check=yes --show-leak-kinds=all
BIN_NAME = aboba
.DEFAULT_GOAL := test

.PHONY: all clean test rebuild build clang clang_n

all : clang build

clean :
	rm -f $(BIN_NAME) vgcore.*

test : clean
	$(CC) $(DEBUGGER_FLAGS) $(FSANITIZER) main.c linked_list.c -o $(BIN_NAME) # -s
	#$(CHECK) ./$(BIN_NAME) # read_from_file
	./$(BIN_NAME)

rebuild : clean all

build :
	$(CC) $(FLAGS) main.c linked_list.c -o $(BIN_NAME)

clang:
	clang-format -style=Google -i *.[ch]

clang_n:
	clang-format -style=Google -n *.[ch]
