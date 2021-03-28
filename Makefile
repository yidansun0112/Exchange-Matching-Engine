CC=g++
CFLAGS=-ggdb
EXTRAFLAGS=-lpqxx -lpq

all: dbtest

dbtest: dbtest.cpp database.cpp
				$(CC) $(CFLAGS) -o dbtest dbtest.cpp database.cpp $(EXTRAFLAGS)

clean:
				rm -f *~ *.o dbtest

clobber:
				rm -f *~ *.o
