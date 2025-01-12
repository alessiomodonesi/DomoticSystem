// @author

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

class Logger {
private:
    std::ofstream fileStream;

public:
    // Costruttore
    Logger(const std::string& filename);

    // Distruttore
    ~Logger();

    // Metodo per scrivere solo sul file di log
    template <typename T>
    void log(const T& message) {
        if (fileStream.is_open())
            fileStream << message;
    }

    // Overload dell'operatore << per *manip
    void log(std::ostream& (*manip)(std::ostream&));

    // Overload dell'operatore << per tipi generici
    template <typename T>
    Logger& operator<<(const T& message) {
        std::cout << message;
        if (fileStream.is_open())
            fileStream << message;
        return *this;
    }

    // Overload dell'operatore << per *manip
    Logger& operator<<(std::ostream& (*manip)(std::ostream&));
};

// Dichiarazione del logger globale
extern Logger logger;

#endif // LOGGER_H