CC = gcc
TARGET = bin/pi-controller

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
LIB_DIR = lib

SRCS = $(wildcard $(SRC_DIR)/*.c) \
	   $(wildcard $(LIB_DIR)/*/*.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))

INCLUDES = $(wildcard $(LIB_DIR)/*)

CFLAGS = -Wall -Wextra -O2 $(addprefix -I, $(INCLUDES))
LDLIBS = 

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

