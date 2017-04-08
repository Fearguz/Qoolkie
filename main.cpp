#include <cstdlib>
#include <ctime>
#include <memory>
#include <QApplication>
#include <mainwindow.h>
#include <gamemap.h>
#include <game.h>

int main(int argc, char **argv)
{
    std::srand(std::time(nullptr));

    QApplication a{argc, argv};
    Qoolkie::Game game;
    MainWindow w {game};
    w.show();

    return a.exec();
}
