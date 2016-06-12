P=dollars
OBJECTS=sqlite3.o database.o string_util.o main.o test.o
CFLAGS=-g -Wall -O2 `pkg-config --cflags gtk+-3.0`
LDLIBS=-pthread -ldl `pkg-config --libs gtk+-3.0`
CC=gcc

$(P): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDLIBS) -rdynamic -o dollars

clean:
	rm dollars *.o
