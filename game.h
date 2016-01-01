#pragma once

#include <gamemap.h>
#include <highscore.h>

class MainWindow;

namespace Qoolkie
{

class Game
{
private:
    GameMap& map;
    MainWindow* mainWindow;
    Highscore highscores_5colors;
    Highscore highscores_7colors;

    bool isBallClicked;
    int ball_x;
    int ball_y;

    long int score;
    int gain;
    int coloursUsed;

    std::string imagesPath;
    std::vector<GameMap::BallColour> coloursPot {GameMap::BLACK, GameMap::BLUE, GameMap::GREEN, GameMap::PINK, GameMap::PURPLE,
                                                 GameMap::RED, GameMap::YELLOW};

public:
    Game(GameMap &_map);
    void setMainWindow(MainWindow *window);

    void start();
    void setColoursNumber(int number);
    void tileClicked(int row, int col);
    void showHighscores(int coloursUsed);

private:
    void preProcessNextTurn();
    int postProcessTurn(int dest_x, int dest_y);

    int doScore(std::vector<std::pair<int, int>> tiles);
    int calculateScore(size_t ballsInARow);

    void generateBalls();
    void focusOnBall(int row_idx, int col_idx, GameMap::BallColour colour);
    void generateBall(int row_idx, int col_idx, GameMap::BallColour colour);
    void moveBall(int dest_x, int dest_y);

    void clearTile(int row_idx, int col_idx);

    QString convertColourToString(GameMap::BallColour colour);
};

}
