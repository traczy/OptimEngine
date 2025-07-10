SRC = src/
BUILD = build/

# Find all .cpp files recursively in SRC_DIR
SOURCES = $(shell find $(SRC) -type f -name "*.cpp")
# Convert source files to object files in OBJ_DIR
OBJECTS = $(patsubst $(SRC)%.cpp,$(BUILD)%.o,$(SOURCES)) $(BUILD)glad.o

INCLUDE_DIRS = -I../../glfw-3.4/include/ -I../../glad/include/ -I./include/
LINK = -lkernel32 -lUser32 -lGdi32 -L"../../glfw-3.4/lib-mingw-w64/" -lglfw3dll -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/" -lOpenGL32 -mconsole

CXXFLAGS_DEBUG = g++ -Wall -O0 -g --std=c++17
CXXFLAGS_RELEASE = g++ -Wall --std=c++17
CXX_FULLBUILD_DEBUG = $(CXXFLAGS_DEBUG) $(INCLUDE_DIRS)
CXX_OBJECT_BUILD_DEBUG = $(CXXFLAGS_DEBUG) $(INCLUDE_DIRS)
CXX_FULLBUILD_RELEASE = $(CXXFLAGS_RELEASE) $(INCLUDE_DIRS)
CXX_OBJECT_BUILD_RELEASE = $(CXXFLAGS_RELEASE) $(INCLUDE_DIRS)

prepare:
	mkdir -p $(BUILD)

all: prepare $(OBJECTS)
	$(CXX_FULLBUILD_DEBUG) -o build/OptimEngine $(OBJECTS) $(LINK)

$(BUILD)glad.o:
	$(CXX_OBJECT_BUILD_DEBUG) -LC:/Program\ Files\ \(x86\)/Windows\ Kits/10/Lib/10.0.22621.0/um/x64/ -lkernel32 -c C:/Users/jrbri/glad/src/glad.c -o $(BUILD)glad.o

$(BUILD)%.o: $(SRC)%.cpp
	mkdir -p $(@D)
	$(CXX_OBJECT_BUILD_DEBUG) -c $< -o $@

clean:
	rm -rf $(BUILD)*

debug:
	echo $(SOURCES)
	echo $(OBJECTS)