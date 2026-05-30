CC = gcc
TARGET = car

SRC_DIR = src
LIB_DIR = lib
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c) \
	   $(wildcard $(LIB_DIR)/*/*.c)

OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))

INCLUDES = $(wildcard $(LIB_DIR)/*)

CFLAGS = -Wall -Wextra -O2 $(addprefix -I, $(INCLUDES))
LDLIBS = -lpigpio -lrt -pthread

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

