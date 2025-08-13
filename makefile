all:
	gcc source/main.c -Wextra -Wall -fsanitize=address -o ../abin-civilization-debug.elf -g -lSDL2 -lGL -lGLEW -lm
	gcc source/main.c -Wextra -Wall -o ../abin-civilization.elf -g -lSDL2 -lGL -lGLEW -lm
