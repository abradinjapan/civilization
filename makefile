all:
	gcc source/main.c -Wextra -Wall -fsanitize=address -o ../civilization-debug.elf -g -lSDL2 -lGL -lGLEW -lm
	gcc source/main.c -Wextra -Wall -o ../civilization.elf -g -lSDL2 -lGL -lGLEW -lm
