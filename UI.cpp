#include "UI.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#endif


UI::UI() {
    cursorRow = 0;
    cursorCol = 0;
    isSelecting = false;
    selectedPoint = Point(-1, -1);
}


void UI::clearScreen() const {
#ifdef _WIN32
    COORD coord;
    coord.X = 0;
    coord.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    std::cout << "\033[H";
#endif
}


void UI::printGemColored(GemType type, int r, int c) const {
    bool isCursor = (r == cursorRow && c == cursorCol);
    bool isSelected = (isSelecting && r == selectedPoint.row && c == selectedPoint.col);

    if (isCursor) std::cout << "\033[1;36m[\033[0m";
    else if (isSelected) std::cout << "\033[1;33m(\033[0m";
    else std::cout << " ";

    switch (type) {
        case RED:    std::cout << "\033[1;31mR\033[0m"; break;
        case BLUE:   std::cout << "\033[1;34mB\033[0m"; break;
        case GREEN:  std::cout << "\033[1;32mG\033[0m"; break;
        case YELLOW: std::cout << "\033[1;33mY\033[0m"; break;
        case PURPLE: std::cout << "\033[1;35mP\033[0m"; break;
        case EMPTY:  std::cout << " "; break;
    }

    if (isCursor) std::cout << "\033[1;36m]\033[0m";
    else if (isSelected) std::cout << "\033[1;33m)\033[0m";
    else std::cout << " ";
}


void UI::drawGame(const Game& game, const std::vector<std::string>& logTrace) {
    clearScreen();
    std::string pad(25, ' ');

    std::cout << "\n\n" << pad << "+--------------------------------------------------+\n";
    std::cout << pad << "|         GEM CASCADE: RECURSIVE EDITION           |\n";
    std::cout << pad << "+--------------------------------------------------+\n";

    std::cout << pad << "  SCORE: \033[1;32m" << std::setw(6) << game.getScore() << "\033[0m"
              << "  |  HIGH: \033[1;35m" << game.getHighScore() << "\033[0m"
              << "  |  TIME: \033[1;31m" << std::setw(3) << game.getTimeRemaining() << "s\033[0m\n";
    std::cout << pad << "----------------------------------------------------\n\n";

    std::cout << pad << "        0   1   2   3   4   5   6   7\n";
    std::cout << pad << "     +---+---+---+---+---+---+---+---+\n";

    const Board& board = game.getBoard();
    for (int r = 0; r < 8; ++r) {
        std::cout << pad << "   " << r << " |";
        for (int c = 0; c < 8; ++c) {
            printGemColored(board.getGem(r, c), r, c);
            if (c < 7) std::cout << "|";
        }
        std::cout << "|\n";
        if (r < 7) std::cout << pad << "     +---+---+---+---+---+---+---+---+\n";
    }
    std::cout << pad << "     +---+---+---+---+---+---+---+---+\n\n";

    std::cout << pad << "+------------------- CONTROLS ---------------------+\n";
    std::cout << pad << "|  Arrow Keys : Move Cursor                        |\n";
    std::cout << pad << "|  Enter      : Select & Swap Gem                  |\n";
    std::cout << pad << "+------------------- POWER-UPS --------------------+\n";
    std::cout << pad << "|  B : Bomb (120 pts)    |  R : Rocket (100 pts)   |\n";
    std::cout << pad << "|  H : Hint (70 pts)     |  S : Save Game          |\n";
    std::cout << pad << "|  Q : Quit to Menu                                |\n";
    std::cout << pad << "+--------------------------------------------------+\n";

    std::cout << "\n" << pad << "[ GAME ACTION LOG & CHAIN REACTIONS ]\n";
    if (logTrace.empty()) {
        std::cout << pad << "  * Board is stable. Waiting for your move...\n";
    } else {
        for (size_t i = 0; i < logTrace.size(); ++i) {
            std::cout << pad << "  > " << logTrace[i] << "\n";
        }
    }
    std::cout << pad << "====================================================\n";
}


void UI::runGameLoop(Game& game) {
    std::vector<std::string> logTrace;
    int tickCounter = 0;

#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif

    drawGame(game, logTrace);

    while (game.getTimeRemaining() > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        tickCounter += 50;

        if (tickCounter >= 1000) {
            game.decrementTime(1);
            tickCounter = 0;
            drawGame(game, logTrace);
        }

        if (_kbhit()) {
            int ch = _getch();
            if (ch == 0 || ch == 224) {
                int arrow = _getch();
                switch (arrow) {
                    case 72: if (cursorRow > 0) cursorRow--; break;
                    case 80: if (cursorRow < 7) cursorRow++; break;
                    case 75: if (cursorCol > 0) cursorCol--; break;
                    case 77: if (cursorCol < 7) cursorCol++; break;
                    default:
                        logTrace.clear();
                        logTrace.push_back("Warning: Invalid arrow extension key!");
                        break;
                }
            }
            else if (ch == 13) {
                if (!isSelecting) {
                    isSelecting = true;
                    selectedPoint = Point(cursorRow, cursorCol);
                    logTrace.clear();
                    logTrace.push_back("Selected (" + std::to_string(cursorRow) + "," + std::to_string(cursorCol) + "). Move cursor and press Enter.");
                } else {
                    Point targetPoint(cursorRow, cursorCol);
                    isSelecting = false;
                    logTrace.clear();

                    if (!game.playTurn(selectedPoint, targetPoint, logTrace, *this)) {
                        logTrace.push_back("Move not allowed! Swap cancelled.");
                    }
                }
            }
            else {
                char command = std::toupper(ch);
                if (command == 'Q') break;
                else if (command == 'S') {
                    if (game.saveGame("save/gamesave.txt")) {
                        logTrace.clear();
                        logTrace.push_back("Game saved successfully!");
                    } else {
                        logTrace.push_back("Error saving game files!");
                    }
                }
                else if (command == 'H') {
                    Point hintPt = game.useHint();
                    logTrace.clear();
                    if (hintPt.row != -1) logTrace.push_back("Hint: Look closely at row " + std::to_string(hintPt.row));
                    else logTrace.push_back("Failed: Low score or no moves available!");
                }
                else if (command == 'B') {
                    logTrace.clear();
                    Point target(cursorRow, cursorCol);
                    if (game.useBomb(target)) {
                        logTrace.push_back("Bomb exploded at (" + std::to_string(target.row) + "," + std::to_string(target.col) + ")! -120 pts.");
                        drawGame(game, logTrace);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    } else {
                        logTrace.push_back("Denied: You need at least 120 points for a Bomb!");
                    }
                }
                else if (command == 'R') {
                    logTrace.clear();
                    if (game.useRocket(cursorRow, true)) {
                        logTrace.push_back("Rocket launched! Row " + std::to_string(cursorRow) + " cleared. -100 pts.");
                        drawGame(game, logTrace);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    } else {
                        logTrace.push_back("Denied: You need at least 100 points for a Rocket!");
                    }
                }
                else {
                    logTrace.clear();
                    logTrace.push_back("Warning: Unknown key! Use Arrows, Enter, B, R, H, S, Q.");
                }
            }
            drawGame(game, logTrace);
        }
    }

    if (game.getTimeRemaining() <= 0) {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
        std::string pad(25, ' ');
        std::cout << "\n\n" << pad << "========================================\n";
        std::cout << pad << "    GAME OVER! TIME IS UP.              \n";
        std::cout << pad << "    FINAL SCORE: " << game.getScore() << "\n";
        std::cout << pad << "========================================\n";
        std::cout << pad << "Press any key to return to main menu...";
        _getch();
    }
}


void UI::showMainMenu() {
    Game game;
    int menuCursor = 1;
    std::string pad(32, ' ');

    while (true) {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
        std::cout << "\n\n\n" << pad << "+--------------------------------------+\n";
        std::cout << pad << "|        WELCOME TO GEM CASCADE        |\n";
        std::cout << pad << "+--------------------------------------+\n";
        std::cout << pad << "|                                      |\n";

        if (menuCursor == 1) std::cout << pad << "|            \033[1;46m > 1. NEW GAME < \033[0m           |\n";
        else std::cout << pad << "|                 1. NEW GAME          |\n";

        if (menuCursor == 2) std::cout << pad << "|            \033[1;46m > 2. LOAD LAST GAME < \033[0m     |\n";
        else std::cout << pad << "|                 2. LOAD LAST GAME    |\n";

        std::cout << pad << "|                                      |\n" << pad << "+--------------------------------------+\n";

        if (menuCursor == 3) std::cout << pad << "|            \033[1;41m > 3. EXIT GAME < \033[0m          |\n";
        else std::cout << pad << "|                 3. EXIT GAME         |\n";

        std::cout << pad << "+--------------------------------------+\n";
        std::cout << pad << "  Use Up/Down Arrows & ENTER to Select. \n";

        int ch = _getch();
        if (ch == 0 || ch == 224) {
            int arrow = _getch();
            if (arrow == 72) {
                if (menuCursor > 1) menuCursor--;
                else menuCursor = 3;
            }
            else if (arrow == 80) {
                if (menuCursor < 3) menuCursor++;
                else menuCursor = 1;
            }
        }
        else if (ch == 13) {
            if (menuCursor == 3) {
                std::cout << "\nGoodbye!\n";
                break;
            }
            else if (menuCursor == 1) {
                game.startNewGame();
                runGameLoop(game);
            }
            else if (menuCursor == 2) {
                if (game.loadGame("save/gamesave.txt")) {
                    runGameLoop(game);
                } else {
                    std::cout << "\nNo save file found! Press any key...";
                    _getch();
                }
            }
        }
    }
}
