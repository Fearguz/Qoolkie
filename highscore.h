#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <cstdint>
#include <string>
#include <vector>
#include <QString>

namespace Qoolkie
{

class Highscore
{
public:
    void save(std::string fileName, std::string userName, uint64_t score) const;
    std::vector<std::pair<std::string, uint64_t>> loadHighscores(std::string fileName) const;
};

}

#endif
