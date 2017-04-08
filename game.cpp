#include "game.h"
#include <mainwindow.h>

#include <QDebug>

namespace Qoolkie
{

constexpr char Game::ResourcesPath[];
constexpr char Game::highscores5FileName[];
constexpr char Game::highscores7FileName[];
constexpr std::array<TileContent, 7> Game::ContentsPot;

void Game::start(ColoursUsed colours)
{
    m_map.clearAllTiles();

    m_coloursInGame = colours;
    m_currentGain = static_cast<uint8_t>(m_coloursInGame);
    m_score = 0U;
    generateQoolkies();
}

void Game::generateQoolkies()
{
    if (!m_map.isAnyFreeTile())
    {
        return;
    }
    std::vector<std::pair<uint8_t, uint8_t>> freeTiles = m_map.getFreeTiles();
    std::map<std::pair<uint8_t, uint8_t>, TileContent> generatedTiles;

    static constexpr uint8_t newTilesNb {3U};
    for (uint8_t i = 0U; i < newTilesNb; ++i)
    {
        if (!m_map.isAnyFreeTile())
        {
            break;
        }
        uint8_t tileIdx = rand() % freeTiles.size();
        uint8_t contentIdx = rand() % static_cast<uint8_t>(m_coloursInGame);

        std::pair<uint8_t, uint8_t> tile = freeTiles.at(tileIdx);
        freeTiles.erase(freeTiles.begin() + tileIdx);

        TileContent content = ContentsPot[contentIdx];
        generatedTiles[tile] = content;

        m_map.setTileContent(tile.first, tile.second, content);
        emit qoolkieGenerated(tile.first - 1, tile.second - 1, content);
    }

    for (auto&& tile : generatedTiles)
    {
        auto ret = m_map.checkForScore(tile.first.first, tile.first.second, tile.second);
        if (ret.size() >= 5)
        {
            doScore(ret);
        }
    }
}

void Game::saveHighscore(const QString &userName) const
{
    m_highscore.save(m_coloursInGame == ColoursUsed::Five ? highscores5FileName : highscores7FileName,
                     userName.toStdString(), m_score);
}

QString Game::getHighscores(ColoursUsed coloursUsedInGame) const
{
    auto highscores = m_highscore.loadHighscores(coloursUsedInGame == ColoursUsed::Five ? highscores5FileName : highscores7FileName);

    QString scores;
    uint8_t counter {1U};
    for (auto&& score : highscores)
    {
        scores.append(QString::number(counter))
              .append(". ")
              .append(QString::fromStdString(score.first))
              .append(" \t")
              .append(QString::number(score.second)).append('\n');
        ++counter;
    }

    return scores;
}

void Game::preProcessNextTurn()
{
    generateQoolkies();
    if (!m_map.isAnyFreeTile())
    {
        emit gameOver();
    }
}

uint32_t Game::postProcessTurn(uint8_t destX, uint8_t destY)
{
    auto ret = m_map.checkForScore(destX, destY, m_map.getTileContent(destX, destY));
    if (ret.size() >= 5)
    {
        return doScore(ret);
    }
    return 0U;
}

uint32_t Game::doScore(std::vector<std::pair<uint8_t, uint8_t>> tiles)
{
    for (auto&& tile : tiles)
    {
        uint8_t x = tile.first;
        uint8_t y = tile.second;
        m_map.setTileContent(x, y, TileContent::None);
        emit tileCleared(x - 1, y - 1);
    }

    uint16_t gain = calculateGain(tiles.size());
    m_score += gain;
    emit scoreChanged(m_score);
    return gain;
}

uint16_t Game::calculateGain(size_t ballsInRow) const noexcept
{
    uint16_t gain = m_currentGain;
    if (ballsInRow == 6)
    {
        return gain * 2;
    }
    else if (ballsInRow == 7)
    {
        return gain * 3;
    }
    else if (ballsInRow > 7)
    {
        return gain * 4;
    }
    return gain;
}

void Game::moveQoolkie(uint8_t destX, uint8_t destY)
{
    TileContent content = m_map.getTileContent(m_ballXPos, m_ballYPos);

    m_map.setTileContent(m_ballXPos, m_ballYPos, TileContent::None);
    emit qoolkieGenerated(m_ballXPos - 1, m_ballYPos - 1, TileContent::None);

    m_map.setTileContent(destX, destY, content);
    emit qoolkieGenerated(destX - 1, destY - 1, content);

    uint32_t gain = postProcessTurn(destX, destY);
    if (gain == 0U)
    {
        preProcessNextTurn();
    }
}

void Game::tileClicked(uint8_t rowIdx, uint8_t colIdx)
{
    uint8_t x = rowIdx + 1;
    uint8_t y = colIdx + 1;
    if (m_map.isTileOccupied(x, y))
    {
        if (m_isBallClicked)
        {
            emit qoolkieGenerated(m_ballXPos - 1, m_ballYPos - 1, m_map.getTileContent(m_ballXPos, m_ballYPos));
        }
        m_isBallClicked = true;
        m_ballXPos = x;
        m_ballYPos = y;
        emit focusChanged(m_ballXPos - 1, m_ballYPos - 1, m_map.getTileContent(m_ballXPos, m_ballYPos));
    }
    else
    {
        if (m_isBallClicked && m_map.findPath(m_ballXPos, m_ballYPos, x, y))
        {
            moveQoolkie(x, y);
            m_isBallClicked = false;
        }
    }
}

QString Game::convertContentToString(TileContent content) noexcept
{
    switch (content)
    {
        case TileContent::Black:
            return "black";
        case TileContent::Blue:
            return "blue";
        case TileContent::Green:
            return "green";
        case TileContent::Pink:
            return "pink";
        case TileContent::Purple:
            return "purple";
        case TileContent::Red:
            return "red";
        case TileContent::Yellow:
            return "yellow";
        default:
            return "";
    }
}

}
