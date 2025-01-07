// @author David Popusoi

#include "FixedCycleDevice.h"

// Costruttore: inizializza il dispositivo con nome, consumo e durata ciclo.
CycleDevice::CycleDevice(const std::string &name, double powerConsumption, double cycleDuration)
    : id_(std::hash<std::string>{}(name)), name_(name), powerConsumption_(powerConsumption), cycleDuration_(cycleDuration) {}

// Setta l'offTime del device di tipo FixedCycle con l'ausilio della struct Time
void CycleDevice::setTimer(const Time &startTime, const int cycleDuration) {
    offTime_ = Time::formattingTime(&startTime, cycleDuration);
}

// Interrompe manualmente un ciclo in corso.
void CycleDevice::stopCycle(void) {
    offTime_ = NULL;
}