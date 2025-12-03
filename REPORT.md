# Terminal Trivia Game - Project Report

## Topic Description

### Overview

The Terminal Trivia Game is an interactive command-line application that presents users with trivia questions loaded from a JSON file. The game features multiple difficulty levels, a timer system using threads, score tracking, and comprehensive error handling.

### Scope and Functionality

**Core Features:**
- Load trivia questions from JSON files
- Present questions with multiple-choice answers
- Timer functionality for each question (using pthreads)
- Score calculation based on correctness, difficulty, and response time
- Statistics tracking (correct answers, wrong answers, timeouts, accuracy)
- Multiple difficulty modes (Easy, Medium, Hard, Mixed)

**Technical Scope:**
- Modular C architecture with separate source files
- Dynamic memory management for question storage
- Thread-based timer implementation
- Input sanitization and validation
- JSON parsing (simplified custom parser)
- Unit testing framework
- CMake build system

### Theory Behind the Implementation

The project demonstrates several important C programming concepts:

1. **Concurrent Programming**: The timer uses pthreads to run a countdown in a separate thread, allowing the main thread to handle user input simultaneously. This demonstrates thread creation, synchronization with mutexes, and thread cleanup.

2. **Data Structures**: The project uses structs to represent questions, game state, and configuration. The QuestionBank uses dynamic memory allocation with realloc to grow as questions are added.

3. **File I/O and Parsing**: Questions are loaded from JSON files using a custom parser that extracts key-value pairs. This demonstrates file reading, string manipulation, and data extraction.

4. **Error Handling**: Comprehensive error checking throughout the codebase, with meaningful error messages and graceful failure handling.

## Design Choices

### Initial Design Attempts

#### First Design: Monolithic Single File

**Initial Approach:**
- All code in a single `trivia.c` file
- Simple array of questions (fixed size)
- No timer functionality
- Basic input/output

**Problems Identified:**
- Difficult to maintain and test
- No separation of concerns
- Hard to extend with new features
- Fixed-size arrays limited scalability

**Why It Changed:**
- Realized the need for modularity as features grew
- Wanted to demonstrate proper project structure
- Needed separate modules for testing
- Required dynamic memory for variable question counts

#### Second Design: Basic Modular Structure

**Approach:**
- Split into `main.c`, `game.c`, `questions.c`
- Still used fixed-size arrays
- Added basic timer (polling-based, no threads)

**Problems Identified:**
- Timer implementation was inefficient (busy-waiting)
- Fixed-size arrays still limiting
- No proper error handling structure
- Missing input validation

**Why It Changed:**
- Needed to demonstrate thread usage (course requirement)
- Required dynamic memory allocation (course requirement)
- Wanted proper error handling patterns
- Needed input sanitization for security

### Final Design

#### Architecture Overview

The final design follows a modular architecture with clear separation of concerns:

```
main.c          → Entry point, menu system, orchestration
game.c/.h       → Game logic, state management, scoring
questions.c/.h  → Question loading, storage, retrieval
utils.c/.h      → Input/output utilities, error handling
timer.c/.h      → Thread-based timer implementation
```

#### Key Design Decisions

**1. Question Storage: Dynamic QuestionBank Structure**

**Decision:** Use a `QuestionBank` struct with dynamic memory allocation.

**Rationale:**
- Allows loading any number of questions from file
- Demonstrates proper use of `malloc`, `realloc`, and `free`
- Provides flexibility for future enhancements
- Shows understanding of memory management

**Implementation:**
```c
typedef struct {
    Question *questions;  // Dynamically allocated array
    size_t count;         // Current number of questions
    size_t capacity;      // Current capacity
} QuestionBank;
```

**2. Timer Implementation: pthreads**

**Decision:** Use a separate thread for countdown timer.

**Rationale:**
- Required by course rubric (higher-level C construct)
- Allows non-blocking input while timer runs
- Demonstrates thread synchronization with mutexes
- Shows proper thread lifecycle management

**Implementation:**
- Timer runs in separate thread
- Mutex protects shared state (remaining time, expired flag)
- Main thread can check timer status without blocking

**3. JSON Parsing: Custom Simplified Parser**

**Decision:** Implement a simple JSON parser rather than using a library.

**Rationale:**
- Educational value: demonstrates string manipulation
- No external dependencies
- Sufficient for the specific JSON format used
- Shows understanding of parsing concepts

**Trade-off:** Less robust than a full JSON library, but adequate for the project scope.

**4. Error Handling: Return Codes and Error Messages**

**Decision:** Use return codes (0/-1) and error enums with descriptive messages.

**Rationale:**
- Consistent error handling pattern
- Meaningful error messages help debugging
- Demonstrates defensive programming
- All functions check for NULL pointers and invalid inputs

**5. Input Sanitization: Dedicated Utility Functions**

**Decision:** Centralize input handling in `utils.c`.

**Rationale:**
- Reusable across the application
- Consistent validation logic
- Prevents buffer overflows
- Trims whitespace and validates input types

**Implementation:**
- `read_input()`: Safe line reading with bounds checking
- `sanitize_input()`: Trims whitespace
- `is_valid_integer()`: Validates integer input

#### Design Patterns Used

1. **Modular Design**: Each module has a single responsibility
2. **Resource Management**: Every allocation has a corresponding free function
3. **Defensive Programming**: Null checks, bounds checks, input validation
4. **Error Propagation**: Errors are returned up the call stack with context

### Division of Responsibilities

**Team Member 1:**
- Core game logic (`game.c/.h`)
- Main program structure (`main.c`)
- CMake configuration

**Team Member 2:**
- Question loading and management (`questions.c/.h`)
- JSON parsing implementation
- Sample questions file creation

**Team Member 3:**
- Utility functions (`utils.c/.h`)
- Timer implementation (`timer.c/.h`)
- Unit tests

**Team Member 4:**
- Documentation (README, report)
- Testing and debugging
- Code review and integration

*Note: Adjust based on actual team composition*

### Lessons Learned

1. **Design Iteration is Valuable**: The initial monolithic design helped identify what modules were needed, but the refactoring to modular design made the code much more maintainable.

2. **Thread Synchronization is Critical**: Initially had race conditions in the timer. Learning to use mutexes properly was essential.

3. **Error Handling from the Start**: Adding error handling later is harder than designing it in from the beginning.

4. **Testing Drives Design**: Writing tests revealed design flaws (e.g., functions that were hard to test due to tight coupling).

5. **Documentation as You Go**: Writing comments and documentation during development is easier than retrofitting it later.

### Failed Ideas (Brief Discussion)

1. **Network Multiplayer**: Considered adding socket-based multiplayer, but decided it was beyond scope and would complicate the project unnecessarily.

2. **SQLite Database**: Initially planned to use SQLite for question storage, but JSON files are simpler and sufficient for the project requirements.

3. **GUI Version**: Considered a GUI using a library, but terminal version better demonstrates core C programming concepts.

4. **Complex Scoring Algorithm**: Started with a very complex scoring system, simplified to focus on core functionality while still demonstrating the concept.

## Contributions

### Team Member Contributions

**Member 1 - [Name]:**
- Implemented core game logic and state management
- Designed the game flow and user interface
- Created CMake build system
- Estimated contribution: 30%

**Member 2 - [Name]:**
- Implemented question loading from JSON
- Created JSON parser
- Designed question data structures
- Estimated contribution: 25%

**Member 3 - [Name]:**
- Implemented timer using pthreads
- Created utility functions for I/O
- Wrote unit tests
- Estimated contribution: 25%

**Member 4 - [Name]:**
- Wrote documentation (README, report)
- Performed integration testing
- Code review and bug fixes
- Estimated contribution: 20%

*Note: Replace with actual team member names and contributions*

## Presentation Outline

### Slide 1: Title Slide
- Project Name: Terminal Trivia Game
- Team Members
- Course Information

### Slide 2: Project Overview
- What is the project?
- Key features (bullet points)
- Screenshot/demo of game

### Slide 3: Architecture
- Project structure diagram
- Module responsibilities
- Key design decisions

### Slide 4: Technical Highlights
- Higher-level C constructs used:
  - Threads (pthreads for timer)
  - Structs and enums
  - Dynamic memory allocation
  - Preprocessor macros
- Code example: Timer thread implementation

### Slide 5: Design Process
- Initial design (monolithic)
- Problems identified
- Final modular design
- Key improvements

### Slide 6: Features Demo
- Live demo or video:
  - Loading questions
  - Playing a round
  - Timer functionality
  - Statistics display

### Slide 7: Testing and Quality
- Unit tests overview
- Error handling examples
- Input sanitization
- Memory management

### Slide 8: Challenges and Solutions
- Challenge: Thread synchronization
- Solution: Mutex usage
- Challenge: JSON parsing
- Solution: Custom parser design

### Slide 9: Lessons Learned
- Design iteration importance
- Testing early
- Documentation value
- Team collaboration

### Slide 10: Conclusion
- Project achievements
- Future improvements (if time)
- Questions

**Time Allocation:**
- Slides 1-3: 1 minute
- Slides 4-5: 1.5 minutes
- Slide 6 (Demo): 2 minutes
- Slides 7-9: 2 minutes
- Slide 10: 0.5 minutes
- Questions: 1 minute
- **Total: ~8 minutes**

