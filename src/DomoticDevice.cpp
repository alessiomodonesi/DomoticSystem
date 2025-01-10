// @author Alessio Modonesi

#include <iostream>
#include "DomoticDevice.h"

// Costruttore: inizializza il dispositivo con un ID, un nome e un valore di consumo energetico.
DomoticDevice::DomoticDevice(const std::string &name, double powerConsumption)
    : id_(std::hash<std::string>{}(name)), name_(name), powerConsumption_(powerConsumption), dailyConsumption_(0) {}

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

// Calcola il consumo energetico in base alle ore di funzionamento.
double DomoticDevice::calculateEnergyConsumption(const Time &startTime, const Time &offTime) const
{
    Time intervals = offTime - startTime;
    double usedTime = intervals.getHours() + (intervals.getMinutes() / 60);
    return this->powerConsumption_ * usedTime;
}

// Mostra a schermo produzione/consumo energetico di uno specifico dispositivo.
double DomoticDevice::showCurrentEnergyConsumption(const Time &startTime) const
{
    double usedTime = NOW.getHours() + (NOW.getMinutes() / 60);
    return this->powerConsumption_ * usedTime;
}

// Distruttore virtuale per supportare l'ereditarietà.
DomoticDevice::~DomoticDevice() = default;

// Ritorna lo stato del dispositivo in formato leggibile.
std::ostream &operator<<(std::ostream &os, const DomoticDevice &device)
{
    os << "ID: " << device.getId()
       << "\nName: " << device.getName()
       << "\nStatus: " << (device.isDeviceOn() ? "Acceso" : "Spento");

    if (device.getPowerConsumption() > 0)
        os << "\nPower Production: " << device.getPowerConsumption() << " kW";
    else
        os << "\nPower Consumption: " << device.getPowerConsumption() << " kW";

    os << "\nDaily Energy Consumption: " << device.getDailyConsumption() << " kWh\n";
    return os;
}