#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <cstdint>
#include <vector>

namespace Qoolkie
{

enum class TileContent : uint8_t
{
    Black,
    Blue,
    Green,
    Pink,
    Purple,
    Red,
    Yellow,
    Wall,
    None,
};

class GameMap
{
public:
    GameMap(uint8_t rows, uint8_t cols);
    GameMap(const GameMap&) = default;
    GameMap(GameMap&&) = default;
    GameMap& operator=(const GameMap&) = default;
    GameMap& operator=(GameMap&&) = default;
    ~GameMap() = default;

    uint8_t getRowsCount() const noexcept;
    uint8_t getColsCount() const noexcept;

    void clearAllTiles();

    bool isTileOccupied(uint8_t rowIdx, uint8_t colIdx) const;
    void setTileContent(uint8_t rowIdx, uint8_t colIdx, TileContent colour);
    TileContent getTileContent(uint8_t rowIdx, uint8_t colIdx) const;

    bool isAnyFreeTile() const noexcept;
    std::vector<std::pair<uint8_t, uint8_t>> getFreeTiles() const noexcept;

    bool findPath(uint8_t from_row, uint8_t from_col, uint8_t dest_row, uint8_t dest_col) const;
    std::vector<std::pair<uint8_t, uint8_t>> checkForScore(uint8_t ballXPos, uint8_t ballYPos, TileContent content) const;

private:
    using MapType = std::vector<std::vector<TileContent>>;

    uint8_t m_rows;
    uint8_t m_cols;
    MapType m_map;

    void defaultFillTiles() noexcept;
};

}

#endif
