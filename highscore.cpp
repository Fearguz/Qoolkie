#include "highscore.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>

namespace Qoolkie
{

void Highscore::save(std::string fileName, std::string userName, uint64_t score) const
{
    QString filepath = QDir::homePath() + QDir::separator() + QString::fromStdString(fileName);
    QFile loadFile(filepath);
    if (loadFile.exists())
    {
        if (!loadFile.open(QIODevice::ReadWrite))
        {
            throw std::runtime_error("Could not open JSON file");
        }

        QByteArray data = loadFile.readAll();
        QJsonDocument document(QJsonDocument::fromJson(data));

        QJsonObject docObject = document.object();
        QJsonArray highscores = docObject["highscores"].toArray();

        QJsonObject newHighscore;
        newHighscore["name"] = QString::fromStdString(userName);
        newHighscore["score"] = QString::number(score);
        highscores.push_back(newHighscore);

        loadFile.remove();

        QFile saveFile(filepath);
        if (!saveFile.open(QIODevice::WriteOnly))
            throw std::runtime_error("Could not open file for writing");

        QJsonObject object;
        object["highscores"] = highscores;

        QJsonDocument newDocument(object);
        saveFile.write(newDocument.toJson());
    }
    else
    {
        QFile saveFile(filepath);
        if (!saveFile.open(QIODevice::ReadWrite))
        {
            throw std::runtime_error("Could not open file for writing");
        }

        QJsonObject json, highscore;
        QJsonArray highscores;

        highscore["name"] = QString::fromStdString(userName);
        highscore["score"] = QString::number(score);
        highscores.append(highscore);
        json["highscores"] = highscores;

        QJsonDocument document(json);
        saveFile.write(document.toJson());
    }
}

struct less_than_score
{
    inline bool operator()(const std::pair<std::string, uint64_t>& left, const std::pair<std::string, uint64_t>& right)
    {
        return (left.second > right.second);
    }
};

std::vector<std::pair<std::string, uint64_t>> Highscore::loadHighscores(std::string fileName) const
{
    QString filepath = QDir::homePath() + QDir::separator() + QString::fromStdString(fileName);
    std::vector<std::pair<std::string, uint64_t>> highscores;

    QFile loadFile(filepath);
    if (!loadFile.exists())
    {
        return highscores;
    }
    if (!loadFile.open(QIODevice::ReadWrite))
    {
        throw std::runtime_error("Could not open JSON file");
    }

    QByteArray data = loadFile.readAll();
    QJsonDocument document(QJsonDocument::fromJson(data));

    QJsonObject docObject = document.object();
    QJsonArray highscoresArray = docObject["highscores"].toArray();

    int arrSize = highscoresArray.size();
    for (int highscoreIdx = 0; highscoreIdx < arrSize; ++highscoreIdx)
    {
        QJsonValue value = highscoresArray.at(highscoreIdx);
        QJsonObject obj = value.toObject();
        QString name = obj["name"].toString();
        QString scoreStr = obj["score"].toString();
        long int score = scoreStr.toLong();

        std::pair<std::string, uint64_t> p(name.toStdString(), score);
        highscores.push_back(p);
    }

    std::sort(highscores.begin(), highscores.end(), less_than_score());
    return highscores;
}

}
