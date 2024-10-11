# DragonSlayer


## Overview

This is a C++ game simulation using **SFML** (Simple and Fast Multimedia Library). The simulation involves a **dragon**, a **slayer**, obstacles, and food items. The goal is to move the dragon around the game grid, avoid obstacles, and consume food while avoiding the slayer. The slayer will hunt the dragon based on its proximity, and if it catches up, the dragon will lose some of its length.

## Features

- **Dragon Movement**: The dragon can move in all four directions using the keyboard.
- **Slayer AI**: The slayer chases the dragon and can "cut" the dragon if it catches up.
- **Obstacles**: Randomly placed obstacles are scattered around the game grid.
- **Food**: Eating food increases the dragon's length.
- **Grid Display**: The game is displayed on a grid, where items like food and obstacles are visually represented.
- **Game End**: The game ends when the dragon's size is reduced to one segment or all the food is consumed.

## Game Controls

- **Up Arrow**: Move the dragon up.
- **Down Arrow**: Move the dragon down.
- **Left Arrow**: Move the dragon left.
- **Right Arrow**: Move the dragon right.
- **Escape**: Quit the game.

## How to Compile and Run

### Prerequisites
1. **SFML Library**:
   - Ensure you have **SFML** installed on your machine. You can get it [here](https://www.sfml-dev.org/).
   - On Linux, install SFML with:
     ```bash
     sudo apt-get install libsfml-dev
     ```
   - On macOS, you can install via Homebrew:
     ```bash
     brew install sfml
     ```
   - On Windows, follow the SFML setup instructions on their website.

### Compilation

1. Clone the repository to your local machine.
2. Run the following command in your terminal:

   ```bash
   g++ dragon_slayer.cpp -std=c++20 -lsfml-graphics -lsfml-window -lsfml-system -o dragonSlayer
    install_name_tool -add_rpath "/usr/local/lib/" dragonSlayer
   ./dragonSlayer


