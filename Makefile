CC=gcc
FLAGS=-Wall
NAME=radiography

all: radiography.o
	$(CC) `pkg-config --cflags gtk+-3.0` $< -o $(NAME) `pkg-config --libs gtk+-3.0` -export-dynamic $(FLAGS) $(CFLAGS)
	sudo setcap cap_sys_ptrace+ep $(NAME)

%.o: %.c
	$(CC) `pkg-config --cflags gtk+-3.0` -c $< `pkg-config --libs gtk+-3.0` -export-dynamic $(FLAGS) $(CFLAGS)

clean:
	rm *.o $(NAME)
