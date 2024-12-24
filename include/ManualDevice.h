#ifndef MANUALDEVICE_H
#define MANUALDEVICE_H

#include "DomoticDevice.h"

// Rappresenta un dispositivo con funzionamento manuale.
class ManualDevice : public DomoticDevice
{
private:
    std::string offTime; // Timer per lo spegnimento automatico (opzionale)

public:
    // Costruttore: inizializza il dispositivo con nome, ID e consumo.
    ManualDevice(const std::string &name, int id, double powerConsumption);

    // Imposta un timer per accensione e spegnimento.
    void setTimer(const std::string &startTime, const std::string &endTime) override;

    // Rimuove il timer di spegnimento.
    // Estensione: Potremmo notificare l'utente che il timer è stato rimosso.
    void removeTimer();

    // Getter per il timer di spegnimento.
    std::string getOffTime() const;
};

#endif // MANUALDEVICE_H