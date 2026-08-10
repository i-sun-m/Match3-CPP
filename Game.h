#ifndef GAME_H
#define GAME_H


#include "Board.h"
#include <string>
#include <vector>


class UI;

class Game {
private:
    Board board;
    int score;
    int highScore;
    int bombCount;
    int rocketCount;
    int hintCount;
    int timeRemaining;


public:
    Game();
    void startNewGame();
    bool loadGame(const std::string& filename);
    bool saveGame(const std::string& filename);

    bool playTurn(Point p1, Point p2, std::vector<std::string>& logTrace, UI& ui);

    bool useBomb(Point p);
    bool useRocket(int index, bool isRow);
    Point useHint();

    const Board& getBoard() const { return board; }
    int getScore() const { return score; }
    int getHighScore() const { return highScore; }
    int getBombCount() const { return bombCount; }
    int getRocketCount() const { return rocketCount; }
    int getHintCount() const { return hintCount; }
    int getTimeRemaining() const { return timeRemaining; }

    void decrementTime(int seconds) {
        timeRemaining -= seconds;
        if (timeRemaining < 0) timeRemaining = 0;
    }
};

#endif
