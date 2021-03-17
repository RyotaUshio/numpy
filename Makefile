CXX = g++
override CXXFLAGS += -std=c++17 -O3 -Wall -I . # -g
HEADER = $(wildcard ./Numpy/*.hpp)
SRC = $(wildcard ./test/*.cpp)
EXC = $(basename $(SRC))

.PHONY: clean all

all: $(EXC)

%: %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm $(EXC)
