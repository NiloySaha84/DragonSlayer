
### `Makefile`

```makefile
# Makefile for Dragon Slayer Game

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2

# Paths for SFML
SFML_LIB = -lsfml-graphics -lsfml-window -lsfml-system
SFML_INC = -I/usr/include/SFML

# Executable name
TARGET = dragon_slayer

# Source files
SRC = main.cpp animatedgif.cpp

# Build the executable
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SFML_INC) -o $(TARGET) $(SRC) $(SFML_LIB)

# Clean the build
clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
