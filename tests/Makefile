# Define the compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Werror

# Unity source files
UNITY_SRC = Unity/src/unity.c

# Source files for each test
TEST_HANDLER_SRC = test_handler.c ../src/handler.c
TEST_SERVER_SRC = test_server.c ../src/server.c
TEST_UTILS_SRC = test_utils.c ../src/utils.c

# Executable names for each test
TEST_HANDLER_EXE = test_handler
TEST_SERVER_EXE = test_server
TEST_UTILS_EXE = test_utils

# Default target, which builds and runs all tests
all: test

# Target to build and run all tests
test: test_handler test_server test_utils
	./$(TEST_HANDLER_EXE)
	./$(TEST_SERVER_EXE)
	./$(TEST_UTILS_EXE)

# Target to build the handler test
test_handler: $(TEST_HANDLER_SRC) $(UNITY_SRC)
	$(CC) $(CFLAGS) -o $(TEST_HANDLER_EXE) $^

# Target to build the server test
test_server: $(TEST_SERVER_SRC) $(UNITY_SRC)
	$(CC) $(CFLAGS) -o $(TEST_SERVER_EXE) $^

# Target to build the utils test
test_utils: $(TEST_UTILS_SRC) $(UNITY_SRC)
	$(CC) $(CFLAGS) -o $(TEST_UTILS_EXE) $^

# Clean target
clean:
	rm -f $(TEST_HANDLER_EXE) $(TEST_SERVER_EXE) $(TEST_UTILS_EXE)