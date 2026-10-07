CXX := x86_64-w64-mingw32-g++

RAYLIB := /mnt/c/raylib/w64devkit

CPPFLAGS := -I./include -I$(RAYLIB)/include
CXXFLAGS := -std=c++17 -Wall -Wextra -g -MMD -MP

RAYLIB_LIB := $(RAYLIB)/lib/libraylib.a
LDLIBS := $(RAYLIB_LIB) -lopengl32 -lgdi32 -lwinmm


TARGET := ./bin/game.exe
SRC := $(wildcard ./src/*.cpp)
OBJ := $(patsubst ./src/%.cpp,./build/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

$(TARGET): $(OBJ)
	mkdir -p bin
	$(CXX) $^ -o $@ $(LDLIBS) 

./build/%.o: ./src/%.cpp
	mkdir -p build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

-include $(DEP)

.PHONY: all run clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf bin build 
