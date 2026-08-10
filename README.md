Gem Cascade

A console-based Match-3 game developed in C++ as part of an Advanced Programming course.

Overview

Gem Cascade is a console-based Match-3 game inspired by classic tile-matching games.

The player swaps adjacent gems to create matching rows or columns. When a valid match is created, the matched gems are removed, the gems above them fall down, and empty cells are filled with new gems. These operations can trigger consecutive chain reactions (cascades).

The project focuses on object-oriented programming, game-state management, file handling, and a modular project structure with separate responsibilities for the game logic, board management, gems, and user interface.

Features

Console-based game interface

Main menu with:

New Game

Load Last Game

Exit


Keyboard-based navigation

Grid-based Match-3 gameplay

Gem selection and swapping

Match detection

Automatic removal of matched gems

Gravity and board refilling

Chain reactions (Cascades)

Score calculation based on the game's scoring rules

Save and load game state

Modular object-oriented structure

Separate classes for game logic, board management, gems, and user interface


Controls

Main Menu

Use the arrow keys to navigate through the menu options.

Game

Arrow Keys — Move around the game board

Space — Select a gem

Arrow Keys — Navigate to another cell

Space — Select the target gem and perform the swap


Additional controls are displayed in the game interface.

Project Structure

Match3-CPP/
├── include/
│   ├── Board.h
│   ├── Game.h
│   ├── Gem.h
│   └── UI.h
│
├── src/
│   ├── Board.cpp
│   ├── Game.cpp
│   ├── UI.cpp
│   └── main.cpp
│
├── .gitignore
└── README.md

Architecture

The project is divided into several classes, with each class responsible for a specific part of the application.

Board

Responsible for managing the game board and operations related to gems and board state.

Game

Acts as the main game controller and manages the game flow, turns, score, game state, saving and loading, and end-game conditions.

Gem

Represents individual gems on the game board and stores their relevant properties.

UI

Handles interaction with the player and console presentation, including the main menu, game board, score and game information, user input, and game messages.

This separation keeps the game logic independent from the console interface and makes the project easier to maintain and extend.

Technologies

C++

Object-Oriented Programming (OOP)

Standard C++ Library

File I/O

Console-based UI

Code::Blocks


How to Run

The project was developed using Code::Blocks.

1. Clone or download the repository.


2. Open the project in Code::Blocks.


3. Build the project.


4. Run the generated executable.



Save System

The game supports saving and loading the current game state.

The saved data contains the information required to restore the game and continue playing from the previous state.

Scoring

The scoring system is based on the chain of consecutive matches.

As chain reactions continue, the score multiplier increases, rewarding the player for creating consecutive cascades.

Project Context

This project was developed as a practical project for an Advanced Programming course at Iran University of Science and Technology.

The project was designed to apply concepts including object-oriented programming, algorithm design, data management, file handling, game-state management, and modular software design.

Future Improvements

Improve the console user interface

Add automated tests

Improve input handling

Further modularize the code

Add a graphical user interface

Expand the game's mechanics
