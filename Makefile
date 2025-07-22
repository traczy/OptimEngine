SRC = src/
BUILD = build/

# Find all .cpp files recursively in SRC_DIR
SOURCES = $(shell find $(SRC) -type f -name "*.cpp")
# Convert source files to object files in OBJ_DIR
OBJECTS = $(patsubst $(SRC)%.cpp,$(BUILD)%.o,$(SOURCES)) $(BUILD)glad.o

INCLUDE_DIRS = -I../../glfw-3.4/include/ -I../../glad/include/ -I./include/ -I../../glm-master/ -I../../stb-master/
LINK = -lkernel32 -lUser32 -lGdi32 -L"../../glfw-3.4/lib-mingw-w64/" -lglfw3dll -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/" -lOpenGL32 -mconsole

CXX = g++
CXXFLAGS = $(CXX) -Wall --std=c++17

CXXFLAGS_DEBUG = $(CXXFLAGS) -O0 -g
CXXFLAGS_RELEASE = $(CXXFLAGS) -O2

CXX_FULLBUILD_DEBUG = $(CXXFLAGS_DEBUG) $(INCLUDE_DIRS)
CXX_FULLBUILD_RELEASE = $(CXXFLAGS_RELEASE) $(INCLUDE_DIRS)

CXX_OBJECT_BUILD_DEBUG = $(CXXFLAGS_DEBUG) $(INCLUDE_DIRS)
CXX_OBJECT_BUILD_RELEASE = $(CXXFLAGS_RELEASE) $(INCLUDE_DIRS)

prepare:
	mkdir -p $(BUILD)

all: CXX_FULLBUILD=$(CXX_FULLBUILD_DEBUG)
all: CXX_OBJECT_BUILD=$(CXX_OBJECT_BUILD_DEBUG)
all: prepare $(OBJECTS)
	$(CXX_FULLBUILD) -o build/OptimEngine $(OBJECTS) $(LINK)

release: CXX_FULLBUILD=$(CXX_FULLBUILD_RELEASE)
release: CXX_OBJECT_BUILD=$(CXX_OBJECT_BUILD_RELEASE)
release: prepare $(OBJECTS)
	$(CXX_FULLBUILD) -o build/OptimEngine $(OBJECTS) $(LINK)

$(BUILD)glad.o:
	$(CXX_OBJECT_BUILD_DEBUG) -LC:/Program\ Files\ \(x86\)/Windows\ Kits/10/Lib/10.0.22621.0/um/x64/ -lkernel32 -c C:/Users/jrbri/glad/src/glad.c -o $(BUILD)glad.o

$(BUILD)%.o: $(SRC)%.cpp
	mkdir -p $(@D)
	$(CXX_OBJECT_BUILD) -c $< -o $@

clean:
	rm -rf $(BUILD)*