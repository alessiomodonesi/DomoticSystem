// @author Alessio Modonesi

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

#include "DomoticDevice.h"

// Costruttore: inizializza il dispositivo con un ID, un nome e un valore di consumo energetico.
DomoticDevice::DomoticDevice(const std::string &name, double powerConsumption)
    : id_(std::hash<std::string>{}(name)), name_(name), powerConsumption_(powerConsumption) {}

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
void setTimer(const Time &startTime, const Time &offTime) {
    startTime_ = startTime;
    offTime_ = offTime_;
}

// Calcola il consumo energetico in base alle ore di funzionamento.
double DomoticDevice::calculateEnergyConsumption(const Time &startTime, const Time &offTime) 
{
    Time intervals = offTime - startTime;
    double useTime = intervals.h + (intervals.m / 60);
    return powerConsumption_ * useTime;
}

// Distruttore virtuale per supportare l'ereditarietà.
DomoticDevice::~DomoticDevice() = default;

// Ritorna lo stato del dispositivo in formato leggibile.
std::ostream &operator<<(std::ostringstream &os, const DomoticDevice &obj){
    std::ostringstream status;
    status << "ID: " << id_
           << "\nName: " << name_ 
           << "\nStatus: " << (isOn_ ? "Acceso" : "Spento")
           << "\nEnergyConsumption: " << calculateEnergyConsumption(const Time &startTime, const Time &offTime) << " kW\n";
    return status.str();
}