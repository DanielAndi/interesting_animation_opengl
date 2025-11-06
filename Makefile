# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Try to use pkg-config for includes and libraries, fallback to manual paths
PKG_CONFIG = pkg-config
HAS_PKG_CONFIG = $(shell $(PKG_CONFIG) --exists glfw3 glew 2>/dev/null && echo "yes" || echo "no")

ifeq ($(HAS_PKG_CONFIG),yes)
    INCLUDES = -Iinclude $(shell $(PKG_CONFIG) --cflags glfw3 glew)
    LIBS = $(shell $(PKG_CONFIG) --libs glfw3 glew) -lGL -lX11 -lpthread
else
    # Fallback: manual paths
    INCLUDES = -Iinclude -I/usr/include -I/usr/include/GL
    LIBS = -lGL -lglfw -lGLEW -lX11 -lpthread
endif

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Executable name
TARGET = $(BUILD_DIR)/InterestingAnimationOpenGL

# Default target
all: $(TARGET)

# Create executable
$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJECTS) -o $@ $(LIBS)
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Run the program
run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	@echo "Clean complete"

# Phony targets
.PHONY: all run clean

