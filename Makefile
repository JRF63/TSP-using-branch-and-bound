all:
	gcc-4.9 -Winline -std=c11 -O3 -march=native -fdiagnostics-color=auto -flto test.c -o test -lm
debug:
	gcc-4.9 -DDEBUG -Wall -Wextra -std=c11 -fdiagnostics-color=auto -flto test.c -o test -lm