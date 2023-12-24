# Define compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Source file directory
SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
TESTS_DIR = tests
UNITY_DIR = Unity/src
OBJ_DIR = build
BIN_DIR = $(OBJ_DIR)/bin

# Unity source files
UNITY_SRC = $(UNITY_DIR)/unity.c

# Source files for the httpServer
HTTP_SERVER_SRC = $(SRC_DIR)/main.c $(SRC_DIR)/server.c $(SRC_DIR)/handler.c $(SRC_DIR)/utils.c

# Object files for httpServer
HTTP_SERVER_OBJ = $(OBJ_DIR)/main.o $(OBJ_DIR)/server.o $(OBJ_DIR)/handler.o $(OBJ_DIR)/utils.o

# Executable for httpServer
HTTP_SERVER_EXE = $(BIN_DIR)/httpServer

# Source files for each test
TEST_HANDLER_SRC = $(TESTS_DIR)/test_handler.c $(SRC_DIR)/handler.c $(UNITY_SRC)
TEST_SERVER_SRC = $(TESTS_DIR)/test_server.c $(SRC_DIR)/server.c $(UNITY_SRC)
TEST_UTILS_SRC = $(TESTS_DIR)/test_utils.c $(SRC_DIR)/utils.c $(UNITY_SRC)

# Executable names for each test
TEST_HANDLER_EXE = $(BIN_DIR)/test_handler
TEST_SERVER_EXE = $(BIN_DIR)/test_server
TEST_UTILS_EXE = $(BIN_DIR)/test_utils

# Default target, which builds httpServer
all: $(HTTP_SERVER_EXE)

# Target to build and run all tests
test: $(TEST_HANDLER_EXE) $(TEST_SERVER_EXE) $(TEST_UTILS_EXE)
	./$(TEST_HANDLER_EXE)
	./$(TEST_SERVER_EXE)
	./$(TEST_UTILS_EXE)

# Targets for httpServer
$(HTTP_SERVER_EXE): $(HTTP_SERVER_OBJ) | build
	$(CC) $(CFLAGS) -o $(HTTP_SERVER_EXE) $(HTTP_SERVER_OBJ)

# Targets: main.o, depends on main.c and server.h
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(INCLUDE_DIR)/server.h | build
	$(CC) $(CFLAGS) -c $< -I$(INCLUDE_DIR) -o $@

# Targets: server.o, depends on server.c, server.h, handler.h and utils.h
$(OBJ_DIR)/server.o: $(SRC_DIR)/server.c $(INCLUDE_DIR)/server.h $(INCLUDE_DIR)/handler.h $(INCLUDE_DIR)/utils.h | build
	$(CC) $(CFLAGS) -c $< -I$(INCLUDE_DIR) -o $@

# Targets: handler.o, depends on handler.c, server.h, handler.h and utils.h
$(OBJ_DIR)/handler.o: $(SRC_DIR)/handler.c $(INCLUDE_DIR)/server.h $(INCLUDE_DIR)/handler.h $(INCLUDE_DIR)/utils.h | build
	$(CC) $(CFLAGS) -c $< -I$(INCLUDE_DIR) -o $@

# Targets: utils.o, depends on utils.c and utils.h
$(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.c $(INCLUDE_DIR)/utils.h | build
	$(CC) $(CFLAGS) -c $< -I$(INCLUDE_DIR) -o $@

# Targets for tests
$(TEST_HANDLER_EXE): $(TEST_HANDLER_SRC) | build
	$(CC) $(CFLAGS) -o $(TEST_HANDLER_EXE) $^ -I$(INCLUDE_DIR) -I$(UNITY_DIR)

$(TEST_SERVER_EXE): $(TEST_SERVER_SRC) | build
	$(CC) $(CFLAGS) -o $(TEST_SERVER_EXE) $^ -I$(INCLUDE_DIR) -I$(UNITY_DIR)

$(TEST_UTILS_EXE): $(TEST_UTILS_SRC) | build
	$(CC) $(CFLAGS) -o $(TEST_UTILS_EXE) $^ -I$(INCLUDE_DIR) -I$(UNITY_DIR)

# Create build directory
build:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(HTTP_SERVER_EXE) $(TEST_HANDLER_EXE) $(TEST_SERVER_EXE) $(TEST_UTILS_EXE)
