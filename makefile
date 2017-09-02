CXX = g++-7 -Wall -pedantic -std=c++14
OBJECTS = lexer.o
TEST_OBJECTS = test_lexer.o

.PHONY: all
.PHONY: test
.PHONY: clean

all: test

%.o: %.cc %.h %.inl
	$(CXX) -c $< 

test_main: $(TEST_OBJECTS) $(OBJECTS)
	$(CXX) $^ -lgtest -lgtest_main -lpthread -o test_main

test: test_main
	./test_main

clean:
	rm -f *.o test_main
