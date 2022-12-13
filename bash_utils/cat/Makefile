CC=gcc
CFLAGS=-Wall -Wextra -Werror
SRC_CAT=s21_cat.c s21_cat_funcs.c
HDR=s21_cat_funcs.h
EXE_CAT=s21_cat

.PHONY: all clean rebuild clang cpp

all: s21_cat

s21_cat: clean
	$(CC) $(CFLAGS) $(SRC_CAT) -o $(EXE_CAT)

clean:
	rm -rf $(EXE_CAT)

rebuild: clean all

clang:
	clang-format -style=google -i $(SRC_CAT)
	clang-format -style=google -i $(HDR)

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SRC_CAT)

leaks_cat: $(EXE_CAT)
	leaks -atExit -- $(word 1,$^)

valgrind: 
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./$(EXE_CAT) 

