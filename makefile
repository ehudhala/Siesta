CXX = g++-7 -Wall -pedantic -std=c++14

SRC    = $(wildcard *.cc)
HEAD    = $(SRC:.cc=.h)
OBJ    = $(SRC:.cc=.o)

.PHONY: all
.PHONY: test
.PHONY: clean

all: test

%.o: %.cc
	$(CXX) -c -MMD $<

main: $(OBJ)
	$(CXX) $^ -lgtest -lgtest_main -lpthread -o main

test: main
	./main

clean:
	rm -f *.o *.d main

-include *.d