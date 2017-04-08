#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QString>

#include <QDebug>

using namespace Qoolkie;

MainWindow::MainWindow(Qoolkie::Game& game, QWidget* parent) : QMainWindow(parent), m_game(game), m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    connect(m_ui->actionStart5, SIGNAL(triggered()), this, SLOT(startGameWith5Colors()));
    connect(m_ui->actionStart7, SIGNAL(triggered()), this, SLOT(startGameWith7Colors()));
    connect(m_ui->actionWyniki5, SIGNAL(triggered()), this, SLOT(showHighscoresFor5Colors()));
    connect(m_ui->actionWyniki7, SIGNAL(triggered()), this, SLOT(showHighscoresFor7Colors()));

    connect(&m_game, SIGNAL(qoolkieGenerated(uint8_t,uint8_t,Qoolkie::TileContent)), this, SLOT(onQoolkieGenerated(uint8_t,uint8_t,Qoolkie::TileContent)));
    connect(&m_game, SIGNAL(focusChanged(uint8_t,uint8_t,Qoolkie::TileContent)), this, SLOT(onFocusChanged(uint8_t,uint8_t,Qoolkie::TileContent)));
    connect(&m_game, SIGNAL(scoreChanged(uint32_t)), this, SLOT(onScoreChanged(uint32_t)));
    connect(&m_game, SIGNAL(tileCleared(uint8_t,uint8_t)), this, SLOT(onTileCleared(uint8_t,uint8_t)));
    connect(&m_game, SIGNAL(gameOver()), this, SLOT(onGameFinished()));

    initGameMap();
}

MainWindow::~MainWindow() noexcept
{
    delete m_ui;
}

void MainWindow::initGameMap()
{
    uint8_t rows = m_ui->gameMap->rowCount();
    uint8_t cols = m_ui->gameMap->columnCount();
    for (uint8_t i = 0U; i < rows; ++i)
    {
        for (uint8_t j = 0U; j < cols; ++j)
        {
            QLayoutItem* layoutItem = m_ui->gameMap->itemAtPosition(i, j);
            if (layoutItem != nullptr)
            {
                QWidget* widget = layoutItem->widget();
                if (widget != nullptr)
                {
                    QPushButton* tile = dynamic_cast<QPushButton*>(widget);
                    tile->setIconSize(QSize(TileSizePx, TileSizePx));
                    connect(tile, SIGNAL(clicked(bool)), this, SLOT(onTileClicked()));
                }
            }
        }
    }
}

void MainWindow::setTileIcon(QString imagePath, uint8_t rowIdx, uint8_t colIdx)
{
    QLayoutItem* layoutItem = m_ui->gameMap->itemAtPosition(rowIdx, colIdx);
    if (layoutItem != nullptr)
    {
        QWidget* widget = layoutItem->widget();
        if (widget != nullptr)
        {
            QPushButton* tile = dynamic_cast<QPushButton*>(widget);
            tile->setIcon(QIcon{QPixmap{imagePath}});
        }
    }
}

void MainWindow::cleanTile(uint8_t rowIdx, uint8_t colIdx)
{
    QLayoutItem* layoutItem = m_ui->gameMap->itemAtPosition(rowIdx, colIdx);
    if (layoutItem != nullptr)
    {
        QWidget* widget = layoutItem->widget();
        if (widget != nullptr)
        {
            QPushButton* tile = dynamic_cast<QPushButton*>(widget);
            tile->setIcon(QIcon());
        }
    }
}

void MainWindow::cleanTiles()
{
    uint8_t rows = m_ui->gameMap->rowCount();
    uint8_t cols = m_ui->gameMap->columnCount();
    for (uint8_t i = 0U; i < rows; ++i)
    {
        for (uint8_t j = 0U; j < cols; ++j)
        {
            cleanTile(i, j);
        }
    }
}

void MainWindow::onQoolkieGenerated(uint8_t x, uint8_t y, TileContent content)
{
    QString iconPath;
    iconPath.append(QString(Game::ResourcesPath));
    iconPath.append(Game::convertContentToString(content));
    iconPath.append(".png");

    setTileIcon(iconPath, x, y);
}

void MainWindow::onFocusChanged(uint8_t x, uint8_t y, TileContent content)
{
    QString iconPath;
    iconPath.append(QString(Game::ResourcesPath));
    iconPath.append(Game::convertContentToString(content));
    iconPath.append("_f.png");

    setTileIcon(iconPath, x, y);
}

void MainWindow::onScoreChanged(uint32_t score)
{
    updateScore(score);
}

void MainWindow::onTileCleared(uint8_t x, uint8_t y)
{
    cleanTile(x, y);
}

void MainWindow::onGameFinished()
{
    QString name = showInputBox("Zapisz wynik", "Przegrałeś. Podaj swoje imię i zapisz wynik.");
    m_game.saveHighscore(name);
}

void MainWindow::onTileClicked()
{
    QObject* object = sender();
    if (object != nullptr)
    {
        QPushButton* tile = dynamic_cast<QPushButton*>(object);
        if (tile != nullptr)
        {
            int rowIdx = -1;
            int colIdx = -1;
            int unused1 = -1;
            int unused2 = -1;
            int itemIdx = m_ui->gameMap->indexOf(tile);
            m_ui->gameMap->getItemPosition(itemIdx, &rowIdx, &colIdx, &unused1, &unused2);
            m_game.tileClicked(rowIdx, colIdx);
        }
    }
}

void MainWindow::startGameWith5Colors()
{
    cleanTiles();
    updateScore(0U);
    m_game.start(ColoursUsed::Five);
}

void MainWindow::startGameWith7Colors()
{
    cleanTiles();
    updateScore(0U);
    m_game.start(ColoursUsed::Seven);
}

void MainWindow::showHighscoresFor5Colors()
{
    QString highscores = m_game.getHighscores(ColoursUsed::Five);
    showMessageBox(highscores.isEmpty() ? "Brak wyników" : "Najlepsze wyniki", highscores);
}

void MainWindow::showHighscoresFor7Colors()
{
    QString highscores = m_game.getHighscores(ColoursUsed::Seven);
    showMessageBox(highscores.isEmpty() ? "Brak wyników" : "Najlepsze wyniki", highscores);
}

int MainWindow::showMessageBox(const QString& title, const QString& message)
{
    QFont font;
    font.setPixelSize(23);
    QMessageBox msgBox;
    msgBox.setText(title);
    msgBox.setInformativeText(message);
    msgBox.setFont(font);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    return msgBox.exec();
}

QString MainWindow::showInputBox(const QString& title, const QString& message)
{
    bool isOk = false;
    QInputDialog* dialog = new QInputDialog();
    QString input = dialog->getText(this, title, message, QLineEdit::Normal, "name", &isOk);
    if (!isOk || input.isEmpty())
    {
        return QString("");
    }
    return input;
}

void MainWindow::updateScore(uint32_t score)
{
    m_ui->score->setText(QString::number(score));
}
