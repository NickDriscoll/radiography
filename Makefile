CC=gcc

all: radiography.o
	$(CC) `pkg-config --cflags gtk+-3.0` $< -o radiography `pkg-config --libs gtk+-3.0` -export-dynamic $(CFLAGS)

%.o: %.c
	$(CC) `pkg-config --cflags gtk+-3.0` -c $< `pkg-config --libs gtk+-3.0` -export-dynamic $(CFLAGS)

clean:
	rm *.o radiography
