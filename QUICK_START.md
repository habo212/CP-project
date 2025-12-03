# Quick Start Guide - Terminal Trivia Game

## How to Start the Game

### Step 1: Build the Project (if not already built)
```bash
cd build
cmake ..
make
```

### Step 2: Run the Game
```bash
./TerminalTriviaGame
```

Or with a custom questions file:
```bash
./TerminalTriviaGame ../data/questions.json
```

## How to Play

### Single Player Mode
1. Select a difficulty (Easy, Medium, Hard, or Mixed)
2. Choose "1. Single Player"
3. Answer questions by typing 1-4
4. View your final score and statistics

### Multiplayer Mode (Player vs Player)

**How to Start Multiplayer:**
1. Select a difficulty level
2. Choose number of players (2, 3, or 4)
3. Enter names for each player when prompted
4. Players take turns answering questions
5. After each question, current scores are displayed
6. At the end, see all players' stats and the winner!

**Example Multiplayer Flow:**
```
Player 1's turn → Question 1 → Score shown
Player 2's turn → Question 1 → Score shown
Player 1's turn → Question 2 → Score shown
Player 2's turn → Question 2 → Score shown
...
Final scores and winner announced!
```

## Game Controls

- **1-4**: Select answer choice
- **q**: Quit during a question
- **Enter**: Continue after each question/answer

## Tips

- Each question has a 30-second timer
- Faster correct answers earn bonus points
- Harder questions are worth more points
- In multiplayer, players alternate turns automatically

## Troubleshooting

**Game won't start?**
- Make sure you're in the `build/` directory
- Check that `data/questions.json` exists
- Try: `./TerminalTriviaGame ../data/questions.json`

**Can't see questions?**
- Ensure the questions file is properly formatted JSON
- Check file path is correct

Enjoy playing! 🎮

