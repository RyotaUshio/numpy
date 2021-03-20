CXX = g++
override CXXFLAGS += -std=c++17 -O3 -Wall -I . # -g
HEADER = $(wildcard ./numpy/*.hpp)
SRC = $(wildcard ./test/*.cpp)
EXC = $(basename $(SRC))
NOTPYTEST = test_iter
PYTEST = $(filter-out $(addprefix %/, $(NOTPYTEST)), $(EXC))
CPPTEST = $(filter-out $(PYTEST), $(EXC))
override pypp += 

.PHONY: clean all run py++

all: test

%: %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -o $@ $<

test: run py++

run: $(CPPTEST)
	for name in $(CPPTEST); do $$name; done

py++:
	for name in $(notdir $(PYTEST)); do py++ $$name $(pypp) ; done

ufunc := numpy/ufunc_instances.hpp

ufunc: ufunc_generate.py ufunc.txt
	python $^ $@

clean:
	rm $(EXC) ./test/._py++_*

