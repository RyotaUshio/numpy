CXX = g++
CXXFLAGS = -std=c++17 -O3 -g

.PHONY: clean all

all: test

%: %.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm *.o test
