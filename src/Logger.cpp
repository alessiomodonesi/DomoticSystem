// @author David Popusoi

#include <iostream>
#include <stdexcept>
#include <sstream>

#include "Logger.h"

Logger::Logger(const std::string &outputPath)
    : logFile_(outputPath, std::ios::app)
{
    if (!logFile_.is_open())
        throw std::runtime_error("Cannot open log file: " + std::string(outputPath));
}

// Distruttore
Logger::~Logger()
{
    if (logFile_.is_open())
        logFile_.close();
}

void Logger::log(const Level level, const std::string &message)
{
    // std::string fullMessage = "[" + tempo + "] " + message;
    // logFile << fullMessage << std::endl;
}

std::string Logger::levelToString(Level level)
{
    switch (level)
    {
    case Level::INFO:
        return "INFO";
    case Level::WARNING:
        return "ATTENZIONE";
    case Level::ERROR:
        return "ERRORE";
    default:
        return "SCONOSCIUTO";
    }
}

Logger defaultLogger("log.txt");