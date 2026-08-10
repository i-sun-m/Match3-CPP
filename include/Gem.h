#ifndef GEM_H
#define GEM_H

// Different types of gems on the board
enum GemType {
    EMPTY,
    RED,
    BLUE,
    GREEN,
    YELLOW,
    PURPLE
};

// Structure to keep row and column together
struct Point {
    int row;
    int col;

    Point() {
        row = 0;
        col = 0;
    }

    Point(int r, int c) {
        row = r;
        col = c;
    }


    // Check if the point is inside the 8x8 grid
    bool isValid() const {
        if (row >= 0 && row < 8 && col >= 0 && col < 8) {
            return true;
        }
        return false;
    }

    // Compare two points
    bool operator==(const Point& other) const {
        return (row == other.row && col == other.col);
    }
};

#endif
