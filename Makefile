# Compiler configuration
CC = gcc
CXX = g++

# Smash
SMASH = C:/smash
SMASH_INCLUDE_DIR := $(SMASH)/include
SMASH_DEP_DIR := $(SMASH)/depends
SMASH_DEP_STATIC_DIR := $(SMASH_DEP_DIR)/static
SMASH_DEP_DYNAMIC_DIR := $(SMASH_DEP_DIR)/dynamic
SMASH_DEP_BIN_DIR := $(SMASH_DEP_DIR)/bin
SMASH_LIB_DIR := $(SMASH)/lib
SMASH_LIB_STATIC_DIR := $(SMASH_LIB_DIR)/static
SMASH_LIB_DYNAMIC_DIR := $(SMASH_LIB_DIR)/dynamic
SMASH_BIN_DIR := $(SMASH)/bin
SMASH_STATICS := $(wildcard $(SMASH_LIB_STATIC_DIR)/*.a) $(wildcard $(SMASH_DEP_STATIC_DIR)/*.a)
SMASH_DYNAMICS := $(wildcard $(SMASH_LIB_DYNAMIC_DIR)/*.a) $(wildcard $(SMASH_DEP_DYNAMIC_DIR)/*.a)
SMASH_DEP_DLLS := $(wildcard $(SMASH_DEP_BIN_DIR)/*.dll)
SMASH_BIN_DLLS := $(wildcard $(SMASH_BIN_DIR)/*.dll)
SMASH_DLLS := $(SMASH_DEP_DLLS) $(SMASH_BIN_DLLS)

# Update smash
update_smash:
	cd $(SMASH) && git pull
	cd $(SMASH) && $(MAKE) all

# Automatically update smash
.PHONY: update_smash

# Compiler flags
CFLAGS := -Wall -Iinclude -I$(SMASH_INCLUDE_DIR)
CXXFLAGS := $(CFLAGS)
LDFLAGS = -lopengl32 -lgdi32

# Input directories
SRC_DIR = src
DEP_DIR = depends
DEP_STATIC_DIR := $(DEP_DIR)/static
DEP_DYNAMIC_DIR := $(DEP_DIR)/dynamic
DEP_BIN_DIR := $(DEP_DIR)/bin

# Output directories
OBJ_DIR = obj
BIN_DIR = bin
BIN_STANDALONE_DIR := $(BIN_DIR)/standalone
BIN_DEPENDENT_DIR := $(BIN_DIR)/dependent

# Output names
EXECUTABLE_STANDALONE = smashgame_standalone.exe
EXECUTABLE_DEPENDENT = smashgame_dependent.exe

# Determine executables
EXEC_STANDALONE := $(BIN_STANDALONE_DIR)/$(EXECUTABLE_STANDALONE)
EXEC_DEPENDENT := $(BIN_DEPENDENT_DIR)/$(EXECUTABLE_DEPENDENT)

# Collect source files
CC_SRCS := $(wildcard $(SRC_DIR)/*.c)
CXX_SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Collect object files
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(CC_SRCS)) \
        $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CXX_SRCS))

# Collect library dependencies
DEPS_STATIC := $(wildcard $(DEP_STATIC_DIR)/*.a) $(SMASH_STATICS)
DEPS_DYNAMIC := $(wildcard $(DEP_DYNAMIC_DIR)/*.a) $(SMASH_DYNAMICS)
DEPS_DLLS := $(wildcard $(DEP_BIN_DIR)/*.dll) $(SMASH_BIN_DLLS)
DEST_DLLS := $(notdir $(DEPS_DLLS))
DEST_DLLS := $(addprefix $(BIN_DEPENDENT_DIR)/,$(DEST_DLLS))

# Compile c files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | update_smash
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile cpp files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | update_smash
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Make standalone executable
$(EXEC_STANDALONE): $(OBJS) $(DEPS_STATIC) | update_smash
	@if not exist "$(BIN_STANDALONE_DIR)" mkdir "$(BIN_STANDALONE_DIR)"
	$(CXX) -o $(EXEC_STANDALONE) $(OBJS) $(DEPS_STATIC) $(LDFLAGS)
	@echo Standalone application created.

# Copy project dependency DLLs
$(BIN_DEPENDENT_DIR)/%.dll: $(DEP_BIN_DIR)/%.dll | update_smash
	@if not exist "$(BIN_DEPENDENT_DIR)" mkdir "$(BIN_DEPENDENT_DIR)"
	copy "$(subst /,\,$<)" "$(subst /,\,$@)" >nul 2>&1

# Copy smash DLLs
$(BIN_DEPENDENT_DIR)/%.dll: $(SMASH_BIN_DIR)/%.dll | update_smash
	@if not exist "$(BIN_DEPENDENT_DIR)" mkdir "$(BIN_DEPENDENT_DIR)"
	@copy "$(subst /,\,$<)" "$(subst /,\,$@)" >nul 2>&1

# Make dependent executable
$(EXEC_DEPENDENT): $(OBJS) $(DEPS_DYNAMIC) $(DEST_DLLS) | update_smash
	@if not exist "$(BIN_DEPENDENT_DIR)" mkdir "$(BIN_DEPENDENT_DIR)"
	$(CXX) -o $(EXEC_DEPENDENT) $(OBJS) $(DEPS_DYNAMIC) $(LDFLAGS)
	@echo Dependent application created.

# Make default
.DEFAULT_GOAL := all

# Make all
all: update_smash standalone dependent

# Make resolved names
standalone: $(EXEC_STANDALONE)
dependent: $(EXEC_DEPENDENT) $(DEST_DLLS)

# Make clean
clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)
	@echo Cleaned up.
