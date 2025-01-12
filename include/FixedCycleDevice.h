// @author

#ifndef FIXEDCYCLEDEVICE_H
#define FIXEDCYCLEDEVICE_H

#include "DomoticDevice.h"

// Rappresenta un dispositivo con un ciclo di funzionamento prefissato.
class FixedCycleDevice : public DomoticDevice
{
private:
    const int cycleDuration_; // Durata del ciclo in minuti

public:
    // Costruttore: inizializza il dispositivo con nome, consumo e durata ciclo.
    FixedCycleDevice(const std::string &name, double powerConsumption, int cycleDuration);

    // Metodo per accendere il dispositivo. Gestisce anche lo stato interno.
    bool turnOn(void) override;

    // Overloading del metodo per i dispositivi di tipo FixedCycle
    void setTimer(const Time &startTime);

    // Interrompe manualmente un ciclo in corso.
    // Questo metodo è utile per sovraccarico o interventi manuali.
    void stopCycle(void);

    // Getter per la durata del ciclo.
    int getCycleDuration(void) const { return cycleDuration_; }
};

#endif // FIXEDCYCLEDEVICE_H