CXX     = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET  = client

all: $(TARGET)

$(TARGET): client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean
