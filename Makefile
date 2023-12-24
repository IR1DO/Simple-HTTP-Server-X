# Define compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Source file directory
SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
OBJ_DIR = build

# Targets: httpServer
httpServer: $(OBJ_DIR)/main.o $(OBJ_DIR)/server.o $(OBJ_DIR)/handler.o $(OBJ_DIR)/utils.o
	$(CC) $(CFLAGS) -o httpServer $(OBJ_DIR)/main.o $(OBJ_DIR)/server.o $(OBJ_DIR)/handler.o $(OBJ_DIR)/utils.o

# Targets: main.o, depends on main.c
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(INCLUDE_DIR)/server.h
	$(CC) $(CFLAGS) -c $< -I$(INCLUDE_DIR) -o $@

# Targets: server.o, depends on server.c, server.h, handler.h and utils.h
$(OBJ_DIR)/server.o: $(SRC_DIR)/server.c $(INCLUDE_DIR)/server.h $(INCLUDE_DIR)/handler.h $(INCLUDE_DIR)/utils.h
	$(CC) $(CFLAGS) -c $< -I$(INCLUDE_DIR) -o $@

# Targets: handler.o, depends on handler.c, server.h, handler.h and utils.h
$(OBJ_DIR)/handler.o: $(SRC_DIR)/handler.c $(INCLUDE_DIR)/server.h $(INCLUDE_DIR)/handler.h $(INCLUDE_DIR)/utils.h
	$(CC) $(CFLAGS) -c $< -I$(INCLUDE_DIR) -o $@

# Targets: utils.o, depends on utils.c and utils.h
$(OBJ_DIR)/utils.o: $(SRC_DIR)/utils.c $(INCLUDE_DIR)/utils.h
	$(CC) $(CFLAGS) -c $< -I$(INCLUDE_DIR) -o $@

# Targets: clean, delete generated files
clean:
	rm -f httpServer $(OBJ_DIR)/*.o