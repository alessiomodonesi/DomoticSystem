// @author Alessio Modonesi

#include <iostream>
#include "DomoticDevice.h"

// Costruttore: inizializza il dispositivo con un ID, un nome e un valore di consumo energetico.
DomoticDevice::DomoticDevice(const std::string &name, double powerConsumption)
    : id_(std::hash<std::string>{}(name)), name_(name), powerConsumption_(powerConsumption), totalEnergyConsumption_(0) {}

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
void DomoticDevice::setTimer(const Time &startTime, const Time &offTime)
{
    setStartTime(startTime);
    setOffTime(offTime);
}

// Calcola il consumo energetico in base alle ore di funzionamento.
double DomoticDevice::calculateEnergyConsumption(const Time &startTime, const Time &offTime)
{
    Time intervals = offTime - startTime;
    double useTime = intervals.getHours() + (intervals.getMinutes() / 60);
    totalEnergyConsumption_ += powerConsumption_ * useTime;
    return powerConsumption_ * useTime;
}

// Distruttore virtuale per supportare l'ereditarietà.
DomoticDevice::~DomoticDevice() = default;

// Ritorna lo stato del dispositivo in formato leggibile.
std::ostream &operator<<(std::ostream &os, const DomoticDevice &device)
{
    os << "ID: " << device.getId()
       << "\nName: " << device.getName()
       << "\nStatus: " << (device.isDeviceOn() ? "Acceso" : "Spento")
       << "\nEnergyConsumption: " << device.calculateEnergyConsumption(device.getStartTime(), device.getOffTime()) << " kW\n"
       << "\nTotalEnergyConsumption: " << device.getTotalEnergyConsumption() << " kW\n";
    return os;
}