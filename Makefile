P=dollars
OBJECTS=sqlite3.o database.o string_util.o main.o
CFLAGS=-g -Wall -O0 -std=c11
LDLIBS=-pthread -ldl
CC=gcc

$(P): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDLIBS) -o dollars

clean:
	rm dollars *.o
