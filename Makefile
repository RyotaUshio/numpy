CXX = g++
CXXFLAGS = -std=c++17 -O3 -g
HPP = $(wildcard *.hpp)

.PHONY: clean all

all: test

%: %.cpp $(HPP)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm *.o test
