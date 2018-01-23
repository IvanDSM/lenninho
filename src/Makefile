CC=gcc
CFLAGS=-lSDL2 -lSDL2_image

alientest: global.o main.o objects.o video.o
	$(CC) -lSDL2 -lSDL2_image -o alientest global.o main.o objects.o video.o

clean:
	rm *.o alientest
