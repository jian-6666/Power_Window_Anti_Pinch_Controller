# Contributing to Power Window Anti-Pinch Controller

Thank you for your interest in contributing to the Power Window Anti-Pinch Controller project! This document provides guidelines for contributing to this project.

## Code of Conduct

This project adheres to professional standards. Please be respectful and constructive in all interactions.

## Development Setup

### Prerequisites

- GCC compiler (C99 support)
- GNU Make
- Unity test framework (included)
- lcov/gcov for coverage analysis

### Building the Project

```bash
# Clone the repository
git clone https://github.com/jian-6666/Power_Window_Anti_Pinch_Controller.git
cd Power_Window_Anti_Pinch_Controller

# Build the project
make clean
make

# Run tests
make test

# Generate coverage report
make coverage
```

## Contribution Guidelines

### Code Standards

- Follow AUTOSAR C coding standards
- Use consistent naming conventions (PowerWindow_ModuleName_FunctionName)
- Maintain 95%+ statement coverage and 90%+ branch coverage
- All code must compile without warnings (-Wall -Wextra -Werror)

### Testing Requirements

- All new functions must have corresponding unit tests
- Property-based tests should be added for new behavioral properties
- Mock interfaces should be used for hardware abstraction
- Test coverage targets must be maintained

### Documentation

- Update API documentation for any interface changes
- Add inline comments for complex algorithms
- Update README.md if adding new features
- Maintain Chinese documentation for project reports

### Pull Request Process

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes following the coding standards
4. Add tests for new functionality
5. Ensure all tests pass (`make test`)
6. Check code coverage (`make coverage-check`)
7. Run static analysis (`make lint`)
8. Commit your changes (`git commit -m 'Add amazing feature'`)
9. Push to the branch (`git push origin feature/amazing-feature`)
10. Open a Pull Request

### Commit Message Format

Use clear, descriptive commit messages:

```
Add anti-pinch detection algorithm

- Implement current-based pinch detection
- Add configurable threshold parameters
- Include comprehensive unit tests
- Update API documentation
```

## Architecture Guidelines

### Module Design

- Each module should have a single responsibility
- Use clear interfaces between modules
- Avoid circular dependencies
- Implement proper error handling

### State Machine

- Follow the 8-state design pattern
- Ensure all state transitions are documented
- Maintain state transition priority rules
- Add logging for state changes

### Safety Features

- Safety functions have highest priority
- Implement proper fault debouncing
- Ensure graceful degradation
- Add comprehensive fault recovery

## Testing Guidelines

### Unit Tests

- Test normal operation paths
- Test boundary conditions
- Test error conditions
- Use descriptive test names

### Property Tests

- Define mathematical properties
- Use random input generation
- Run sufficient iterations (≥100)
- Document the property being tested

### Mock Usage

- Use mocks only for hardware interfaces
- Reset mocks between tests
- Keep mock behavior simple
- Verify mock interactions when relevant

## Documentation Standards

- Use clear, concise language
- Include code examples
- Maintain both English and Chinese versions
- Keep documentation synchronized with code

## Questions and Support

If you have questions about contributing, please:

1. Check existing documentation
2. Search existing issues
3. Create a new issue with the "question" label

## Recognition

Contributors will be acknowledged in the project documentation and release notes.

Thank you for contributing to the Power Window Anti-Pinch Controller project!