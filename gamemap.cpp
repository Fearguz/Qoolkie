#include "gamemap.h"

#include <queue>
#include <map>
#include <utility>
#include <stdexcept>

namespace Qoolkie
{

GameMap::GameMap(int _rows, int _cols) : rows(_rows + 2), cols(_cols + 2)
{
    tiles = new BallColour*[rows];
    for (int i = 0; i < rows; ++i)
    {
        tiles[i] = new BallColour[cols];
    }

    fillTiles();
}

GameMap::~GameMap()
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] tiles[i];
    }
    delete[] tiles;
}

void GameMap::clearTiles()
{
    fillTiles();
}

void GameMap::fillTiles()
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (i == 0)
                tiles[i][j] = WALL;
            else if (j == 0)
                tiles[i][j] = WALL;
            else if (i == (rows - 1))
                tiles[i][j] = WALL;
            else if (j == (cols - 1))
                tiles[i][j] = WALL;
            else
                tiles[i][j] = NONE;
        }
    }
}

void GameMap::setTileOccupation(int row, int col, GameMap::BallColour colour)
{
    tiles[row][col] = colour;
}

bool GameMap::isTileOccupied(int row, int col)
{
    return (tiles[row][col] != NONE);
}

GameMap::BallColour GameMap::getTileBallColour(int row, int col)
{
    return tiles[row][col];
}

std::vector<std::pair<int, int>> GameMap::getAllFreeTiles()
{
    std::vector<std::pair<int, int>> freeTiles;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (tiles[i][j] == NONE)
            {
                freeTiles.push_back(std::make_pair(i, j));
            }
        }
    }

    return freeTiles;
}

bool GameMap::isAnyTileStillFree()
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (tiles[i][j] == NONE)
                return true;
        }
    }

    return false;
}

bool GameMap::findPath(int from_row, int from_col, int dest_row, int dest_col)
{
    std::queue<std::pair<int, int>> set;
    if (tiles[from_row - 1][from_col] == NONE)
        set.push(std::make_pair(from_row - 1, from_col));
    if (tiles[from_row][from_col - 1] == NONE)
        set.push(std::make_pair(from_row, from_col - 1));
    if (tiles[from_row + 1][from_col] == NONE)
        set.push(std::make_pair(from_row + 1, from_col));
    if (tiles[from_row][from_col + 1] == NONE)
        set.push(std::make_pair(from_row, from_col + 1));

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols));
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (i == 0)
                visited[i][j] = true;
            else if (j == 0)
                visited[i][j] = true;
            else if (i == (rows - 1))
                visited[i][j] = true;
            else if (j == (cols - 1))
                visited[i][j] = true;
            else
                visited[i][j] = false;
        }
    }

    while (!set.empty())
    {
        auto elem = set.front();
        set.pop();

        int curr_row_idx = elem.first;
        int curr_col_idx = elem.second;
        visited[curr_row_idx][curr_col_idx] = true;

        if (curr_row_idx == dest_row && curr_col_idx == dest_col)
        {
            return true;
        }
        else
        {
            if (tiles[curr_row_idx - 1][curr_col_idx] == NONE && visited[curr_row_idx - 1][curr_col_idx] == false)
                set.push(std::make_pair(curr_row_idx - 1, curr_col_idx));
            if (tiles[curr_row_idx][curr_col_idx - 1] == NONE && visited[curr_row_idx][curr_col_idx - 1] == false)
                set.push(std::make_pair(curr_row_idx, curr_col_idx - 1));
            if (tiles[curr_row_idx + 1][curr_col_idx] == NONE && visited[curr_row_idx + 1][curr_col_idx] == false)
                set.push(std::make_pair(curr_row_idx + 1, curr_col_idx));
            if (tiles[curr_row_idx][curr_col_idx + 1] == NONE && visited[curr_row_idx][curr_col_idx + 1] == false)
                set.push(std::make_pair(curr_row_idx, curr_col_idx + 1));
        }
    }

    return false;
}

std::vector<std::pair<int, int>> GameMap::checkForScore(int ball_x, int ball_y, BallColour ballColour)
{
    std::vector<std::pair<int, int>> tilesToBeCleared_leftToRight;
    std::vector<std::pair<int, int>> tilesToBeCleared_leftDiagonal;
    std::vector<std::pair<int, int>> tilesToBeCleared_TopToBottom;
    std::vector<std::pair<int, int>> tilesToBeCleared_rightDiagonal;

    // Check Left to Right
    int curr_x = ball_x, curr_y = ball_y;
    bool goLeft = true, goRight = true;
    tilesToBeCleared_leftToRight.push_back(std::make_pair(curr_x, curr_y));
    while (goLeft)
    {
        if (tiles[curr_x][curr_y - 1] != ballColour)
        {
            goLeft = false;
        }
        else
        {
            tilesToBeCleared_leftToRight.push_back(std::make_pair(curr_x, curr_y - 1));
            curr_y -= 1;
        }
    }

    curr_x = ball_x, curr_y = ball_y;
    while (goRight)
    {
        if (tiles[curr_x][curr_y + 1] != ballColour)
        {
            goRight = false;
        }
        else
        {
            tilesToBeCleared_leftToRight.push_back(std::make_pair(curr_x, curr_y + 1));
            curr_y += 1;
        }
    }

    //Check Left Diagonal
    curr_x = ball_x, curr_y = ball_y;
    bool goLeftDiag = true, goRightDiag = true;
    tilesToBeCleared_leftDiagonal.push_back(std::make_pair(curr_x, curr_y));
    while (goLeftDiag)
    {
        if (tiles[curr_x - 1][curr_y - 1] != ballColour)
        {
            goLeftDiag = false;
        }
        else
        {
            tilesToBeCleared_leftDiagonal.push_back(std::make_pair(curr_x - 1, curr_y - 1));
            curr_x -= 1;
            curr_y -= 1;
        }
    }

    curr_x = ball_x, curr_y = ball_y;
    while (goRightDiag)
    {
        if (tiles[curr_x + 1][curr_y + 1] != ballColour)
        {
            goRightDiag = false;
        }
        else
        {
            tilesToBeCleared_leftDiagonal.push_back(std::make_pair(curr_x + 1, curr_y + 1));
            curr_x += 1;
            curr_y += 1;
        }
    }

    //Check Top to Bottom
    curr_x = ball_x, curr_y = ball_y;
    bool goTop = true, goBottom = true;
    tilesToBeCleared_TopToBottom.push_back(std::make_pair(curr_x, curr_y));
    while (goTop)
    {
        if (tiles[curr_x - 1][curr_y] != ballColour)
        {
            goTop = false;
        }
        else
        {
            tilesToBeCleared_TopToBottom.push_back(std::make_pair(curr_x - 1, curr_y));
            curr_x -= 1;
        }
    }

    curr_x = ball_x, curr_y = ball_y;
    while (goBottom)
    {
        if (tiles[curr_x + 1][curr_y] != ballColour)
        {
            goBottom = false;
        }
        else
        {
            tilesToBeCleared_TopToBottom.push_back(std::make_pair(curr_x + 1, curr_y));
            curr_x += 1;
        }
    }

    //Check Right Diagonal
    curr_x = ball_x, curr_y = ball_y;
    bool goLDiag = true, goRDiag = true;
    tilesToBeCleared_rightDiagonal.push_back(std::make_pair(curr_x, curr_y));
    while (goLDiag)
    {
        if (tiles[curr_x + 1][curr_y - 1] != ballColour)
        {
            goLDiag = false;
        }
        else
        {
            tilesToBeCleared_rightDiagonal.push_back(std::make_pair(curr_x + 1, curr_y - 1));
            curr_x += 1;
            curr_y -= 1;
        }
    }

    curr_x = ball_x, curr_y = ball_y;
    while (goRDiag)
    {
        if (tiles[curr_x - 1][curr_y + 1] != ballColour)
        {
            goRDiag = false;
        }
        else
        {
            tilesToBeCleared_rightDiagonal.push_back(std::make_pair(curr_x - 1, curr_y + 1));
            curr_x -= 1;
            curr_y += 1;
        }
    }

    std::map<size_t, std::vector<std::pair<int, int>>*> sizeVectorMap;
    sizeVectorMap[tilesToBeCleared_leftDiagonal.size()] = &tilesToBeCleared_leftDiagonal;
    sizeVectorMap[tilesToBeCleared_leftToRight.size()] = &tilesToBeCleared_leftToRight;
    sizeVectorMap[tilesToBeCleared_rightDiagonal.size()] = &tilesToBeCleared_rightDiagonal;
    sizeVectorMap[tilesToBeCleared_TopToBottom.size()] = &tilesToBeCleared_TopToBottom;

    std::vector<size_t> sizes { tilesToBeCleared_leftToRight.size(), tilesToBeCleared_leftDiagonal.size(),
                             tilesToBeCleared_rightDiagonal.size(), tilesToBeCleared_TopToBottom.size() };

    std::sort(sizes.begin(), sizes.end(), std::greater<size_t>());
    return *(sizeVectorMap.at(sizes.at(0)));
}

}
