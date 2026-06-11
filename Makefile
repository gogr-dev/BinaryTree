CXX      := c++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2

.PHONY: all run test clean

all: names

names: main.cpp tree.cpp tree.h
	$(CXX) $(CXXFLAGS) -o $@ main.cpp tree.cpp

run: names
	./names

test_tree: tests/test_tree.cpp tree.cpp tree.h
	$(CXX) $(CXXFLAGS) -o $@ tests/test_tree.cpp tree.cpp

test: test_tree
	./test_tree

clean:
	rm -f names test_tree
