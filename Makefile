CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
SAMPLES_DIR = samples
BUILD_DIR = build_make

# Main app
APP = RailwayApp
APP_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
APP_OBJS = $(APP_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Generator
GEN = generator
GEN_SRC = $(SAMPLES_DIR)/generate_samples.cpp
GEN_OBJ = $(BUILD_DIR)/generate_samples.o

# Default target
all: $(APP) $(GEN)

# Create build directory if missing
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build RailwayApp
$(APP): $(BUILD_DIR) $(APP_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(APP_OBJS)

# Build generator
$(GEN): $(BUILD_DIR) $(GEN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(GEN_OBJ)

# Compile app object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile generator object
$(BUILD_DIR)/generate_samples.o: $(GEN_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(APP) $(GEN)

# Run app
run: $(APP)
	./$(APP)

# Run generator (optional convenience)
generate: $(GEN)
	./$(GEN)
