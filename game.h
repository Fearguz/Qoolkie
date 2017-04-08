#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <array>
#include <QObject>
#include <QString>

#include "gamemap.h"
#include "highscore.h"

namespace Qoolkie
{

enum class ColoursUsed : uint8_t
{
    Five = 5U,
    Seven = 7U
};

class Game : public QObject
{
    Q_OBJECT

public:
    static constexpr char ResourcesPath[] = ":/images/";
    static QString convertContentToString(TileContent content) noexcept;

    void start(ColoursUsed colours);
    void saveHighscore(const QString& userName) const;
    QString getHighscores(ColoursUsed coloursUsedInGame) const;
    void tileClicked(uint8_t row, uint8_t col);

signals:
    void qoolkieGenerated(uint8_t x, uint8_t y, Qoolkie::TileContent content);
    void focusChanged(uint8_t x, uint8_t y, Qoolkie::TileContent content);
    void scoreChanged(uint32_t score);
    void tileCleared(uint8_t x, uint8_t y);
    void gameOver();

private:
    static constexpr uint8_t GameMapRows {9};
    static constexpr uint8_t GameMapCols {9};
    static constexpr char highscores5FileName[] = "highscores_5.json";
    static constexpr char highscores7FileName[] = "highscores_7.json";
    static constexpr std::array<TileContent, 7> ContentsPot { TileContent::Black, TileContent::Blue, TileContent::Green, TileContent::Pink,
                                                              TileContent::Red, TileContent::Yellow };

    void preProcessNextTurn();
    uint32_t postProcessTurn(uint8_t destX, uint8_t destY);

    void generateQoolkies();
    void moveQoolkie(uint8_t destX, uint8_t destY);

    uint32_t doScore(std::vector<std::pair<uint8_t, uint8_t>> tiles);
    uint16_t calculateGain(size_t ballsInRow) const noexcept;

    GameMap m_map {GameMapRows, GameMapCols};
    Highscore m_highscore;

    uint32_t m_score {0U};
    int32_t m_currentGain {0};
    uint8_t m_ballXPos {0U};
    uint8_t m_ballYPos {0U};
    ColoursUsed m_coloursInGame {ColoursUsed::Five};
    bool m_isBallClicked {false};
};

}

#endif
