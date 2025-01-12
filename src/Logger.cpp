// @author

#include "Logger.h"

// Inizializzazione del logger globale
Logger logger("../log.txt");

// Costruttore
Logger::Logger(const std::string &filename)
{
    fileStream.open(filename, std::ios::out | std::ios::trunc); // Modalità trunc per svuotare il file
    if (!fileStream.is_open())
        throw std::runtime_error("Impossibile aprire il file: " + filename);
}

// Distruttore
Logger::~Logger()
{
    if (fileStream.is_open())
        fileStream.close();
}

// Overload dell'operatore << per *manip
void Logger::log(std::ostream &(*manip)(std::ostream &))
{
    if (fileStream.is_open())
        fileStream << manip;
}

// Overload dell'operatore << per *manip
Logger &Logger::operator<<(std::ostream &(*manip)(std::ostream &))
{
    std::cout << manip;
    if (fileStream.is_open())
        fileStream << manip;
    return *this;
}
