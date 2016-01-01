#include <QApplication>
#include <mainwindow.h>
#include <gamemap.h>
#include <game.h>
#include <cstdlib>
#include <ctime>

const int MAP_ROWS = 9;
const int MAP_COLS = 9;

int main(int argc, char **argv)
{
    std::srand(time(NULL));

    QApplication a(argc, argv);

    Qoolkie::GameMap gameMap(MAP_ROWS, MAP_COLS);
    Qoolkie::Game game(gameMap);
    MainWindow w(game);
    w.show();

    return a.exec();
}
