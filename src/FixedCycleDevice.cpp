// @author

#include "FixedCycleDevice.h"

// Costruttore: inizializza il dispositivo con nome, consumo e durata ciclo.
FixedCycleDevice::FixedCycleDevice(const std::string &name, double powerConsumption, int cycleDuration)
    : DomoticDevice(name, powerConsumption), cycleDuration_(cycleDuration) {}

// Metodo per accendere il dispositivo.
bool FixedCycleDevice::turnOn(void)
{
    if (!this->isOn_)
    {
        this->isOn_ = true;
        this->setTimer(NOW);
        return true;
    }
    else
    {
        logger << "Il dispositivo è già acceso" << std::endl;
        return false;
    }
}

// Setta l'offTime del device di tipo FixedCycle con l'ausilio della struct Time
void FixedCycleDevice::setTimer(const Time &startTime)
{
    this->startTime_ = startTime;
    setOffTime(Time::calculateOffTime(startTime, this->cycleDuration_));
}

// Interrompe manualmente un ciclo in corso.
void FixedCycleDevice::stopCycle(void)
{
    setOffTime(Time(-1, -1));
}