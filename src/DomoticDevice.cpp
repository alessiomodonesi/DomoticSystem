// @author Alessio Modonesi

#include <iostream>
#include <stdexcept>
#include <sstream>

#include "DomoticDevice.h"

// Costruttore: inizializza il dispositivo con un nome, un ID e un valore di consumo energetico.
DomoticDevice::DomoticDevice(const std::string &name, double powerConsumption)
    : name_(name), id_(std::hash<std::string>{}(name)), powerConsumption_(powerConsumption) 
{
    if (powerConsumption < 0)
        throw std::invalid_argument("Il consumo energetico non può essere negativo.");
}

// Distruttore virtuale per supportare l'ereditarietà.
DomoticDevice::~DomoticDevice() = default;

// Metodo per accendere il dispositivo.
void DomoticDevice::turnOn() 
{
    if (!isOn_) {
        isOn_ = true;
        std::cout << "Dispositivo " << name_ << " acceso.\n";
    } else {
        std::cout << "Dispositivo " << name_ << " è già acceso.\n";
    }
}

// Metodo per spegnere il dispositivo.
void DomoticDevice::turnOff() 
{
    if (isOn_) {
        isOn_ = false;
        std::cout << "Dispositivo " << name_ << " spento.\n";
    } else {
        std::cout << "Dispositivo " << name_ << " è già spento.\n";
    }
}

// Imposta un timer per l'accensione e lo spegnimento del dispositivo.
void DomoticDevice::setTimer(const std::string &startTime, const std::string &endTime) 
{
    // Validazione del formato dell'orario
    if (startTime.size() != 5 || endTime.size() != 5 || startTime[2] != ':' || endTime[2] != ':')
        throw std::invalid_argument("Formato orario non valido. Usa HH:MM.");

    std::cout << "Timer impostato per il dispositivo " << name_ 
              << ": acceso dalle " << startTime << " alle " << endTime << ".\n";
}

// Calcola il consumo energetico in base alle ore di funzionamento.
double DomoticDevice::calculateEnergyConsumption(double hours) const 
{
    if (hours < 0)
        throw std::invalid_argument("Le ore di funzionamento non possono essere negative.");
    
    return powerConsumption_ * hours;
}

// Ritorna lo stato del dispositivo in formato leggibile.
std::string DomoticDevice::getStatus() const 
{
    std::ostringstream status;
    status << "Dispositivo: " << name_ 
           << "\nID: " << id_
           << "\nStato: " << (isOn_ ? "Acceso" : "Spento")
           << "\nConsumo: " << powerConsumption_ << " kW\n";
    return status.str();
}