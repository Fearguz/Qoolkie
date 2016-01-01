#include "game.h"
#include <mainwindow.h>

namespace Qoolkie
{

Game::Game(GameMap &_map) : map(_map), mainWindow(NULL), highscores_5colors("highscores_5.json"),
                            highscores_7colors("highscores_7.json"), isBallClicked(false),
                            ball_x(0), ball_y(0), score(0), gain(0), coloursUsed(5), imagesPath(":/images/")
{
}

void Game::setMainWindow(MainWindow *window)
{
    mainWindow = window;
    mainWindow->updateScore(score);
}

void Game::start()
{
    map.clearTiles();
    mainWindow->cleanTiles();
    score = 0;
    mainWindow->updateScore(score);

    generateBalls();
}

void Game::setColoursNumber(int number)
{
    gain = number;
    if (coloursUsed != number)
    {
        coloursUsed = number;
        start();
    }
}

void Game::generateBalls()
{
    std::vector<std::pair<int, int>> freeTiles = map.getAllFreeTiles();
    std::map<std::pair<int, int>, GameMap::BallColour> newTiles;

    const int ballsToBeGenerated = 3;
    for (int i = 0; i < ballsToBeGenerated; ++i)
    {
        if (freeTiles.empty())
        {
            break;
        }

        int idx = rand() % freeTiles.size();
        std::pair<int, int> tile = freeTiles.at(idx);
        freeTiles.erase(freeTiles.begin() + idx);

        int colour = rand() % coloursUsed;
        GameMap::BallColour ballColour = coloursPot.at(colour);
        map.setTileOccupation(tile.first, tile.second, ballColour);
        generateBall(tile.first - 1, tile.second - 1, ballColour);
        newTiles[tile] = ballColour;
    }

    for (auto it = newTiles.begin(); it != newTiles.end(); ++it)
    {
        auto pair = *it;

        std::vector<std::pair<int, int>> ret = map.checkForScore(pair.first.first, pair.first.second, pair.second);
        if (ret.size() >= 5)
        {
            doScore(ret);
        }
    }
}

void Game::showHighscores(int coloursUsed)
{
    Highscore* highscore = NULL;
    if (coloursUsed == 5)
        highscore = &highscores_5colors;
    else if (coloursUsed == 7)
        highscore = &highscores_7colors;
    else
        throw std::runtime_error("Something wrong happened here");

    std::vector<std::pair<std::string, long int>> highscores = highscore->loadHighscores();

    QString scores;
    size_t len = highscores.size();
    for (size_t i = 0; i < len; ++i)
    {
        scores.append(QString::number(i + 1) + ". " + QString::fromStdString(highscores.at(i).first) + " \t"
                      + QString::number(highscores.at(i).second) + "\n");
    }

    if (!scores.isEmpty())
        mainWindow->showMessageBox("Najlepsze wyniki", scores);
    else
        mainWindow->showMessageBox("Brak wyników", "");
}

void Game::preProcessNextTurn()
{
    generateBalls();

    if (!map.isAnyTileStillFree())
    {
        QString name = mainWindow->showInputBox("Zapisz wynik", "Przegrałeś. Podaj swoje imię i zapisz wynik.");
        if (coloursUsed == 5)
            highscores_5colors.save(name.toStdString(), score);
        else if (coloursUsed == 7)
            highscores_7colors.save(name.toStdString(), score);
    }
}

int Game::postProcessTurn(int dest_x, int dest_y)
{
    std::vector<std::pair<int, int>> ret = map.checkForScore(dest_x, dest_y, map.getTileBallColour(dest_x, dest_y));
    if (ret.size() >= 5)
    {
        return doScore(ret);
    }
    return 0;
}

int Game::doScore(std::vector<std::pair<int, int>> tiles)
{
    for (auto it = tiles.begin(); it != tiles.end(); ++it)
    {
        std::pair<int, int> tile = *it;
        int x = tile.first;
        int y = tile.second;

        map.setTileOccupation(x, y, GameMap::NONE);
        clearTile(x, y);
    }

    size_t size = tiles.size();
    int addition = calculateScore(size);
    score += addition;
    mainWindow->updateScore(score);
    return addition;
}

int Game::calculateScore(size_t ballsInARow)
{
    int actual_score = gain;
    if (ballsInARow == 6)
        actual_score *= 2;
    else if (ballsInARow == 7)
        actual_score *= 3;
    else if (ballsInARow > 7)
        actual_score *= 4;

    return actual_score;
}

void Game::moveBall(int dest_x, int dest_y)
{
    GameMap::BallColour colour = map.getTileBallColour(ball_x, ball_y);
    map.setTileOccupation(ball_x, ball_y, GameMap::NONE);
    generateBall(ball_x - 1, ball_y - 1, GameMap::NONE);
    map.setTileOccupation(dest_x, dest_y, colour);
    generateBall(dest_x - 1, dest_y - 1, colour);

    int scored = postProcessTurn(dest_x, dest_y);
    if (scored == 0)
        preProcessNextTurn();
}

void Game::tileClicked(int row, int col)
{
    int actual_row = row + 1;
    int actual_col = col + 1;

    if (map.isTileOccupied(actual_row, actual_col) == false)
    {
        if (isBallClicked && map.findPath(ball_x, ball_y, actual_row, actual_col))
        {
            moveBall(actual_row, actual_col);
            isBallClicked = false;
        }
    }
    else
    {
        if (isBallClicked)
        {
            generateBall(ball_x - 1, ball_y - 1, map.getTileBallColour(ball_x, ball_y));
        }

        isBallClicked = true;
        ball_x = actual_row;
        ball_y = actual_col;

        focusOnBall(ball_x - 1, ball_y - 1, map.getTileBallColour(ball_x, ball_y));
    }
}

void Game::generateBall(int row_idx, int col_idx, GameMap::BallColour colour)
{
    QString path = QString::fromStdString(imagesPath) + convertColourToString(colour) + ".png";
    mainWindow->setIconOnTile(path, row_idx, col_idx);
}

void Game::focusOnBall(int row_idx, int col_idx, GameMap::BallColour colour)
{
    QString path = QString::fromStdString(imagesPath) + convertColourToString(colour) + "_f.png";
    mainWindow->setIconOnTile(path, row_idx, col_idx);
}

void Game::clearTile(int row_idx, int col_idx)
{
    mainWindow->cleanTile(row_idx - 1, col_idx - 1);
}

QString Game::convertColourToString(GameMap::BallColour colour)
{
    switch (colour)
    {
        case GameMap::BLACK: return "black";
        case GameMap::BLUE: return "blue";
        case GameMap::GREEN: return "green";
        case GameMap::PINK: return "pink";
        case GameMap::PURPLE: return "purple";
        case GameMap::RED: return "red";
        case GameMap::YELLOW: return "yellow";
        case GameMap::WALL: return "wall";
        default: return "Unknown";
    }
}

}
