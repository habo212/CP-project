# Terminal Trivia Game

A terminal-based trivia game written in C, featuring question loading from JSON files, timer functionality using threads, and comprehensive error handling.

## Features

- 🎮 Interactive terminal-based gameplay
- 📚 Load questions from JSON files
- ⏱️ Timer functionality using pthreads
- 🎯 Multiple difficulty levels (Easy, Medium, Hard)
- 📊 Score tracking and statistics
- ✅ Input sanitization and error handling
- 🧪 Unit tests with CTest
- 🔧 CMake build system

## Project Structure

```
.
├── CMakeLists.txt          # CMake build configuration
├── README.md               # This file
├── REPORT.md               # Project report
├── src/                    # Source code
│   ├── main.c             # Main entry point
│   ├── game.c/.h          # Game logic and state management
│   ├── questions.c/.h     # Question loading and management
│   ├── utils.c/.h         # Utility functions
│   └── timer.c/.h         # Timer thread implementation
├── tests/                  # Unit tests
│   ├── test_main.c        # Test runner
│   ├── test_utils.c       # Utils tests
│   └── test_questions.c   # Questions tests
└── data/                   # Data files
    └── questions.json     # Sample questions file
```

## Prerequisites

- CMake (version 3.10 or higher)
- GCC compiler with C11 support
- pthread library (usually included with GCC)
- Make

## Building the Project

### On macOS/Linux:

1. Clone or download this repository

2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure the project with CMake:
```bash
cmake ..
```

4. Build the project:
```bash
make
```

5. (Optional) Build with tests:
```bash
cmake -DBUILD_TESTS=ON ..
make
```

### Build Options

- `BUILD_TESTS`: Enable test suite (default: ON)
  ```bash
  cmake -DBUILD_TESTS=OFF ..
  ```

## Running the Game

### Basic Usage

From the build directory:
```bash
./TerminalTriviaGame
```

Or specify a custom questions file:
```bash
./TerminalTriviaGame ../data/questions.json
```

### Gameplay

1. Select a difficulty level from the main menu:
   - Easy
   - Medium
   - Hard
   - Mixed Difficulty

2. Choose the number of players:
   - Single Player (1 player)
   - Two Players (2 players)
   - Three Players (3 players)
   - Four Players (4 players)

3. If playing multiplayer, enter names for each player

4. Players take turns answering questions (in multiplayer mode)

5. Answer questions by entering the number (1-4) corresponding to your choice

6. Each question has a 30-second time limit (if timer is enabled)

7. View statistics at the end of the game:
   - Single player: Your personal stats
   - Multiplayer: All players' scores and the winner

### Controls

- Enter a number (1-4) to select an answer
- Press 'q' to quit during a question
- Press Enter to continue after each question

## Running Tests

From the build directory:
```bash
# Run all tests
ctest

# Or run the test executable directly
./test_TerminalTriviaGame all

# Run specific test suites
./test_TerminalTriviaGame utils
./test_TerminalTriviaGame questions
```

## Questions File Format

The questions file should be in JSON format. Example:

```json
[
  {
    "question": "What is the capital of France?",
    "options": ["London", "Berlin", "Paris", "Madrid"],
    "correct": 2,
    "difficulty": "easy"
  }
]
```

- `question`: The question text (string)
- `options`: Array of 4 answer options (strings)
- `correct`: Index of correct answer (0-3, integer)
- `difficulty`: "easy", "medium", or "hard" (string)

See `data/questions.json` for a complete example.

## Technical Details

### Higher-Level C Constructs Used

1. **Threads (pthreads)**: Timer functionality uses a separate thread to count down for each question
2. **Structs**: Question, QuestionBank, GameState, GameConfig, GameStats, Timer
3. **Enums**: Difficulty (EASY, MEDIUM, HARD), Category, UtilsError
4. **Dynamic Memory Allocation**: QuestionBank uses realloc for dynamic growth
5. **Preprocessor Macros**: Configuration constants (MAX_INPUT_LEN, MAX_QUESTION_LEN, etc.)

### Design Principles

- **Modular Architecture**: Clear separation of concerns across multiple files
- **Error Handling**: Comprehensive error checking and meaningful error messages
- **Input Sanitization**: All user input is validated and sanitized
- **Memory Management**: Proper allocation and deallocation with cleanup functions
- **Thread Safety**: Mutex-protected timer operations

## Troubleshooting

### Build Issues

**Problem**: CMake not found
- **Solution**: Install CMake: `brew install cmake` (macOS) or `sudo apt-get install cmake` (Linux)

**Problem**: pthread not found
- **Solution**: pthread is usually included with GCC. Ensure you have a complete C development environment.

**Problem**: Compilation errors
- **Solution**: Ensure you're using GCC with C11 support. Check with `gcc --version`

### Runtime Issues

**Problem**: Questions file not found
- **Solution**: Ensure `data/questions.json` exists, or provide the correct path as a command-line argument

**Problem**: Timer not working
- **Solution**: Ensure pthread library is properly linked. Check CMake configuration.

## Contributing

This is a course project. For improvements or bug fixes:

1. Create a new branch
2. Make your changes
3. Test thoroughly
4. Submit a pull request

## License

This project is created for educational purposes as part of a C programming course.

## Authors

[Your Name/Team Name]

## Acknowledgments

- Course instructor for project guidelines
- C programming community for best practices
