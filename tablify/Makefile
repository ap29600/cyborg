CC = gcc
CFLAGS = -Wall -Wmissing-prototypes -Wextra -std=c11 -pedantic -ggdb
LFLAGS = -x c -fPIC -c
BINARY_PATH = ~/.local/bin

default: tablify

tablify: src/*.c
	${CC} ${CFLAGS} src/tablify.c -o tablify

clean:
	${RM} tablify

install: tablify
	cp tablify ${BINARY_PATH}/tablify
