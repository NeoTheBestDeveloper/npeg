CC = clang
CFLAGS = -g -Wall
LDFLAGS = -lm

MKDIR_P ?= mkdir -p

SRC_DIRS ?= ./src
BUILD_DIR ?= ./build

SRCS := $(shell find $(SRC_DIR) -name "*.c")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

TARGET=$(BUILD_DIR)/npeg

all: $(TARGET)

release: CFLAGS = -Wall -O2
release: clean all

test: $(TARGET)
	./tests/test.py

$(TARGET): $(OBJS) 
	$(MKDIR_P) $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c 
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)
