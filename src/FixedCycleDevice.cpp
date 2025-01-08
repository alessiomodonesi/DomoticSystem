// @author David Popusoi

#include "FixedCycleDevice.h"

// Costruttore: inizializza il dispositivo con nome, consumo e durata ciclo.
FixedCycleDevice::FixedCycleDevice(const std::string &name, double powerConsumption, int cycleDuration)
    : DomoticDevice(name, powerConsumption), cycleDuration_(cycleDuration) {}

// Setta l'offTime del device di tipo FixedCycle con l'ausilio della struct Time
void FixedCycleDevice::setTimer(const Time &startTime, const int cycleDuration)
{
    setOffTime(Time::formattingTime(startTime, cycleDuration));
}

// Interrompe manualmente un ciclo in corso.
void FixedCycleDevice::stopCycle(void)
{
    setOffTime(Time(-1, -1));
}