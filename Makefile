CC=gcc
CXX=g++
ECHO=echo
RM=rm -rvf
MKDIR=mkdir -p
CD=cd
INSTALL=install

# Binary name
BIN=hangman

# Directories 
DEST_DIR=/
ROOT_DIR=.
SRC_BASE_DIR=$(ROOT_DIR)

# Generated directories
BUILD_DIR=$(ROOT_DIR)/build

# Compilation and linking flags
CXX_FLAGS = -std=c++11
LD_FLAGS = -lncurses

ifeq ($(shell uname -s),Darwin)
	GCC_DIR := $(shell brew --prefix gcc)
	GXX_PATH := $(GCC_DIR)"/bin/g++-9"

	CXX := $(GXX_PATH)
endif

# Sources, objects, and dependencies
C_SRC=$(shell find $(SRC_BASE_DIR) -name '*.c')
CXX_SRC=$(shell find $(SRC_BASE_DIR) -name '*.cpp')
OBJ=$(patsubst %.c, $(BUILD_DIR)/%.o, $(filter %.c,$(subst /, , $(C_SRC)))) \
	$(patsubst %.cpp, $(BUILD_DIR)/%.o, $(filter %.cpp,$(subst /, , $(CXX_SRC))))
DEP=$(OBJ:%.o=%.d)

all: $(BIN)

$(BIN): $(OBJ)
	@$(ECHO) Linking compiled files... 
	@$(CXX) $^ -o $(@F) $(LD_FLAGS)

-include $(DEP)

$(BUILD_DIR)/%.o: $(SRC_SUB_DIRS)%.c | $(BUILD_DIR)
	@$(ECHO) Compiling $<
	@$(CC) $(C_FLAGS) -MMD -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_SUB_DIRS)%.cpp | $(BUILD_DIR)
	@$(ECHO) Compiling $<
	@$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

.PHONY: clean lint install uninstall
clean:
	@$(ECHO) Removing all generated files and executables...
	@$(RM) $(BUILD_DIR) $(BIN) core vgcore.* valgrind*

lint:
	@$(ECHO) Linting source files per Google\'s CXX Styleguide...
	@$(LINT) $(C_SRC) $(CXX_SRC)

install:
	@$(ECHO) "Installing to $(DEST_DIR)$(INSTALL_DIR)"
	@$(INSTALL) $(BIN) $(DEST_DIR)$(INSTALL_DIR)

uninstall:
	@$(RM) $(DEST_DIR)$(INSTALL_DIR)/$(BIN)

# Directory generation
$(BUILD_DIR):
	@$(MKDIR) $(BUILD_DIR)
