// @author Alessio Modonesi

#include "UnifiedLogger.h"

// Inizializzazione del logger globale
UnifiedLogger logger("../log.txt");

// Costruttore
UnifiedLogger::UnifiedLogger(const std::string& filename) {
    fileStream.open(filename, std::ios::out | std::ios::trunc); // Modalità trunc per svuotare il file
    if (!fileStream.is_open())
        throw std::runtime_error("Impossibile aprire il file: " + filename);
}

// Distruttore
UnifiedLogger::~UnifiedLogger() {
    if (fileStream.is_open())
        fileStream.close();
}

// Overload dell'operatore << per manipolatori di stream
UnifiedLogger& UnifiedLogger::operator<<(std::ostream& (*manip)(std::ostream&)) {
    std::cout << manip;
    if (fileStream.is_open())
        fileStream << manip;
    return *this;
}