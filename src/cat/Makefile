CC = gcc
CFLAG = -std=c11 -Wall -Werror -Wextra

my_cat : clean cat.o
	@$(CC) $(CFLAG) -o my_cat cat.o
	
cat.o : cat.c cat.h
	@$(CC) $(CFLAG) -c cat.c

test: clean my_cat
	@./test_func_cat.sh

.PHONY: all clean

clean :
	@rm -f cat.o my_cat *.log
