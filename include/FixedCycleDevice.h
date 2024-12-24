#ifndef CYCLEDEVICE_H
#define CYCLEDEVICE_H

#include "DomoticDevice.h"

// Rappresenta un dispositivo con un ciclo di funzionamento prefissato.
class CycleDevice : public DomoticDevice
{
private:
    double cycleDuration;  // Durata del ciclo in ore
    std::string startTime; // Orario di inizio del ciclo

public:
    // Costruttore: inizializza il dispositivo con nome, ID, consumo e durata ciclo.
    CycleDevice(const std::string &name, int id, double powerConsumption, double cycleDuration);

    // Imposta un timer per il ciclo di funzionamento.
    void setTimer(const std::string &startTime, const std::string &endTime = "") override;

    // Interrompe manualmente un ciclo in corso.
    // Chiarimento: Questo metodo è utile per sovraccarico o interventi manuali.
    void stopCycle();

    // Getter per la durata e l'orario di inizio del ciclo.
    double getCycleDuration() const;
    std::string getStartTime() const;
};

#endif // CYCLEDEVICE_H