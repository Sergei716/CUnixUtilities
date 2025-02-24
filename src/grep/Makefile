CC = gcc
CFLAG = -g -std=c11 -Wall -Werror -Wextra

my_grep : clean grep.o
	@$(CC) $(CFLAG) -o my_grep grep.o

grep.o : grep.c grep.h
	@$(CC) $(CFLAG) -c grep.c

test : clean my_grep
	@./test_func_grep.sh

.PHONY: clean
clean :
	@rm -f grep.o my_grep
