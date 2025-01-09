// @author David Popusoi

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

struct Logger
{
    // Possiamo definire diversi livelli per differenziare semplici informazioni ed errori
    enum class Level { INFO, WARNING, ERROR };

    explicit Logger(const std::string &outputPath);
    ~Logger();

    void log(const Level level = Level::INFO, const std::string &message);

    private:
        std::ofstream logFile_;

        std::string levelToString(Level level);
}

// Logger usato dal sistema
extern Logger LOGGER;

#endif // LOGGER_H
