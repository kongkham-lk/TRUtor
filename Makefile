# This file is needed when attempting to compile C++ code on VS_Code
# To compile all code run:
#	1. "g++ -std=c++11 $(find . -name "*.cpp") -o main"
#	2. "./main"
#	or
#	  "make clean && make && ./main"

CXX = g++
CXXFLAGS = -std=c++11
TARGET = main

# Find all .cpp files
SOURCES = $(shell find . -name "*.cpp")
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	find . -name "*.o" -delete
	rm -f $(TARGET)

.PHONY: clean
