// @author David Popusoi

#ifndef FIXEDCYCLEDEVICE_H
#define FIXEDCYCLEDEVICE_H

#include "DomoticDevice.h"

// Rappresenta un dispositivo con un ciclo di funzionamento prefissato.
class FixedCycleDevice : public DomoticDevice
{
private:
    const int cycleDuration_;  // Durata del ciclo in minuti

public:
    // Costruttore: inizializza il dispositivo con nome, consumo e durata ciclo.
    FixedCycleDevice(const std::string &name, double powerConsumption, double cycleDuration);

    // Overloading del metodo per i dispositivi di tipo FixedCycle
    void setTimer(const Time &startTime, const int cycleDuration);

    // Interrompe manualmente un ciclo in corso.
    // Questo metodo è utile per sovraccarico o interventi manuali.
    void stopCycle(void);

    // Getter per la durata del ciclo.
    double getCycleDuration(void) const { return cycleDuration_; }
};

#endif // FIXEDCYCLEDEVICE_H