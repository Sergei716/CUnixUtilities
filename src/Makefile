CC = gcc
CFLAG = -std=c11 -Wall -Werror -Wextra

all : my_grep my_cat

cat_test :
	@cd ./cat && make test

grep_test :
	@cd ./grep && make test

my_cat :
	@cd ./cat && make my_cat

my_grep :
	@cd ./grep && make my_grep

.PHONY: cat_test grep_test my_cat my_grep

clean :
	@rm -f cat/*.o cat/my_cat cat/*.log cat/*.log  grep/*.o grep/my_grep grep/*.log grep/*.log