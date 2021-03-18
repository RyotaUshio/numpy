CXX = g++
override CXXFLAGS += -std=c++17 -O3 -Wall -I . # -g
HEADER = $(wildcard ./numpy/*.hpp)
SRC = $(wildcard ./test/*.cpp)
EXC = $(basename $(SRC))

.PHONY: clean all run $(notdir $(EXC))

all: $(EXC) run

%: %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -o $@ $<

run:
	for name in $(EXC); do $$name; done

clean:
	rm $(EXC)
