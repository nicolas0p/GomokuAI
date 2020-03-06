CC=g++-9
CFLAGS=-Wall -std=c++17 -pedantic
DEBUGFLAGS=-g -Og
TEST_FLAGS=--gtest_break_on_failure

TEST_LIB=-lgtest -pthread

EXEC=gomoku
TEST_EXEC=gomoku_test

SOURCE_DIR=src
HEADERS_DIR=include
TEST_DIR=test

MAIN_FILE :=$(SOURCE_DIR)/main.cpp
SOURCE_FILES :=$(filter-out $(MAIN_FILE),$(wildcard $(SOURCE_DIR)/*.cpp))
TEST_SOURCE_FILES :=$(wildcard $(TEST_DIR)/*.cpp)
.PHONY:make debug test clean
make:
	$(CC) -o $(EXEC) $(SOURCE_FILES) $(MAIN_FILE) $(CFLAGS) -I$(HEADERS_DIR)

debug:
	$(CC) -o $(EXEC) $(SOURCE_FILES) $(MAIN_FILE) $(CFLAGS) $(DEBUGFLAGS) -I$(HEADERS_DIR)

test:
	$(CC) -o $(TEST_EXEC) $(SOURCE_FILES) $(TEST_SOURCE_FILES) $(CFLAGS) $(DEBUGFLAGS) $(TEST_LIB) -I$(HEADERS_DIR)
	./$(TEST_EXEC) $(TEST_FLAGS)

clean:
	rm -f $(EXEC) $(TEST_EXEC) include/*.h.gch
