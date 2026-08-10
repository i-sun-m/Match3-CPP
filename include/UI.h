#ifndef UI_H
#define UI_H

#include "Game.h"
#include <vector>
#include <string>



class UI {
private:
    // Variables for managing cursor position and selection
    int cursorRow;
    int cursorCol;
    bool isSelecting;
    Point selectedPoint;

    // Internal display helpers
    void clearScreen() const;
    void printGemColored(GemType type, int r, int c) const;


public:
    UI();

    void showMainMenu();
    void drawGame(const Game& game, const std::vector<std::string>& logTrace);
    void runGameLoop(Game& game);
};


#endif
