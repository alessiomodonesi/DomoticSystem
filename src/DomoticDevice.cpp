// @author Alessio Modonesi

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

#include "DomoticDevice.h"

// Costruttore: inizializza il dispositivo con un ID, un nome e un valore di consumo energetico.
DomoticDevice::DomoticDevice(const std::string &name, double powerConsumption)
    : id_(std::hash<std::string>{}(name)), name_(name), powerConsumption_(powerConsumption), totalEnergyConsumption_(0) {}  // AGGIUNTA

// Metodo per accendere il dispositivo.
void DomoticDevice::turnOn() 
{
    if (!isOn_)
        isOn_ = true;
}

// Metodo per spegnere il dispositivo.
void DomoticDevice::turnOff() 
{
    if (isOn_)
        isOn_ = false;
}

// Imposta l’orario di accensione e spegnimento per il dispositivo.
void DomoticDevice::setTimer(const Time &startTime, const Time &offTime) {
    setStartTime(startTime);
    setOffTime(offTime);
}

// Calcola il consumo energetico in base alle ore di funzionamento.
double DomoticDevice::calculateEnergyConsumption(const Time &startTime, const Time &offTime) 
{
    Time intervals = offTime - startTime;
    double useTime = intervals.h + (intervals.m / 60);
    totalEnergyConsumption_ += powerConsumption_ * useTime; // AGGIUNTA
    return powerConsumption_ * useTime;
}

// Distruttore virtuale per supportare l'ereditarietà.
DomoticDevice::~DomoticDevice() = default;

// Ritorna lo stato del dispositivo in formato leggibile.
std::ostream &operator<<(std::ostream &os, const std::unique_ptr<DomoticDevice> &obj){
    os << "ID: " << getId()
           << "\nName: " << getName() 
           << "\nStatus: " << (isDeviceOn() ? "Acceso" : "Spento")
           << "\nEnergyConsumption: " << calculateEnergyConsumption(getStartTime(), getOffTime()) << " kW\n";
           << "\nTotalEnergyConsumption: " << getTotalEnergyConsumption() << " kW\n";
    return os;
}