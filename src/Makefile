CC =gcc # clang
FLAGS = #-Wall -Werror -Wextra -std=c11
LIBS = -lm
FSANITIZER = -fsanitize=address -fsanitize=undefined # для -fsanitize=memory нужно сменить компилятор на clang
DEBUGGER_FLAGS = -ggdb -O0
CHECK = 2>&1 valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all #--leak-check=yes
SOURCE_FILES = main.c linked_list/linked_list.c

BIN_NAME := a.out
#.DEFAULT_GOAL := test

.PHONY: all clean clean_src rebuild test clang clang_n

all : 
	$(CC) $(DEBUGGER_FLAGS) $(FLAGS) $(LIBS) $(SOURCE_FILES) -o $(BIN_NAME) #DEBUGGER_FLAGS

clean : clean_src
	rm -f ../output/*

clean_src :
	rm -f $(BIN_NAME)* vgcore.*

rebuild : clean all

test : clean
	$(CC) $(DEBUGGER_FLAGS) $(LIBS) $(SOURCE_FILES) -o $(BIN_NAME)_debug && $(CHECK) ./$(BIN_NAME)_debug | grep "ERROR SUMMARY"
	$(CC) $(DEBUGGER_FLAGS) $(LIBS) $(FSANITIZER) $(SOURCE_FILES) -o $(BIN_NAME)_fsanitize && ./$(BIN_NAME)_fsanitize 

clang:
	clang-format -style=Google -i *.[ch]

clang_n:
	clang-format -style=Google -n *.[ch]
