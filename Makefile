# Compiler setup
CC      = g++
CFLAGS  = -Wall -Wextra -O3 -g -pthread --std=c++2a

# Source files
SRC_DIR     = src
SRC_FILES   = $(wildcard $(SRC_DIR)/*.cpp)

# Header files
HEADER_DIR      = headers
HEADER_FILES    = $(wildcard $(HEADER_DIR)/*.h)

# Object files
OBJ_DIR     = obj
OBJ_FILES   = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Output executable name
EXECUTABLE = mysort

# Make makefile cross platform
ifeq ($(OS), Windows_NT)
    # Windows configuration
    RM                          = del /Q /F
    RM_EXECUTABLE_EXTENSION     = .exe
    RM_OBJ_FILES                = $(subst /, \, $(OBJ_FILES))
    RM_ANS_FILES                = $(subst /, \, $(ANS_FILES))
    RMDIR                       = rmdir /S /Q
    MKDIR                       = mkdir
else
    # Unix configuration
    RM                          = rm -f
    RM_EXECUTABLE_EXTENSION     = ""
    RM_OBJ_FILES                = $(OBJ_FILES)
    RM_ANS_FILES                = $(ANS_FILES)
    RMDIR                       = rm -rf
    MKDIR                       = mkdir
endif

#################
#    Targets    #
#################

# Executable target
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Object directory target
$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)

# Pattern rule to make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADER_FILES) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Cleanup object files/directory, and executable
clean:
	$(RM) $(RM_OBJ_FILES) $(EXECUTABLE)$(RM_EXECUTABLE_EXTENSION)
	$(RMDIR) $(OBJ_DIR)

.PHONY: clean
