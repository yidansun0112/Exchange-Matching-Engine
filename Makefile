CFLAGS=-Wall -pedantic -ggdb3
LDFLAGS= -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lxerces-c -lpqxx -lpq
PROGS=server client
OBJS=$(patsubst %,%.o,$(PROGS)) database.o server.o xmlParser.o xmlPrinter.o client.o
all:$(PROGS) 
server: server.o database.o xmlParser.o xmlPrinter.o
	g++ -o $@ $^ $(LDFLAGS)
client: client.o xmlParser.o xmlPrinter.o 
	g++ -o $@ $^ $(LDFLAGS)
%.o: %.cpp
	g++ $(CFLAGS) -c $<
.PHONY: clean
clean:
	rm -f *~ $(PROGS) $(OBJS)

database.o: database.hpp
server.o: server.h
xmlParser.o: xmlParser.h
xmlPrinter.o: xmlPrinter.h
client.o: client.h
