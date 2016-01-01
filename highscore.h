#pragma once

#include <string>
#include <vector>
#include <QString>

namespace Qoolkie
{

class Highscore
{
private:
    QString filepath;

public:
    Highscore(std::string filename);

    void save(std::string name, long int score);
    std::vector<std::pair<std::string, long int>> loadHighscores();
};

}
