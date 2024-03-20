# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra

SRCS := $(wildcard *.cpp)

EXEC := shell

.PHONY: all clean

all: $(EXEC)
$(EXEC): $(SRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@
clean:
	rm -f $(EXEC)
