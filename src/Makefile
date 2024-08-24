CC=g++
CFLAGS=-Wall -Wextra -Werror -std=c++17 -lstdc++
DFLAGS= -fsanitize=address -g -Wall -Wextra -Werror -std=c++17 -lstdc++
UNAME:=$(shell uname -s) # определяем ОС
GCOV_FLAGS:=-ftest-coverage -fprofile-arcs

ifeq ($(UNAME),Linux ) # Linux
	OPEN_CMD=xdg-open # команда для открытия отчета
	ADD_LIB=-lm -lrt -lpthread -lsubunit # дополнительные библиотеки
	BREW=/home/darkwolf/homebrew
endif

ifeq ($(UNAME),Darwin ) # MacOS
	OPEN_CMD=open
	ADD_LIB= 
	BREW=${HOME}/homebrew
endif

PROJECTNAME=s21_brick_game

all: uninstall clean install tests

install: uninstall
	rm -rf buildSnake
	rm -rf buildTetris
	mkdir buildSnake
	cd buildSnake && cmake ../brick_game/snake/ && cmake --build .
	mkdir buildTetris
	cd buildTetris && cmake ../brick_game/tetris/ && cmake --build .
	
uninstall:
	rm -rf buildSnake
	rm -rf buildTetris

tests:
	rm -rf buildTests
	mkdir buildTests
	cd buildTests && cmake ../Tests && cmake --build . && ./Tests

gcov_report: tests
	lcov -t ${PROJECTNAME} -o ${PROJECTNAME}.info -c -d .;\
	genhtml -o report ${PROJECTNAME}.info;
	-$(OPEN_CMD) ./report/index.html

rebuild: all

foo.dvi: config_file
	doxygen config_file

dvi: foo.dvi
	-$(OPEN_CMD) html/index.html


dist: install foo.dvi
	-mkdir dist
	cp -r buildSnake/Snake dist/Snake
	cp -r buildTetris/Tetris dist/Tetris
	cp -r html/ dist/html
	cp -r latex/ dist/latex
	tar -czvf dist.tar.gz dist
	rm -rf dist
	rm -rf html
	rm -rf latex
	rm -rf buildSnake
	rm -rf buildTetris

clean:
	-rm -rf *.o && rm -rf *.gcno
	-rm -rf *.a && rm -rf *.gcda
	-rm -rf *.info && rm -rf *.gcov
	-rm -rf ./test && rm -rf ./gcov_report
	-rm -rf ./report/
	-rm -rf ./manual/
	-rm -rf ./buildTests/
	-rm -rf ./html/
	-rm -rf ./latex/

valgrind: tests
	valgrind --tool=memcheck --leak-check=yes ./buildTests/Tests
	
leaks: tests
	leaks -atExit -- buildTests/Tests

clang:
	@find . \( -name '*.h' -o -name '*.cpp' \) -print0 | xargs -0 clang-format -i -style=Google

clang_err:
	@find . \( -name '*.h' -o -name '*.cpp' \) -print0 | xargs -0 clang-format -n -style=Google

.PHONY: all clean tests


