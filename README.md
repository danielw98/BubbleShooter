# BubbleShooter

## Introduction

**BubbleShooter** is a captivating game with a unique twist—a rotating board and an endless number of levels for non-stop entertainment. This project leverages the SDL2 library for rendering, event handling, and audio playback, ensuring a pleasing gaming experience for the player.

## Demo Animation

![Bubble Shooter Animation](https://github.com/danielw98/BubbleShooter/blob/main/BubbleShooter.gif?raw=true)

## Features

- **Dynamic Rotating Board:** Test your aiming skills on a board that keeps turning, adding a layer of strategy to the classic bubble shooter gameplay.
- **Unlimited Levels:** The fun never ends with levels that increase in difficulty and complexity.
- **Game Customization:** Players can tweak game settings like the timer in the `config.txt` file for a personalized experience.
- **Sound Effects:** From shooting bubbles to achieving combos, every action is accompanied by satisfying sound effects, enhancing the game's immersion.
- **Score System:** Keep track of your progress with an in-game scoring system.

## Installation and Running

Ensure SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer are installed on your system. Clone the repository, compile using the provided Makefile, and run the game:

```bash
git clone https://github.com/danielw98/BubbleShooter.git
cd BubbleShooter
make
./BubbleShooter
```

## Gameplay Mechanics

- Aim and shoot bubbles to form groups of three or more of the same color to clear them from the board.
- Clearing combos causes all unconnected bubbles to fall, not just the matched ones, adding a satisfying twist to the gameplay. Your score increases based on the size of the combo, so try maximizing the number of bubbles you clear in a move.
- Progress through the levels by clearing all bubbles before time runs out, maximizing your score.
- **Spare Ball**: there is a spare ball in the left which you can toggle using SPACE key
- **Score Multiplier**: there is a multiplier bar so when you hit 5 consecutive combos in a row without missing, the score you get for the next combos will be 150% of the normal score, until missing or not making a combo, and the bar resets.

## Files and Architecture

The game's logic is spread across multiple files, with `Game.cpp` serving as the heart of the game, `LevelManager.cpp` for managing levels, and `MatchFinding.cpp` for handling bubble matching logic. Singleton classes like `TextureManager`, `SoundManager`, and `FontRenderer` are used for efficient resource management. 
This is only a brief overview of the project files, for a deeper dive, check the `src/` and `include/` folder structures and files.

## Contributing

Contributions are welcome! Whether it's adding new features, fixing bugs, or improving the code, feel free to fork the repository and submit a pull request.

## License

BubbleShooter is open-sourced under the MIT License. See the LICENSE file for more details.