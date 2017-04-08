#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Qoolkie::Game& game, QWidget *parent = nullptr);
    ~MainWindow() noexcept;

    void setTileIcon(QString imagePath, uint8_t rowIdx, uint8_t colIdx);
    void cleanTile(uint8_t rowIdx, uint8_t colIdx);
    void cleanTiles();

    QString showInputBox(const QString& title, const QString& message);
    int showMessageBox(const QString& title, const QString& message);

public slots:
    void onQoolkieGenerated(uint8_t x, uint8_t y, Qoolkie::TileContent content);
    void onFocusChanged(uint8_t x, uint8_t y, Qoolkie::TileContent content);
    void onScoreChanged(uint32_t score);
    void onTileCleared(uint8_t x, uint8_t y);
    void onGameFinished();

    void onTileClicked();

    void startGameWith5Colors();
    void startGameWith7Colors();
    void showHighscoresFor5Colors();
    void showHighscoresFor7Colors();

private:
    static constexpr uint8_t TileSizePx {60};

    Qoolkie::Game& m_game;
    Ui::MainWindow* m_ui;

    void initGameMap();
    void updateScore(uint32_t score);
};

#endif
