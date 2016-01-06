P=dollars
OBJECTS=main.o pgsql_util.o
CFLAGS=-g -Wall -O3 `pkg-config --cflags libpq libexplain`
LDLIBS=`pkg-config --libs libpq libexplain`
CC=gcc

$(P): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDLIBS) -o dollars
