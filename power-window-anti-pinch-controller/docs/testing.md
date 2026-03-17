# Power Window Anti-Pinch Controller - Testing Guide

## Overview

This document provides comprehensive testing guidance for the Power Window Anti-Pinch Controller system, including test environment setup, execution procedures, coverage targets, and test case development guidelines.

## Testing Strategy

The system employs a dual testing approach to ensure comprehensive validation:

### 1. Unit Testing
- **Framework**: Unity Test Framework
- **Purpose**: Validate individual functions and modules
- **Focus**: Specific examples, boundary conditions, error scenarios
- **Coverage**: Function-level validation

### 2. Property-Based Testing
- **Purpose**: Validate universal properties across all inputs
- **Focus**: System invariants, behavioral properties
- **Coverage**: Input space exploration through random generation

## Test Environment Setup

### Prerequisites

#### Required Tools
- **GCC Compiler**: Version 7.0+ with C99 support
- **GNU Make**: Build automation
- **Unity Framework**: Included in project
- **gcov/lcov**: Coverage analysis tools

#### Optional Tools
- **Valgrind**: Memory leak detection (Linux)
- **cppcheck**: Static code analysis
- **clang-format**: Code formatting verification

### Installation Steps

#### Ubuntu/Debian
```bash
# Install required packages
sudo apt-get update
sudo apt-get install build-essential gcc make lcov cppcheck

# Verify installation
gcc --version
make --version
lcov --version
```

#### macOS
```bash
# Install Xcode command line tools
xcode-select --install

# Install additional tools via Homebrew
brew install lcov cppcheck

# Verify installation
gcc --version
make --version
```

#### Windows (MinGW)
```bash
# Install MinGW-w64
# Download from: https://www.mingw-w64.org/

# Add to PATH and verify
gcc --version
make --version
```

### Project Setup

```bash
# Clone the repository
git clone <repository-url>
cd power-window-anti-pinch-controller

# Verify project structure
ls -la
# Should show: docs/ inc/ src/ test/ config/ Makefile README.md

# Initial build test
make clean
make
```

## Test Execution

### Running All Tests

```bash
# Run complete test suite
make test

# Expected output:
# Compiling tests...
# Running Unity tests...
# All tests passed: XX/XX
# Test execution time: X.XXs
```

### Running Specific Test Modules

```bash
# Run individual test modules
./build/test_runner --filter="PowerWindow"
./build/test_runner --filter="StateMachine"
./build/test_runner --filter="Safety"
./build/test_runner --filter="NvM"
./build/test_runner --filter="Diag"
./build/test_runner --filter="Interface"
```

### Running Property-Based Tests

```bash
# Run property tests with default iterations (100)
make test-properties

# Run with custom iteration count
make test-properties ITERATIONS=1000

# Run specific property tests
./build/property_test_runner --property="ButtonInputResponse"
./build/property_test_runner --property="PositionConversion"
```

### Coverage Analysis

```bash
# Generate coverage report
make coverage

# View coverage summary
cat coverage/summary.txt

# Open detailed HTML report
open coverage/index.html  # macOS
xdg-open coverage/index.html  # Linux
start coverage/index.html  # Windows
```

## Test Coverage Targets

### Coverage Metrics

| Metric | Target | Minimum Acceptable |
|--------|--------|--------------------|
| Statement Coverage | ≥ 95% | ≥ 90% |
| Branch Coverage | ≥ 90% | ≥ 85% |
| Function Coverage | 100% | 100% |
| Property Coverage | 100% | 100% |

### Coverage Verification

```bash
# Check current coverage
make coverage-check

# Expected output:
# Statement Coverage: 96.2% (Target: 95%)  ✓
# Branch Coverage: 91.5% (Target: 90%)     ✓
# Function Coverage: 100% (Target: 100%)   ✓
# Property Coverage: 100% (Target: 100%)   ✓
```

### Coverage Exclusions

Certain code sections are excluded from coverage requirements:

```c
/* Coverage exclusion examples */

// LCOV_EXCL_START
static void PowerWindow_DebugPrint(const char* message) {
    #ifdef DEBUG_MODE
    printf("[DEBUG] %s\n", message);
    #endif
}
// LCOV_EXCL_STOP

// Single line exclusion
assert(ptr != NULL); // LCOV_EXCL_LINE
```

## Test Case Development

### Unit Test Guidelines

#### Test Structure
```c
/* Test file: test_powerwindow.c */
#include "unity.h"
#include "PowerWindow.h"
#include "PowerWindow_Interface.h"  // For mock functions

void setUp(void) {
    /* Called before each test */
    PowerWindow_Interface_Mock_Reset();
    PowerWindow_Init();
}

void tearDown(void) {
    /* Called after each test */
    // Cleanup if needed
}

void test_PowerWindow_Init_ShouldInitializeToIdleState(void) {
    // Arrange
    // (Setup done in setUp())
    
    // Act
    PowerWindow_SystemStateType state = PowerWindow_GetCurrentState();
    
    // Assert
    TEST_ASSERT_EQUAL(POWERWINDOW_STATE_IDLE, state);
}
```

#### Naming Conventions
- Test files: `test_<module>.c`
- Test functions: `test_<Module>_<Function>_<Scenario>(void)`
- Examples:
  - `test_PowerWindow_SetButtonState_ShouldStartManualUp(void)`
  - `test_Safety_MonitorAntiPinch_ShouldDetectHighCurrent(void)`

#### Test Categories

**1. Happy Path Tests**
```c
void test_PowerWindow_ManualUp_ShouldMoveWindowUp(void) {
    // Setup normal conditions
    PowerWindow_Interface_Mock_SetPosition(2000);
    PowerWindow_Interface_Mock_SetVoltage(120);
    PowerWindow_Interface_Mock_SetCurrent(50);
    
    // Press UP button
    PowerWindow_SetButtonState(POWERWINDOW_BUTTON_UP);
    PowerWindow_MainFunction_10ms();
    
    // Verify motor moves up
    TEST_ASSERT_EQUAL(POWERWINDOW_MOTOR_UP, 
                      PowerWindow_Interface_Mock_GetMotor());
    TEST_ASSERT_EQUAL(POWERWINDOW_STATE_MANUAL_UP, 
                      PowerWindow_GetCurrentState());
}
```

**2. Boundary Tests**
```c
void test_PowerWindow_ManualUp_ShouldStopAtUpperLimit(void) {
    // Setup at upper limit
    PowerWindow_Interface_Mock_SetPosition(4095);
    
    // Try to move up
    PowerWindow_SetButtonState(POWERWINDOW_BUTTON_UP);
    PowerWindow_MainFunction_10ms();
    
    // Verify motor doesn't move
    TEST_ASSERT_EQUAL(POWERWINDOW_MOTOR_STOP, 
                      PowerWindow_Interface_Mock_GetMotor());
    TEST_ASSERT_EQUAL(POWERWINDOW_STATE_IDLE, 
                      PowerWindow_GetCurrentState());
}
```

**3. Error Condition Tests**
```c
void test_PowerWindow_UnderVoltage_ShouldEnterFaultState(void) {
    // Setup undervoltage condition
    PowerWindow_Interface_Mock_SetVoltage(85);  // 8.5V < 10V threshold
    
    // Run for debounce time
    for (int i = 0; i < 25; i++) {  // 250ms > 200ms debounce
        PowerWindow_MainFunction_10ms();
    }
    
    // Verify fault state
    TEST_ASSERT_EQUAL(POWERWINDOW_STATE_FAULT, 
                      PowerWindow_GetCurrentState());
    TEST_ASSERT_TRUE(PowerWindow_Diag_IsDTCActive(POWERWINDOW_DTC_UNDERVOLTAGE));
}
```

### Property-Based Test Guidelines

#### Property Test Structure
```c
/* Property test example */
void test_property_position_conversion_roundtrip(void) {
    for (int iteration = 0; iteration < 100; iteration++) {
        // Generate random test data
        PowerWindow_PositionType lowerLimit = rand() % 1000;
        PowerWindow_PositionType upperLimit = lowerLimit + 1000 + (rand() % 3000);
        PowerWindow_PositionType rawPosition = lowerLimit + 
                                               (rand() % (upperLimit - lowerLimit));
        
        // Convert to percentage and back
        PowerWindow_PercentType percent = PowerWindow_ConvertPositionToPercent(
            rawPosition, lowerLimit, upperLimit);
        
        // Verify properties
        TEST_ASSERT_TRUE(percent >= 0 && percent <= 100);
        
        // Verify monotonicity
        if (rawPosition == lowerLimit) {
            TEST_ASSERT_EQUAL(0, percent);
        }
        if (rawPosition == upperLimit) {
            TEST_ASSERT_EQUAL(100, percent);
        }
        
        // Verify relative accuracy (within 1%)
        uint32_t expectedPercent = ((uint32_t)(rawPosition - lowerLimit) * 100) / 
                                   (upperLimit - lowerLimit);
        TEST_ASSERT_INT_WITHIN(1, expectedPercent, percent);
    }
}
```

#### Property Annotations
Each property test must be annotated with the corresponding design property:

```c
/* Feature: power-window-anti-pinch-controller, Property 3: 位置转换正确性 */
void test_property_position_conversion_accuracy(void) {
    // Property test implementation
}
```

### Mock Interface Usage

#### Setting Up Mock Data
```c
void test_setup_normal_conditions(void) {
    PowerWindow_Interface_Mock_SetButton(POWERWINDOW_BUTTON_RELEASED);
    PowerWindow_Interface_Mock_SetPosition(2048);  // 50% position
    PowerWindow_Interface_Mock_SetCurrent(30);     // 3.0A normal current
    PowerWindow_Interface_Mock_SetVoltage(120);    // 12.0V normal voltage
}

void test_setup_fault_conditions(void) {
    PowerWindow_Interface_Mock_SetVoltage(85);     // 8.5V undervoltage
    PowerWindow_Interface_Mock_SetCurrent(250);    // 25.0A overcurrent
}
```

#### Verifying Mock Outputs
```c
void test_verify_motor_control(void) {
    // Test motor control
    PowerWindow_SetButtonState(POWERWINDOW_BUTTON_UP);
    PowerWindow_MainFunction_10ms();
    
    // Verify mock captured the output
    PowerWindow_MotorDirectionType direction = PowerWindow_Interface_Mock_GetMotor();
    TEST_ASSERT_EQUAL(POWERWINDOW_MOTOR_UP, direction);
}
```

## Test Execution Commands

### Basic Test Commands

```bash
# Clean build and test
make clean test

# Test with verbose output
make test VERBOSE=1

# Test with specific compiler flags
make test CFLAGS="-DDEBUG_MODE -g"

# Test in release mode
make test CFLAGS="-O2 -DNDEBUG"
```

### Advanced Test Commands

```bash
# Run tests with memory checking (Linux)
make test-valgrind

# Run tests with thread sanitizer
make test CFLAGS="-fsanitize=thread"

# Run tests with address sanitizer
make test CFLAGS="-fsanitize=address"

# Run performance benchmarks
make test-performance
```

### Continuous Integration Commands

```bash
# CI test script
#!/bin/bash
set -e

echo "Running Power Window Controller Tests..."

# Clean build
make clean

# Compile with warnings as errors
make CFLAGS="-Wall -Wextra -Werror"

# Run all tests
make test

# Generate coverage report
make coverage

# Check coverage targets
make coverage-check

# Run static analysis
make lint

echo "All tests passed successfully!"
```

## Test Results and Reporting

### Test Output Format

```
Power Window Anti-Pinch Controller Test Results
==============================================

Unity Test Framework v2.5.2
Test execution started: 2024-01-15 10:30:00

Module: PowerWindow
  ✓ test_PowerWindow_Init_ShouldInitializeToIdleState
  ✓ test_PowerWindow_SetButtonState_ShouldStartManualUp
  ✓ test_PowerWindow_ManualUp_ShouldStopAtUpperLimit
  ✓ test_PowerWindow_AntiPinch_ShouldRetractOnHighCurrent
  Tests: 4, Failures: 0, Time: 0.025s

Module: StateMachine
  ✓ test_StateMachine_Init_ShouldStartUninitialized
  ✓ test_StateMachine_Transition_IdleToManualUp
  ✓ test_StateMachine_Transition_ManualUpToAntiPinch
  Tests: 3, Failures: 0, Time: 0.018s

[... other modules ...]

Property Tests
  ✓ Property 1: Button Input Response (100 iterations)
  ✓ Property 3: Position Conversion Accuracy (100 iterations)
  ✓ Property 6: Anti-Pinch Detection (100 iterations)
  Properties: 24, Failures: 0, Time: 2.150s

==============================================
Total Tests: 127
Passed: 127
Failed: 0
Total Time: 3.245s

Coverage Summary:
  Statement Coverage: 96.2% (Target: 95%) ✓
  Branch Coverage: 91.5% (Target: 90%)    ✓
  Function Coverage: 100% (Target: 100%)  ✓
```

### Coverage Report Structure

```
coverage/
├── index.html              # Main coverage report
├── summary.txt             # Text summary
├── PowerWindow.c.html      # Module-specific reports
├── PowerWindow_Safety.c.html
├── PowerWindow_StateMachine.c.html
└── ...
```

### Test Artifacts

```
build/
├── test_runner             # Main test executable
├── property_test_runner    # Property test executable
├── test_results.xml        # JUnit format results
├── coverage.info           # LCOV coverage data
└── test_logs/
    ├── unit_tests.log      # Detailed unit test log
    ├── property_tests.log  # Property test log
    └── coverage.log        # Coverage generation log
```

## Troubleshooting

### Common Issues

#### 1. Compilation Errors
```bash
# Error: undefined reference to Unity functions
# Solution: Ensure Unity is properly linked
make clean
make test VERBOSE=1  # Check linking commands
```

#### 2. Test Failures
```bash
# Error: Mock functions not working
# Solution: Verify test mode compilation
make test CFLAGS="-DPOWERWINDOW_TEST_MODE"
```

#### 3. Coverage Issues
```bash
# Error: Low coverage reported
# Solution: Check for excluded files
lcov --list coverage.info | grep -v "100.0%"
```

#### 4. Memory Issues
```bash
# Error: Segmentation fault in tests
# Solution: Run with debugging
gdb ./build/test_runner
(gdb) run
(gdb) bt  # Get backtrace on crash
```

### Debug Test Execution

```bash
# Run single test with debugging
gdb --args ./build/test_runner --filter="PowerWindow_Init"

# Enable debug output
make test CFLAGS="-DDEBUG_MODE -DTEST_DEBUG"

# Run with verbose Unity output
make test CFLAGS="-DUNITY_VERBOSE"
```

### Performance Profiling

```bash
# Profile test execution
make test-profile

# Analyze performance bottlenecks
gprof ./build/test_runner gmon.out > profile_report.txt
```

## Test Maintenance

### Adding New Tests

1. **Create test file** (if new module):
   ```bash
   cp test/test_template.c test/test_newmodule.c
   ```

2. **Update Makefile**:
   ```makefile
   TEST_SRCS += test/test_newmodule.c
   ```

3. **Implement tests** following naming conventions

4. **Verify coverage**:
   ```bash
   make coverage
   # Check that new module appears in report
   ```

### Updating Existing Tests

1. **Modify test functions** as needed
2. **Update property tests** if behavior changes
3. **Verify all tests still pass**:
   ```bash
   make test
   ```
4. **Check coverage impact**:
   ```bash
   make coverage-check
   ```

### Test Review Checklist

- [ ] All new functions have corresponding tests
- [ ] Boundary conditions are tested
- [ ] Error conditions are tested
- [ ] Property tests cover new behaviors
- [ ] Coverage targets are met
- [ ] Tests follow naming conventions
- [ ] Mock interfaces are used correctly
- [ ] Test documentation is updated

## Continuous Integration

### GitHub Actions Example

```yaml
name: Power Window Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v2
    
    - name: Install dependencies
      run: |
        sudo apt-get update
        sudo apt-get install build-essential lcov
    
    - name: Build and test
      run: |
        make clean
        make test
        make coverage
    
    - name: Check coverage
      run: make coverage-check
    
    - name: Upload coverage
      uses: codecov/codecov-action@v1
      with:
        file: ./coverage.info
```

### Jenkins Pipeline Example

```groovy
pipeline {
    agent any
    
    stages {
        stage('Build') {
            steps {
                sh 'make clean'
                sh 'make'
            }
        }
        
        stage('Test') {
            steps {
                sh 'make test'
                sh 'make coverage'
            }
            
            post {
                always {
                    publishTestResults testResultsPattern: 'build/test_results.xml'
                    publishHTML([
                        allowMissing: false,
                        alwaysLinkToLastBuild: true,
                        keepAll: true,
                        reportDir: 'coverage',
                        reportFiles: 'index.html',
                        reportName: 'Coverage Report'
                    ])
                }
            }
        }
        
        stage('Quality Check') {
            steps {
                sh 'make lint'
                sh 'make coverage-check'
            }
        }
    }
}
```

## Best Practices

### Test Design Principles

1. **Independence**: Tests should not depend on each other
2. **Repeatability**: Tests should produce consistent results
3. **Fast Execution**: Unit tests should complete quickly
4. **Clear Intent**: Test names should describe what is being tested
5. **Comprehensive**: Cover normal, boundary, and error conditions

### Mock Usage Guidelines

1. **Use mocks for hardware interfaces only**
2. **Reset mocks between tests**
3. **Verify mock interactions when relevant**
4. **Keep mock behavior simple and predictable**

### Property Test Guidelines

1. **Focus on invariants and universal properties**
2. **Use appropriate input generators**
3. **Run sufficient iterations (≥100)**
4. **Document the property being tested**

---

## Version History

- **v1.0.0** - Initial testing framework and guidelines
- Current implementation supports comprehensive unit and property testing

**Document Version**: 1.0.0  
**Last Updated**: 2024  
**Status**: Complete