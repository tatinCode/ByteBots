CC = gcc
CXX = g++
TARGET = bin/pi-controller

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
LIB_DIR = lib

C_SRCS = $(wildcard $(SRC_DIR)/*.c) \
	   $(wildcard $(LIB_DIR)/Esp32_serial/*.c) \
	   $(wildcard $(LIB_DIR)/Pi_mmio/*.c) 

CPP_SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
		   $(wildcard $(LIB_DIR)/Camera/*.cpp) \
		   $(wildcard $(LIB_DIR)/Vision/*.cpp)

C_OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(C_SRCS))
CPP_OBJS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(CPP_SRCS))

OBJS = $(C_OBJS) $(CPP_OBJS)

INCLUDES = $(wildcard $(LIB_DIR)/*)

OPENCV_FLAGS = $(shell pkg-config --cflags opencv4)
OPENCV_LIBS = $(shell pkg-config --libs opencv4)

LIBCAMERA_FLAGS = $(shell pkg-config --cflags libcamera)
LIBCAMERA_LIBS = $(shell pkg-config --libs libcamera)

CFLAGS = -Wall -Wextra -O2 -I$(SRC_DIR) $(addprefix -I, $(INCLUDES))
CXXFLAGS = -Wall -Wextra -O2 -I$(SRC_DIR) $(addprefix -I, $(INCLUDES)) $(OPENCV_FLAGS) $(LIBCAMERA_FLAGS)
LDLIBS = $(OPENCV_LIBS) $(LIBCAMERA_LIBS)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

