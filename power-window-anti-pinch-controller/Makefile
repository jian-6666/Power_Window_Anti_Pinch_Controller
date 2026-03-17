# Power Window Anti-Pinch Controller Makefile
# Supports: compilation, testing, coverage analysis

# Compiler and tools
CC := gcc
AR := ar
GCOV := gcov
LCOV := lcov
GENHTML := genhtml

# Directories
SRC_DIR := src
INC_DIR := inc
TEST_DIR := test
BUILD_DIR := build
COVERAGE_DIR := coverage
CONFIG_DIR := config

# Compiler flags
CFLAGS := -std=c99 -Wall -Wextra -Werror -pedantic
CFLAGS += -I$(INC_DIR) -I$(CONFIG_DIR)
CFLAGS += -DPOWERWINDOW_TEST_MODE

# Coverage flags
COVERAGE_FLAGS := --coverage -fprofile-arcs -ftest-coverage

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

# Test files
TEST_SOURCES := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/test_%.o,$(TEST_SOURCES))

# Exclude main.c from library build
LIB_SOURCES := $(filter-out $(SRC_DIR)/main.c,$(SOURCES))
LIB_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SOURCES))

# Output files
LIBRARY := $(BUILD_DIR)/libpowerwindow.a
TEST_RUNNER := $(BUILD_DIR)/test_runner
COVERAGE_REPORT := $(COVERAGE_DIR)/index.html

# Default target
.PHONY: all
all: $(LIBRARY)

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Create coverage directory
$(COVERAGE_DIR):
	@mkdir -p $(COVERAGE_DIR)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files with coverage
$(BUILD_DIR)/test_%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c $< -o $@

# Build static library
$(LIBRARY): $(LIB_OBJECTS) | $(BUILD_DIR)
	$(AR) rcs $@ $^
	@echo "Library built: $@"

# Build test runner
$(TEST_RUNNER): $(LIB_OBJECTS) $(TEST_OBJECTS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) $^ -o $@
	@echo "Test runner built: $@"

# Run tests
.PHONY: test
test: $(TEST_RUNNER)
	@echo "Running tests..."
	@$(TEST_RUNNER)
	@echo "Tests completed"

# Generate coverage report
.PHONY: coverage
coverage: clean test | $(COVERAGE_DIR)
	@echo "Generating coverage report..."
	@$(LCOV) --directory $(BUILD_DIR) --capture --output-file $(COVERAGE_DIR)/coverage.info
	@$(LCOV) --remove $(COVERAGE_DIR)/coverage.info '/usr/*' '*/test/*' --output-file $(COVERAGE_DIR)/coverage.info
	@$(GENHTML) $(COVERAGE_DIR)/coverage.info --output-directory $(COVERAGE_DIR)
	@echo "Coverage report generated: $(COVERAGE_REPORT)"

# Static code analysis (if cppcheck is available)
.PHONY: lint
lint:
	@command -v cppcheck >/dev/null 2>&1 || { echo "cppcheck not installed"; exit 1; }
	@cppcheck --enable=all --suppress=missingIncludeSystem $(SRC_DIR) $(INC_DIR) $(TEST_DIR)

# Code formatting (if clang-format is available)
.PHONY: format
format:
	@command -v clang-format >/dev/null 2>&1 || { echo "clang-format not installed"; exit 1; }
	@find $(SRC_DIR) $(INC_DIR) $(TEST_DIR) -name "*.c" -o -name "*.h" | xargs clang-format -i

# Check formatting
.PHONY: format-check
format-check:
	@command -v clang-format >/dev/null 2>&1 || { echo "clang-format not installed"; exit 1; }
	@find $(SRC_DIR) $(INC_DIR) $(TEST_DIR) -name "*.c" -o -name "*.h" | xargs clang-format --dry-run -Werror

# Clean build artifacts
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Build artifacts cleaned"

# Clean all including coverage
.PHONY: clean-all
clean-all: clean
	@rm -rf $(COVERAGE_DIR)
	@echo "All artifacts cleaned"

# Help target
.PHONY: help
help:
	@echo "Power Window Anti-Pinch Controller - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all              - Build the library (default)"
	@echo "  test             - Run unit tests"
	@echo "  coverage         - Generate code coverage report"
	@echo "  lint             - Run static code analysis"
	@echo "  format           - Format code with clang-format"
	@echo "  format-check     - Check code formatting"
	@echo "  clean            - Remove build artifacts"
	@echo "  clean-all        - Remove all generated files"
	@echo "  help             - Display this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make             - Build the library"
	@echo "  make test        - Run tests"
	@echo "  make coverage    - Generate coverage report"
	@echo "  make clean       - Clean build directory"

.PHONY: .SILENT
