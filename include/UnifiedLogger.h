// @author Alessio Modonesi

#ifndef UNIFIEDLOGGER_H
#define UNIFIEDLOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

class UnifiedLogger {
private:
    std::ofstream fileStream;

public:
    // Costruttore
    UnifiedLogger(const std::string& filename);

    // Distruttore
    ~UnifiedLogger();

    // Overload dell'operatore << per manipolatori di stream
    UnifiedLogger& operator<<(std::ostream& (*manip)(std::ostream&));
    
    // Overload dell'operatore << per tipi generici
    template <typename T>
    UnifiedLogger& operator<<(const T& message) {
        std::cout << message;
        if (fileStream.is_open())
            fileStream << message;
        return *this;
    }
};

// Dichiarazione del logger globale
extern UnifiedLogger logger;

#endif // UNIFIEDLOGGER_H