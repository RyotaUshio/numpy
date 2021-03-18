CXX = g++
override CXXFLAGS += -std=c++17 -O3 -Wall -I . # -g
HEADER = $(wildcard ./numpy/*.hpp)
SRC = $(wildcard ./test/*.cpp)
EXC = $(basename $(SRC))
NOTPYTEST = test_iter
PYTEST = $(filter-out $(addprefix %/, $(NOTPYTEST)), $(EXC))
CPPTEST = $(filter-out $(PYTEST), $(EXC))

.PHONY: clean all run py++

all: $(CPPTEST) test

%: %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -o $@ $<

test: run py++

run:
	for name in $(filter $(addprefix %/, $(NOTPYTEST)), $(EXC)); do $$name; done

py++:
	for name in $(notdir $(PYTEST)); do ./py++ $$name; done

clean:
	rm $(EXC) ./test/._py++_*
