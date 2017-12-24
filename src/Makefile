CC=gcc
CFLAGS=-Wall -g -O2 -std=c99 -D_FILE_OFFSET_BITS=64 -fstack-protector-strong -fPIC
LDFLAGS=-Wl,-z,now -pie
LIBS=`pkg-config --libs gtk+-3.0 glib-2.0 gmodule-2.0`
INCS=`pkg-config --cflags gtk+-3.0 glib-2.0 gmodule-2.0`

main: main.o
	$(CC) ${LDFLAGS} -o main main.o ${LIBS}

main.o: main.c
	$(CC) $(CFLAGS) -c main.c ${INCS}

clean:
	rm -f main *.o
