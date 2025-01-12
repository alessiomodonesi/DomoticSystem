// @author

#include <iostream>
#include "DomoticDevice.h"

// Costruttore: inizializza il dispositivo con un ID, un nome e un valore di consumo energetico.
DomoticDevice::DomoticDevice(const std::string &name, double powerConsumption)
    : id_(std::hash<std::string>{}(name)), name_(name), powerConsumption_(powerConsumption), dailyConsumption_(0) {}

// Distruttore virtuale per supportare l'ereditarietà.
DomoticDevice::~DomoticDevice() = default;

// Metodo per accendere il dispositivo.
bool DomoticDevice::turnOn(void)
{
    if (!this->isOn_)
    {
        this->isOn_ = true;
        this->setTimer(NOW, this->offTime_);
        return true;
    }
    else
    {
        logger << "Il dispositivo è già acceso" << std::endl;
        return false;
    }
}

// Metodo per spegnere il dispositivo.
bool DomoticDevice::turnOff()
{
    if (this->isOn_)
    {
        this->isOn_ = false;
        this->setTimer(Time(-1, -1), Time(-1, -1));
        return true;
    }
    else
    {
        logger << "Il dispositivo è già spento" << std::endl;
        return false;
    }
}

// Imposta l’orario di accensione e spegnimento per il dispositivo.
void DomoticDevice::setTimer(const Time &startTime, const Time &offTime)
{
    if (startTime != Time(-1, -1) && offTime != Time(-1, -1)) // Chiamata del tipo "set ${DEVICENAME} ${START} ${STOP}"
    {
        // Controllo che startTime sia minore di offTime
        if (startTime <= offTime)
        {
            this->startTime_ = startTime;
            this->offTime_ = offTime;
            // logger<< "DEBUG: set ${DEVICENAME} ${START} ${STOP}" << std::endl;
        }
        else
            logger << "L'orario di accensione deve essere precedente a quello di spegnimento" << std::endl;
    }
    else if (startTime != Time(-1, -1) && offTime == Time(-1, -1)) // Chiamata del tipo "set ${DEVICENAME} on", "set ${DEVICENAME} ${START}"
    {
        this->startTime_ = startTime;
        this->offTime_ = offTime;
        // logger<< "DEBUG: set ${DEVICENAME} on | set ${DEVICENAME} ${START}" << std::endl;
    }
    else if (startTime == Time(-1, -1) && offTime == Time(-1, -1)) // Chiamata del tipo "set ${DEVICENAME} off", "reset time", "reset all"
    {
        this->startTime_ = startTime;
        this->offTime_ = offTime;
        // logger<< "DEBUG: set ${DEVICENAME} off | reset time | reset all" << std::endl;
    }
    else
        logger << "Impossibile settare il timer" << std::endl;
}

// Calcola la produzione/consumo energetico di uno specifico dispositivo durante l'ultima accensione.
double DomoticDevice::calculateEnergyConsumption(const Time &startTime) const
{
    Time intervals = NOW - startTime;
    int minutes = (intervals.getHours() * 60) + intervals.getMinutes();
    return (this->powerConsumption_ / 60.0) * minutes;
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