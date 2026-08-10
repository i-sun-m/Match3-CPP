#include "Game.h"
#include "UI.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>


Game::Game() {
    score = 0;
    highScore = 50000;
    bombCount = 0;
    rocketCount = 0;
    hintCount = 0;
    timeRemaining = 180;
}


void Game::startNewGame() {
    score = 0;
    bombCount = 0;
    rocketCount = 0;
    hintCount = 0;
    timeRemaining = 180;
    board.initializeBoard();
}


bool Game::saveGame(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) return false;
    outFile << score << " " << bombCount << " " << rocketCount << " " << hintCount << " " << timeRemaining << "\n";
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            outFile << (int)board.getGem(r, c) << " ";
        }
        outFile << "\n";
    }
    outFile.close();
    return true;
}


bool Game::loadGame(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return false;
    inFile >> score >> bombCount >> rocketCount >> hintCount >> timeRemaining;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            int gemVal;
            inFile >> gemVal;
            board.setGem(r, c, (GemType)gemVal);
        }
    }
    inFile.close();
    if (score > highScore) highScore = score;
    return true;
}


bool Game::playTurn(Point p1, Point p2, std::vector<std::string>& logTrace, UI& ui) {
    if (!board.swapGems(p1, p2)) return false;

    logTrace.clear();
    logTrace.push_back("Swapped (" + std::to_string(p1.row) + "," + std::to_string(p1.col) + ") with (" + std::to_string(p2.row) + "," + std::to_string(p2.col) + ").");

    long long comboMultiplier = 1;
    bool matchFound = true;

    while (matchFound) {
        std::vector<std::vector<bool>> toRemove(8, std::vector<bool>(8, false));
        int gemsRemoved = 0;
        bool hasSpecialMatch = false;

        matchFound = board.findAndMarkMatches(toRemove, gemsRemoved, hasSpecialMatch);

        if (matchFound && gemsRemoved > 0) {
            long long turnScore = 10LL * comboMultiplier * gemsRemoved;
            score += turnScore;

            if (score > highScore) {
                highScore = score;
            }

            logTrace.push_back("Combo x" + std::to_string(comboMultiplier) + ": Cleared " + std::to_string(gemsRemoved) + " gems! +" + std::to_string(turnScore) + " pts.");

            board.removeGems(toRemove);
            ui.drawGame(*this, logTrace);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            board.applyGravityAndFill();
            ui.drawGame(*this, logTrace);

            comboMultiplier *= 2;
            if (comboMultiplier > 64) comboMultiplier = 64;
        } else {
            matchFound = false;
        }
    }

    if (!board.hasValidMoves()) {
        logTrace.push_back("Deadlock detected! Shuffling board...");
        board.shuffleBoard();
        ui.drawGame(*this, logTrace);
    }
    return true;
}


bool Game::useBomb(Point p) {
    if (score < 120) return false;
    score -= 120;

    std::vector<std::vector<bool>> toRemove(8, std::vector<bool>(8, false));
    for (int r = p.row - 1; r <= p.row + 1; ++r) {
        for (int c = p.col - 1; c <= p.col + 1; ++c) {
            if (r >= 0 && r < 8 && c >= 0 && c < 8) toRemove[r][c] = true;
        }
    }
    board.removeGems(toRemove);
    board.applyGravityAndFill();
    return true;
}


bool Game::useRocket(int index, bool isRow) {
    if (score < 100) return false;
    score -= 100;

    std::vector<std::vector<bool>> toRemove(8, std::vector<bool>(8, false));
    if (isRow) {
        for (int c = 0; c < 8; ++c) toRemove[index][c] = true;
    } else {
        for (int r = 0; r < 8; ++r) toRemove[r][index] = true;
    }
    board.removeGems(toRemove);
    board.applyGravityAndFill();
    return true;
}


Point Game::useHint() {
    if (score < 70) return Point(-1, -1);

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 7; ++c) {
            if (board.swapGems(Point(r, c), Point(r, c + 1))) {
                board.swapGems(Point(r, c), Point(r, c + 1));
                score -= 70;
                return Point(r, c);
            }
        }
    }
    for (int r = 0; r < 7; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (board.swapGems(Point(r, c), Point(r + 1, c))) {
                board.swapGems(Point(r, c), Point(r + 1, c));
                score -= 70;
                return Point(r, c);
            }
        }
    }
    return Point(-1, -1);
}
