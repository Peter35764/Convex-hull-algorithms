CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11
CHECK = 2>&1 valgrind --tool=memcheck --leak-check=yes

BIN_NAME = aboba
.DEFAULT_GOAL := exec

.PHONY: all clean test rebuild build exec clang clang_n

all : clang build

clean :
	-@rm $(BIN_NAME)

test : clean
	$(CC) $(FLAGS) -g main.c -o $(BIN_NAME) # -s
	$(CHECK) ./$(BIN_NAME) # read_from_file

rebuild : clean all

build :
	$(CC) $(FLAGS) main.c -o $(BIN_NAME)

exec : rebuild
	./$(BIN_NAME)

clang:
	clang-format -style=Google -i *.[ch]

clang_n:
	clang-format -style=Google -n *.[ch]
