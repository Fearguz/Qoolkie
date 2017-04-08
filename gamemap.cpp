#include "gamemap.h"
#include <queue>
#include <map>
#include <algorithm>

namespace Qoolkie
{

GameMap::GameMap(uint8_t rows, uint8_t cols) : m_rows(rows + 2), m_cols(cols + 2),
                                               m_map(m_rows, std::vector<TileContent>(m_cols, TileContent::None))
{
    defaultFillTiles();
}

uint8_t GameMap::getRowsCount() const noexcept
{
    return m_rows - 2;
}

uint8_t GameMap::getColsCount() const noexcept
{
    return m_cols - 2;
}

void GameMap::clearAllTiles()
{
    defaultFillTiles();
}

void GameMap::defaultFillTiles() noexcept
{
    for (uint8_t i = 0U; i < m_rows; ++i)
    {
        for (uint8_t j = 0U; j < m_cols; ++j)
        {
            if (i == 0U)
            {
                m_map[i][j] = TileContent::Wall;
            }
            else if (j == 0U)
            {
                m_map[i][j] = TileContent::Wall;
            }
            else if (i == (m_rows - 1))
            {
                m_map[i][j] = TileContent::Wall;
            }
            else if (j == (m_cols - 1))
            {
                m_map[i][j] = TileContent::Wall;
            }
            else
            {
                m_map[i][j] = TileContent::None;
            }
        }
    }
}

void GameMap::setTileContent(uint8_t rowIdx, uint8_t colIdx, TileContent content)
{
    m_map[rowIdx][colIdx] = content;
}

TileContent GameMap::getTileContent(uint8_t rowIdx, uint8_t colIdx) const
{
    return m_map[rowIdx][colIdx];
}

bool GameMap::isTileOccupied(uint8_t rowIdx, uint8_t colIdx) const
{
    TileContent currentTileContent = m_map[rowIdx][colIdx];
    return (currentTileContent != TileContent::None);
}

std::vector<std::pair<uint8_t, uint8_t>> GameMap::getFreeTiles() const noexcept
{
    std::vector<std::pair<uint8_t, uint8_t>> freeTiles;
    for (uint8_t i = 0U; i < m_rows; ++i)
    {
        for (uint8_t j = 0U; j < m_cols; ++j)
        {
            if (m_map[i][j] == TileContent::None)
            {
                freeTiles.push_back(std::make_pair(i, j));
            }
        }
    }
    return freeTiles;
}

bool GameMap::isAnyFreeTile() const noexcept
{
    for (uint8_t i = 0U; i < m_rows; ++i)
    {
        for (uint8_t j = 0U; j < m_cols; ++j)
        {
            if (m_map[i][j] == TileContent::None)
            {
                return true;
            }
        }
    }
    return false;
}

bool GameMap::findPath(uint8_t fromRow, uint8_t fromCol, uint8_t destRow, uint8_t destCol) const
{
    std::queue<std::pair<uint8_t, uint8_t>> set;
    if (m_map[fromRow - 1][fromCol] == TileContent::None)
        set.push(std::make_pair(fromRow - 1, fromCol));
    if (m_map[fromRow][fromCol - 1] == TileContent::None)
        set.push(std::make_pair(fromRow, fromCol - 1));
    if (m_map[fromRow + 1][fromCol] == TileContent::None)
        set.push(std::make_pair(fromRow + 1, fromCol));
    if (m_map[fromRow][fromCol + 1] == TileContent::None)
        set.push(std::make_pair(fromRow, fromCol + 1));

    std::vector<std::vector<bool>> visited(m_rows, std::vector<bool>(m_cols));
    for (uint8_t i = 0U; i < m_rows; ++i)
    {
        for (uint8_t j = 0U; j < m_cols; ++j)
        {
            if (i == 0U)
                visited[i][j] = true;
            else if (j == 0U)
                visited[i][j] = true;
            else if (i == (m_rows - 1))
                visited[i][j] = true;
            else if (j == (m_cols - 1))
                visited[i][j] = true;
            else
                visited[i][j] = false;
        }
    }

    while (!set.empty())
    {
        auto elem = set.front();
        set.pop();

        uint8_t currentRowIdx = elem.first;
        uint8_t currentColIdx = elem.second;
        visited[currentRowIdx][currentColIdx] = true;

        if (currentRowIdx == destRow && currentColIdx == destCol)
        {
            return true;
        }
        else
        {
            if (m_map[currentRowIdx - 1][currentColIdx] == TileContent::None && visited[currentRowIdx - 1][currentColIdx] == false)
                set.push(std::make_pair(currentRowIdx - 1, currentColIdx));
            if (m_map[currentRowIdx][currentColIdx - 1] == TileContent::None && visited[currentRowIdx][currentColIdx - 1] == false)
                set.push(std::make_pair(currentRowIdx, currentColIdx - 1));
            if (m_map[currentRowIdx + 1][currentColIdx] == TileContent::None && visited[currentRowIdx + 1][currentColIdx] == false)
                set.push(std::make_pair(currentRowIdx + 1, currentColIdx));
            if (m_map[currentRowIdx][currentColIdx + 1] == TileContent::None && visited[currentRowIdx][currentColIdx + 1] == false)
                set.push(std::make_pair(currentRowIdx, currentColIdx + 1));
        }
    }
    return false;
}

std::vector<std::pair<uint8_t, uint8_t>> GameMap::checkForScore(uint8_t ballXPos, uint8_t ballYPos, TileContent content) const
{
    std::vector<std::pair<uint8_t, uint8_t>> tilesToBeCleared_leftToRight;
    std::vector<std::pair<uint8_t, uint8_t>> tilesToBeCleared_leftDiagonal;
    std::vector<std::pair<uint8_t, uint8_t>> tilesToBeCleared_TopToBottom;
    std::vector<std::pair<uint8_t, uint8_t>> tilesToBeCleared_rightDiagonal;

    // Check Left to Right
    int curr_x = ballXPos, curr_y = ballYPos;
    bool goLeft = true, goRight = true;
    tilesToBeCleared_leftToRight.push_back(std::make_pair(curr_x, curr_y));
    while (goLeft)
    {
        if (m_map[curr_x][curr_y - 1] != content)
        {
            goLeft = false;
        }
        else
        {
            tilesToBeCleared_leftToRight.push_back(std::make_pair(curr_x, curr_y - 1));
            curr_y -= 1;
        }
    }

    curr_x = ballXPos, curr_y = ballYPos;
    while (goRight)
    {
        if (m_map[curr_x][curr_y + 1] != content)
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
    curr_x = ballXPos, curr_y = ballYPos;
    bool goLeftDiag = true, goRightDiag = true;
    tilesToBeCleared_leftDiagonal.push_back(std::make_pair(curr_x, curr_y));
    while (goLeftDiag)
    {
        if (m_map[curr_x - 1][curr_y - 1] != content)
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

    curr_x = ballXPos, curr_y = ballYPos;
    while (goRightDiag)
    {
        if (m_map[curr_x + 1][curr_y + 1] != content)
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
    curr_x = ballXPos, curr_y = ballYPos;
    bool goTop = true, goBottom = true;
    tilesToBeCleared_TopToBottom.push_back(std::make_pair(curr_x, curr_y));
    while (goTop)
    {
        if (m_map[curr_x - 1][curr_y] != content)
        {
            goTop = false;
        }
        else
        {
            tilesToBeCleared_TopToBottom.push_back(std::make_pair(curr_x - 1, curr_y));
            curr_x -= 1;
        }
    }

    curr_x = ballXPos, curr_y = ballYPos;
    while (goBottom)
    {
        if (m_map[curr_x + 1][curr_y] != content)
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
    curr_x = ballXPos, curr_y = ballYPos;
    bool goLDiag = true, goRDiag = true;
    tilesToBeCleared_rightDiagonal.push_back(std::make_pair(curr_x, curr_y));
    while (goLDiag)
    {
        if (m_map[curr_x + 1][curr_y - 1] != content)
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

    curr_x = ballXPos, curr_y = ballYPos;
    while (goRDiag)
    {
        if (m_map[curr_x - 1][curr_y + 1] != content)
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

    std::map<size_t, std::vector<std::pair<uint8_t, uint8_t>>*> sizeVectorMap;
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
