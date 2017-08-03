TARGET_EXEC = texmer

BUILD_DIR = ./build
SRC_DIRS = ./src
INCLUDE_DIRS = ./inst/include
INC_DIRS := $(shell find $(INCLUDE_DIRS) $(SRC_DIRS) -type d)

SRCS := $(shell find $(SRC_DIRS) ! -name "R*" -and \( -name "*.cpp" -or -name "*.c" \))
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INCLUDES := $(shell find $(INCLUDE_DIRS) -name "*.h" -or -name "*.hpp")

CPP = g++
CPPFLAGS = -std=c++14 -g -Wall -Wpedantic -Werror -Wformat

CC = g++
CFLAGS = $(CPPFLAGS)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LIB_FLAGS :=

all: $(BUILD_DIR)/$(TARGET_EXEC)

run: all
	$(BUILD_DIR)/$(TARGET_EXEC)

test: run

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@ $(LIB_FLAGS)

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CPP) $(CPPFLAGS) $(INC_FLAGS) -c $< -o $@ $(LIB_FLAGS)

clean:
	$(RM) -r $(BUILD_DIR)

# -include $(DEPS)

MKDIR_P = mkdir -p
