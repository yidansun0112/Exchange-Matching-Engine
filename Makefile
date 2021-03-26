TARGETS=test

all: $(TARGETS)
clean:
	rm -f $(TARGETS)
test: test.cpp
	g++ -g -o $@ $<
