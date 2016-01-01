#pragma once

#include <QMainWindow>
#include <game.h>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Qoolkie::Game &_controller, QWidget *parent = 0);
    ~MainWindow();

    void updateScore(unsigned int score);
    void setIconOnTile(QString imagePath, int row, int col);
    void cleanTiles();
    void cleanTile(int x, int y);

    QString showInputBox(QString title, QString text);
    int showMessageBox(QString title, QString message);

public slots:
    void start5Clicked();
    void start7Clicked();
    void highscores5Clicked();
    void highscores7Clicked();
    void tileClicked();

private:
    Qoolkie::Game &controller;
    Ui::MainWindow *ui;
    int focusedTile_x;
    int focusedTile_y;

    void tilesInit();
};
