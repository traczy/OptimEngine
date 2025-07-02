SRC = src/
BUILD = build/

OBJECTS = $(wildcard $(SRC)*.cpp)

INCLUDE_DIRS = -I../../glfw-3.4/include/ -I../../glad/include/ -I./include/ 
LINK = -L../../glfw-3.4/lib-mingw-win64/

CXXFLAGS = g++ $(INCLUDE_DIRS)

prepare:
	mkdir -p $(BUILD)

all: prepare
	$(CXXFLAGS) -o build/OptimEngine $(OBJECTS)

%.o: %.cpp
	$(CXXFLAGS) $< -o $@

clean:
	cd $(BUILD) && rm -f *