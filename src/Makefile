CC=gcc
CFLAGS=-Wall -Wextra -Werror
SRC_CAT=cat/s21_cat.c
HDR=
EXE_CAT=cat/s21_cat

.PHONY: all clean rebuild clang cpp

all: s21_cat

s21_cat:
	$(CC) $(CFLAGS) $(SRC_CAT) -o $(EXE_CAT)

clean:
	rm -rf $(EXE_CAT)

rebuild: clean all

clang:
	clang-format -i $(SRC_CAT)

cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem $(SRC_CAT)

leaks_cat: $(EXE_CAT)
	leaks -atExit -- $(word 1,$^)


