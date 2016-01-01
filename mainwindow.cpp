#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QString>

using namespace Qoolkie;

MainWindow::MainWindow(Qoolkie::Game &_controller, QWidget *parent) : QMainWindow(parent), controller(_controller), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    controller.setMainWindow(this);
    QObject::connect(ui->actionStart5, SIGNAL(triggered()), this, SLOT(start5Clicked()));
    QObject::connect(ui->actionStart7, SIGNAL(triggered()), this, SLOT(start7Clicked()));
    QObject::connect(ui->actionWyniki5, SIGNAL(triggered()), this, SLOT(highscores5Clicked()));
    QObject::connect(ui->actionWyniki7, SIGNAL(triggered()), this, SLOT(highscores7Clicked()));

    tilesInit();
}

void MainWindow::tilesInit()
{
    int rowCnt = ui->plansza->rowCount(), colCnt = ui->plansza->columnCount();
    for (int i = 0; i < rowCnt; ++i)
    {
        for (int j = 0; j < colCnt; ++j)
        {
            QLayoutItem* item = ui->plansza->itemAtPosition(i, j);
            if (item != NULL)
            {
                QPushButton* button = dynamic_cast<QPushButton*>(item->widget());
                button->setIconSize(QSize(60, 60));
                QObject::connect(button, SIGNAL(clicked()), this, SLOT(tileClicked()));
            }
        }
    }
}

void MainWindow::updateScore(unsigned int score)
{
    ui->score->setText(QString::number(score));
}

void MainWindow::setIconOnTile(QString imagePath, int row, int col)
{
    QPixmap image(imagePath);
    QIcon icon(image);

    QLayoutItem* item = ui->plansza->itemAtPosition(row, col);
    if (item != NULL)
    {
        QPushButton* button = dynamic_cast<QPushButton*>(item->widget());
        button->setIcon(icon);
    }
}

void MainWindow::cleanTiles()
{
    int rowCnt = ui->plansza->rowCount(), colCnt = ui->plansza->columnCount();
    for (int i = 0; i < rowCnt; ++i)
    {
        for (int j = 0; j < colCnt; ++j)
        {
            cleanTile(i, j);
        }
    }
}

void MainWindow::cleanTile(int x, int y)
{
    QLayoutItem* item = ui->plansza->itemAtPosition(x, y);
    if (item != NULL)
    {
        QPushButton* button = dynamic_cast<QPushButton*>(item->widget());
        button->setIcon(QIcon());
    }
}

void MainWindow::tileClicked()
{
    QObject *button = sender();
    QWidget *widget = dynamic_cast<QWidget*>(button);
    if (widget == NULL)
        return;

    int row = -1, col = -1, unused_1 = 0, unused_2 = 0;
    int idx = ui->plansza->indexOf(widget);
    ui->plansza->getItemPosition(idx, &row, &col, &unused_1, &unused_2);

    controller.tileClicked(row, col);
}

void MainWindow::start5Clicked()
{
    controller.setColoursNumber(5);
    controller.start();
}

void MainWindow::start7Clicked()
{
    controller.setColoursNumber(7);
    controller.start();
}

void MainWindow::highscores5Clicked()
{
    controller.showHighscores(5);
}

void MainWindow::highscores7Clicked()
{
    controller.showHighscores(7);
}

int MainWindow::showMessageBox(QString title, QString message)
{
    QFont font;
    font.setPixelSize(23);

    QMessageBox msgBox;
    msgBox.setText(title);
    msgBox.setInformativeText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setFont(font);
    return msgBox.exec();
}

QString MainWindow::showInputBox(QString title, QString textToShow)
{
    bool ok;
    QInputDialog* dialog = new QInputDialog();
    QString text = dialog->getText(this, title, textToShow, QLineEdit::Normal, QDir::home().dirName(), &ok);

    if (ok && !text.isEmpty())
        return text;
    else
        return QString("");
}

MainWindow::~MainWindow()
{
    delete ui;
}
