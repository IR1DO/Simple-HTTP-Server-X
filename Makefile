# Define compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Source file directory
SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
TESTS_DIR = tests
UNITY_DIR = Unity/src

# Unity source files
UNITY_SRC = $(UNITY_DIR)/unity.c

# Source files for the httpServer
HTTP_SERVER_SRC = $(SRC_DIR)/main.c $(SRC_DIR)/server.c $(SRC_DIR)/handler.c $(SRC_DIR)/utils.c

# Object files for httpServer
HTTP_SERVER_OBJ = build/main.o build/server.o build/handler.o build/utils.o

# Executable for httpServer
HTTP_SERVER_EXE = build/bin/httpServer

# Source files for each test
TEST_HANDLER_SRC = $(TESTS_DIR)/test_handler.c $(SRC_DIR)/handler.c $(UNITY_SRC)
TEST_SERVER_SRC = $(TESTS_DIR)/test_server.c $(SRC_DIR)/server.c $(UNITY_SRC)
TEST_UTILS_SRC = $(TESTS_DIR)/test_utils.c $(SRC_DIR)/utils.c $(UNITY_SRC)

# Executable names for each test
TEST_HANDLER_EXE = build/bin/test_handler
TEST_SERVER_EXE = build/bin/test_server
TEST_UTILS_EXE = build/bin/test_utils

# Default target, which builds httpServer
all: $(HTTP_SERVER_EXE)

# Target to build and run all tests
test: $(TEST_HANDLER_EXE) $(TEST_SERVER_EXE) $(TEST_UTILS_EXE)
	./$(TEST_HANDLER_EXE)
	./$(TEST_SERVER_EXE)
	./$(TEST_UTILS_EXE)

# Targets for httpServer
$(HTTP_SERVER_EXE): $(HTTP_SERVER_OBJ)
	$(CC) $(CFLAGS) -o $(HTTP_SERVER_EXE) $(HTTP_SERVER_OBJ)

build/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/%.h | build
	$(CC) $(CFLAGS) -c $< -I$(INCLUDE_DIR) -o $@

# Targets for tests
$(TEST_HANDLER_EXE): $(TEST_HANDLER_SRC)
	$(CC) $(CFLAGS) -o $(TEST_HANDLER_EXE) $^ -I$(INCLUDE_DIR) -I$(UNITY_DIR)

$(TEST_SERVER_EXE): $(TEST_SERVER_SRC)
	$(CC) $(CFLAGS) -o $(TEST_SERVER_EXE) $^ -I$(INCLUDE_DIR) -I$(UNITY_DIR)

$(TEST_UTILS_EXE): $(TEST_UTILS_SRC)
	$(CC) $(CFLAGS) -o $(TEST_UTILS_EXE) $^ -I$(INCLUDE_DIR) -I$(UNITY_DIR)

# Clean target
clean:
	rm -f $(HTTP_SERVER_EXE) $(HTTP_SERVER_OBJ) $(TEST_HANDLER_EXE) $(TEST_SERVER_EXE) $(TEST_UTILS_EXE)
