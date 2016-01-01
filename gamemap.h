#pragma once

#include <vector>
#include <QString>

namespace Qoolkie
{

class GameMap
{
public:
    enum BallColour {
        BLACK = 0,
        BLUE,
        GREEN,
        PINK,
        PURPLE,
        RED,
        YELLOW,
        NONE,
        WALL
    };

private:
    int rows;
    int cols;
    BallColour** tiles;

public:
    GameMap(int _rows, int _cols);
    ~GameMap();

    int getRowsCount() { return rows - 2; }
    int getColsCount() { return cols - 2; }

    std::vector<std::pair<int, int>> getAllFreeTiles();
    bool isAnyTileStillFree();

    void clearTiles();

    bool isTileOccupied(int row, int col);
    void setTileOccupation(int row, int col, BallColour colour);
    BallColour getTileBallColour(int row, int col);

    bool findPath(int from_row, int from_col, int dest_row, int dest_col);
    std::vector<std::pair<int, int>> checkForScore(int ball_x, int ball_y, BallColour colour);

private:
    void fillTiles();
};

}
