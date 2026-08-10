#ifndef BOARD_H
#define BOARD_H


#include <vector>
#include "Gem.h"


class Board {
private:
    std::vector<std::vector<GemType>> grid;
    const int SIZE = 8;
    bool checkMatchesWithoutRemoving(std::vector<std::vector<GemType>>& tempGrid);

public:
    Board();
    void initializeBoard();
    GemType getGem(int r, int c) const;
    void setGem(int r, int c, GemType type);
    bool swapGems(Point p1, Point p2);
    bool hasValidMoves();
    void shuffleBoard();

    bool findAndMarkMatches(std::vector<std::vector<bool>>& toRemove, int& gemsRemoved, bool& hasSpecialMatch);
    void removeGems(const std::vector<std::vector<bool>>& toRemove);
    bool applyGravityAndFill();
};

#endif
