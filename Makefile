CXX := clang++
CXXFLAGS := -std=c++17 -I../template -Wall
TARGET := test

all:
	$(CXX) $(CXXFLAGS) *.cpp -g -o $(TARGET)

test: all
	./$(TARGET)

clean:
	rm -rf $(TARGET)

