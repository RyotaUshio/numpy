CXX = g++
override CXXFLAGS += -std=c++17 -O3 -Wall # -g
HEADER = $(wildcard *.hpp)
SRC = $(wildcard *.cpp)
EXC = $(basename $(SRC))

.PHONY: clean all

all: $(EXC)

%: %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm *.o $(EXC)
