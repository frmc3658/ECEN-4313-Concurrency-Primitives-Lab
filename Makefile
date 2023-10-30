# Compiler setup
CC = g++
CFLAGS = -Wall -Wextra -O3 -g -pthread --std=c++2a

# Code Directories
SRC_DIR 	= src
HEADER_DIR 	= headers

# Object Output Directories
OBJ_DIR 	= obj
COUNT_OBJ 	= $(OBJ_DIR)/counter
MYSORT_OBJ 	= $(OBJ_DIR)/mysort

# Files
MAIN_CPP 		= $(SRC_DIR)/main.cpp
MAIN_H 			= $(HEADER_DIR)/main.h
SRC_FILES 		= $(filter-out $(MAIN_CPP) $(SRC_DIR)/counter.cpp, $(wildcard $(SRC_DIR)/*.cpp))
HEADER_FILES 	= $(filter-out $(MAIN_H) $(HEADER_DIR)/counter.h, $(wildcard $(HEADER_DIR)/*.h))


# Object files
MYSORT_OBJ_FILES 	= $(patsubst $(SRC_DIR)/%.cpp, $(MYSORT_OBJ)/%.o, $(SRC_FILES)) $(MAIN_CPP:$(SRC_DIR)/%.cpp=$(MYSORT_OBJ)/%.o)
COUNT_OBJ_FILES 	= $(patsubst $(SRC_DIR)/%.cpp, $(COUNT_OBJ)/%.o, $(filter-out $(SRC_DIR)/main.cpp $(SRC_DIR)/counter.cpp, $(wildcard $(SRC_DIR)/*.cpp))) $(COUNT_OBJ)/counter.o
OBJ_FILES 			= $(MYSORT_OBJ_FILES) $(COUNT_OBJ_FILES)

# Output executables
MYSORT_EXE 		= mysort
COUNTER_EXE 	= counter

# Make makefile cross platform
ifeq ($(OS), Windows_NT)
    # Windows configuration
    RM 							= del /Q /F
    RM_EXECUTABLE_EXTENSION 	= .exe
    RM_OBJ_FILES 				= $(subst /,\,$(OBJ_FILES))
    RMDIR 						= rmdir /S /Q
    MKDIR 						= mkdir
else
    # Unix configuration
    RM 							= rm -f
    RM_EXECUTABLE_EXTENSION 	= ""
    RM_OBJ_FILES 				= $(OBJ_FILES)
    RMDIR 						= rm -rf
    MKDIR 						= mkdir -p
endif

# Object files for mysort

all: $(COUNTER_EXE) $(MYSORT_EXE)

#### MYSORT TARGET ####
# Create object directory
$(MYSORT_OBJ):
	$(MKDIR) $@

# Rule to compile source files into object files
$(MYSORT_OBJ)/%.o: $(SRC_DIR)/%.cpp $(HEADER_FILES) | $(MYSORT_OBJ)
	$(CC) $(CFLAGS) -c -o $@ $<

# Mysort Executable
$(MYSORT_EXE): $(MYSORT_OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

#### COUNTER TARGET ####
# Create object directory for counter
$(COUNT_OBJ):
	$(MKDIR) $@

# Rule to compile source files into object files for counter
$(COUNT_OBJ)/%.o: $(SRC_DIR)/%.cpp $(HEADER_FILES) $(HEADER_DIR)/counter.h | $(COUNT_OBJ)
	$(CC) $(CFLAGS) -c -o $@ $<

# Counter Executable
$(COUNTER_EXE): $(COUNT_OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Clean rule
clean:
	$(RM) $(RM_OBJ_FILES) $(MYSORT_EXE)$(RM_EXECUTABLE_EXTENSION) $(COUNTER_EXE)$(RM_EXECUTABLE_EXTENSION)
	$(RMDIR) $(OBJ_DIR)

.PHONY: all clean
