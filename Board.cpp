#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <vector>


Board::Board() {
    grid.resize(SIZE, std::vector<GemType>(SIZE, EMPTY));
    std::srand(std::time(0));
    initializeBoard();
}


void Board::initializeBoard() {
    bool hasValid;
    do {
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                grid[r][c] = (GemType)((std::rand() % 5) + 1);
            }
        }

        std::vector<std::vector<bool>> toRemove(SIZE, std::vector<bool>(SIZE, false));
        int dummyGems = 0;
        bool dummySpecial = false;

        while (findAndMarkMatches(toRemove, dummyGems, dummySpecial)) {
            for (int r = 0; r < SIZE; ++r) {
                for (int c = 0; c < SIZE; ++c) {
                    if (toRemove[r][c]) {
                        grid[r][c] = (GemType)((std::rand() % 5) + 1);
                        toRemove[r][c] = false;
                    }
                }
            }
            dummyGems = 0;
            dummySpecial = false;
        }
        hasValid = hasValidMoves();
    } while (!hasValid);
}

GemType Board::getGem(int r, int c) const {
    if (r >= 0 && r < SIZE && c >= 0 && c < SIZE) return grid[r][c];
    return EMPTY;
}

void Board::setGem(int r, int c, GemType type) {
    if (r >= 0 && r < SIZE && c >= 0 && c < SIZE) grid[r][c] = type;
}

bool Board::swapGems(Point p1, Point p2) {
    if (!p1.isValid() || !p2.isValid()) return false;

    int diffRow = p1.row - p2.row;
    if (diffRow < 0) diffRow = -diffRow;
    int diffCol = p1.col - p2.col;
    if (diffCol < 0) diffCol = -diffCol;

    if (diffRow + diffCol != 1) return false;

    GemType temp = grid[p1.row][p1.col];
    grid[p1.row][p1.col] = grid[p2.row][p2.col];
    grid[p2.row][p2.col] = temp;

    std::vector<std::vector<bool>> toRemove(SIZE, std::vector<bool>(SIZE, false));
    int gemsRemoved = 0;
    bool dummySpecial = false;

    if (findAndMarkMatches(toRemove, gemsRemoved, dummySpecial)) {
        return true;
    }

    temp = grid[p1.row][p1.col];
    grid[p1.row][p1.col] = grid[p2.row][p2.col];
    grid[p2.row][p2.col] = temp;
    return false;
}

bool Board::findAndMarkMatches(std::vector<std::vector<bool>>& toRemove, int& gemsRemoved, bool& hasSpecialMatch) {
    std::vector<std::vector<bool>> visited(SIZE, std::vector<bool>(SIZE, false));
    bool totalMatchFound = false;
    gemsRemoved = 0;
    hasSpecialMatch = false;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (visited[r][c] || grid[r][c] == EMPTY) continue;

            GemType currentType = grid[r][c];
            std::vector<Point> cluster;
            std::vector<Point> queue;

            queue.push_back(Point(r, c));
            visited[r][c] = true;
            int head = 0;

            while (head < (int)queue.size()) {
                Point curr = queue[head++];
                cluster.push_back(curr);

                for (int i = 0; i < 4; ++i) {
                    int nr = curr.row + dr[i];
                    int nc = curr.col + dc[i];
                    Point neighbor(nr, nc);

                    if (neighbor.isValid() && !visited[nr][nc] && grid[nr][nc] != EMPTY && grid[nr][nc] == currentType) {
                        visited[nr][nc] = true;
                        queue.push_back(neighbor);
                    }
                }
            }

            if (cluster.size() >= 3) {
                totalMatchFound = true;
                for (size_t k = 0; k < cluster.size(); ++k) {
                    Point pt = cluster[k];
                    if (!toRemove[pt.row][pt.col]) {
                        toRemove[pt.row][pt.col] = true;
                        gemsRemoved++;
                    }
                }

                if (cluster.size() == 4) {
                    hasSpecialMatch = true;
                    for (int i = 0; i < SIZE; ++i) {
                        if (grid[r][i] != EMPTY && !toRemove[r][i]) { toRemove[r][i] = true; gemsRemoved++; }
                    }
                } else if (cluster.size() >= 5) {
                    hasSpecialMatch = true;
                    for (int i = 0; i < SIZE; ++i) {
                        for (int j = 0; j < SIZE; ++j) {
                            if (grid[i][j] == currentType && !toRemove[i][j]) {
                                toRemove[i][j] = true;
                                gemsRemoved++;
                            }
                        }
                    }
                }
            }
        }
    }
    return totalMatchFound;
}

void Board::removeGems(const std::vector<std::vector<bool>>& toRemove) {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (toRemove[r][c]) grid[r][c] = EMPTY;
        }
    }
}

bool Board::applyGravityAndFill() {
    bool shifted = false;
    for (int c = 0; c < SIZE; ++c) {
        int emptyRow = SIZE - 1;
        for (int r = SIZE - 1; r >= 0; --r) {
            if (grid[r][c] != EMPTY) {
                if (r != emptyRow) {
                    grid[emptyRow][c] = grid[r][c];
                    grid[r][c] = EMPTY;
                    shifted = true;
                }
                emptyRow--;
            }
        }
        for (int r = emptyRow; r >= 0; --r) {
            grid[r][c] = (GemType)((std::rand() % 5) + 1);
            shifted = true;
        }
    }
    return shifted;
}

bool Board::checkMatchesWithoutRemoving(std::vector<std::vector<GemType>>& tempGrid) {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE - 2; ++c) {
            if (tempGrid[r][c] != EMPTY && tempGrid[r][c] == tempGrid[r][c+1] && tempGrid[r][c] == tempGrid[r][c+2]) return true;
        }
    }
    for (int c = 0; c < SIZE; ++c) {
        for (int r = 0; r < SIZE - 2; ++r) {
            if (tempGrid[r][c] != EMPTY && tempGrid[r+1][c] == tempGrid[r][c] && tempGrid[r+2][c] == tempGrid[r][c]) return true;
        }
    }
    return false;
}

bool Board::hasValidMoves() {
    std::vector<std::vector<GemType>> tempGrid = grid;
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (c + 1 < SIZE) {
                GemType t = tempGrid[r][c];
                tempGrid[r][c] = tempGrid[r][c+1];
                tempGrid[r][c+1] = t;
                if (checkMatchesWithoutRemoving(tempGrid)) return true;
                t = tempGrid[r][c];
                tempGrid[r][c] = tempGrid[r][c+1];
                tempGrid[r][c+1] = t;
            }
            if (r + 1 < SIZE) {
                GemType t = tempGrid[r][c];
                tempGrid[r][c] = tempGrid[r+1][c];
                tempGrid[r+1][c] = t;
                if (checkMatchesWithoutRemoving(tempGrid)) return true;
                t = tempGrid[r][c];
                tempGrid[r][c] = tempGrid[r+1][c];
                tempGrid[r+1][c] = t;
            }
        }
    }
    return false;
}

void Board::shuffleBoard() {
    do {
        for (int r = 0; r < SIZE; ++r) {
            for (int c = 0; c < SIZE; ++c) {
                grid[r][c] = (GemType)((std::rand() % 5) + 1);
            }
        }
    } while (!hasValidMoves());
}
