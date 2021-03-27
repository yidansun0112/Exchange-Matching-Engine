CC		= g++ -g
CFLAGS		= -c -Wall
LDFLAGS		= -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lxerces-c
SOURCES		= test.cpp xmlParser.cpp xmlPrinter.cpp
INCLUDES	= 
OBJECTS		= $(SOURCES:.cpp=.o)
TARGET		= test

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
