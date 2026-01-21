# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
DEBUG_FLAGS = -g -O0 -DDEBUG
INCLUDES = -Isrc

# Build mode (release or debug)
BUILD_MODE ?= release

# Directories
SRC_DIR = src
TOOLS_DIR = $(SRC_DIR)/tools
BUILD_DIR = build
DEBUG_BUILD_DIR = build-debug
OBJ_DIR = $(BUILD_DIR)/obj
DEBUG_OBJ_DIR = $(DEBUG_BUILD_DIR)/obj

# Source files
MAIN_SRC = $(SRC_DIR)/main.cpp
QUEST_SRCS = $(wildcard $(SRC_DIR)/quest*.cpp)
TOOLS_SRCS = $(TOOLS_DIR)/map.cpp $(TOOLS_DIR)/readData.cpp $(TOOLS_DIR)/timings.cpp
ALL_SRCS = $(MAIN_SRC) $(QUEST_SRCS) $(TOOLS_SRCS)

# Object files (release)
MAIN_OBJ = $(OBJ_DIR)/main.o
QUEST_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(QUEST_SRCS))
TOOLS_OBJS = $(patsubst $(TOOLS_DIR)/%.cpp,$(OBJ_DIR)/tools_%.o,$(TOOLS_SRCS))
ALL_OBJS = $(MAIN_OBJ) $(QUEST_OBJS) $(TOOLS_OBJS)

# Object files (debug)
DEBUG_MAIN_OBJ = $(DEBUG_OBJ_DIR)/main.o
DEBUG_QUEST_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(DEBUG_OBJ_DIR)/%.o,$(QUEST_SRCS))
DEBUG_TOOLS_OBJS = $(patsubst $(TOOLS_DIR)/%.cpp,$(DEBUG_OBJ_DIR)/tools_%.o,$(TOOLS_SRCS))
DEBUG_ALL_OBJS = $(DEBUG_MAIN_OBJ) $(DEBUG_QUEST_OBJS) $(DEBUG_TOOLS_OBJS)

# Target executables
TARGET = $(BUILD_DIR)/puzzle
DEBUG_TARGET = $(DEBUG_BUILD_DIR)/puzzle

# Default target
all: $(TARGET)

# Debug target
debug: $(DEBUG_TARGET)

# Create directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(DEBUG_OBJ_DIR):
	@mkdir -p $(DEBUG_OBJ_DIR)

$(DEBUG_BUILD_DIR):
	@mkdir -p $(DEBUG_BUILD_DIR)

# Link object files into executable (release)
$(TARGET): $(ALL_OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(ALL_OBJS) -o $(TARGET)
	@echo "Build complete: $(TARGET)"

# Link object files into executable (debug)
$(DEBUG_TARGET): $(DEBUG_ALL_OBJS) | $(DEBUG_BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(DEBUG_ALL_OBJS) -o $(DEBUG_TARGET)
	@echo "Debug build complete: $(DEBUG_TARGET)"

# Compile main.cpp (release)
$(OBJ_DIR)/main.o: $(MAIN_SRC) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile quest files (release)
$(OBJ_DIR)/quest%.o: $(SRC_DIR)/quest%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile tool files (release)
$(OBJ_DIR)/tools_%.o: $(TOOLS_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Compile main.cpp (debug)
$(DEBUG_OBJ_DIR)/main.o: $(MAIN_SRC) | $(DEBUG_OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $< -o $@

# Compile quest files (debug)
$(DEBUG_OBJ_DIR)/quest%.o: $(SRC_DIR)/quest%.cpp | $(DEBUG_OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $< -o $@

# Compile tool files (debug)
$(DEBUG_OBJ_DIR)/tools_%.o: $(TOOLS_DIR)/%.cpp | $(DEBUG_OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(INCLUDES) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(DEBUG_BUILD_DIR)
	@echo "Clean complete"

# Run the executable (release)
run: $(TARGET)
	./$(TARGET)

# Run the executable (debug)
run-debug: $(DEBUG_TARGET)
	./$(DEBUG_TARGET)

# Phony targets
.PHONY: all debug clean run run-debug
