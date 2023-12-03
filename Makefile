all: main

main:
	gcc conway.c -o game-of-life -lSDL2 -lSDL2_image

clean:
	rm game-of-life
