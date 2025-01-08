// @author David Popusoi

#include "FixedCycleDevice.h"

// Costruttore: inizializza il dispositivo con nome, consumo e durata ciclo.
FixedCycleDevice::FixedCycleDevice(const std::string &name, double powerConsumption, double cycleDuration)
    : DomoticDevice(name, powerConsumption), // Chiama il costruttore della classe padre.
      cycleDuration_(cycleDuration) {}       // Inizializza l'attributo specifico della classe derivata.

// Setta l'offTime del device di tipo FixedCycle con l'ausilio della struct Time
void FixedCycleDevice::setTimer(const Time &startTime, const int cycleDuration) {
    offTime_ = Time::formattingTime(&startTime, cycleDuration);
}

// Interrompe manualmente un ciclo in corso.
void FixedCycleDevice::stopCycle(void) {
    offTime_ = NULL;
}