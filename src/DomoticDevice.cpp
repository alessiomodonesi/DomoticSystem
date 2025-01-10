// @author Alessio Modonesi

#include <iostream>
#include "DomoticDevice.h"

// Costruttore: inizializza il dispositivo con un ID, un nome e un valore di consumo energetico.
DomoticDevice::DomoticDevice(const std::string &name, double powerConsumption)
    : id_(std::hash<std::string>{}(name)), name_(name), powerConsumption_(powerConsumption), dailyConsumption_(0) {}

// Distruttore virtuale per supportare l'ereditarietà.
DomoticDevice::~DomoticDevice() = default;

// Metodo per accendere il dispositivo.
void DomoticDevice::turnOn(void)
{
    if (!this->isOn_)
        this->isOn_ = true;

    this->setTimer(NOW, this->offTime_);
}

// Metodo per spegnere il dispositivo.
void DomoticDevice::turnOff()
{
    if (this->isOn_)
        this->isOn_ = false;

    this->dailyConsumption_ += calculateEnergyConsumption(getStartTime(), getOffTime());
    this->setTimer(Time(-1, -1), Time(-1, -1));
}

// Imposta l’orario di accensione e spegnimento per il dispositivo.
void DomoticDevice::setTimer(const Time &startTime, const Time &offTime)
{
    this->startTime_ = startTime;
    this->offTime_ = offTime;
}

// // Mostra a schermo (calcola) la produzione/consumo energetico di uno specifico dispositivo.
double DomoticDevice::calculateEnergyConsumption(const Time &startTime, const Time &offTime) const
{
    Time intervals = offTime - startTime;
    double usedTime = intervals.getHours() + (intervals.getMinutes() / 60);
    return this->powerConsumption_ * usedTime;
}

// Ritorna lo stato del dispositivo in formato leggibile.
std::ostream &operator<<(std::ostream &os, const DomoticDevice &device)
{
    os << "ID: " << device.getId()
       << "\nNome: " << device.getName()
       << "\nStato: " << (device.isDeviceOn() ? "Acceso" : "Spento");

    if (device.getPowerConsumption() > 0)
    {
        os << "\nPotenza Prodotta: " << device.getPowerConsumption() << " kW";
        os << "\nEnergia Giornaliera Prodotta: " << device.getDailyConsumption() << " kWh\n";
    }
    else
    {
        os << "\nPotenza Consumata: " << device.getPowerConsumption() << " kW";
        os << "\nEnergia Giornaliera Consumata: " << device.getDailyConsumption() << " kWh\n";
    }
    return os;
}