// @author Alessio Modonesi

#ifndef DOMOTICDEVICE_H
#define DOMOTICDEVICE_H

#include <string>

#include "Time.h"

// Classe base per rappresentare un dispositivo domotico generico.
// Questa classe offre funzionalità comuni come accensione/spegnimento,
// gestione timer, e calcolo del consumo energetico.
class DomoticDevice
{
protected:
    const std::size_t id_;          // ID univoco del dispositivo
    const std::string name_;        // Nome del dispositivo
    const double powerConsumption_; // Potenza consumata (negativa) o prodotta (positiva)
    bool isOn_{false};              // Stato del dispositivo: acceso o spento
    Time startTime_{NULL};          // Orario di accensione automatica 
    Time offTime_{NULL};            // Orario di spegnimento automatico (opzionale)

    // AGGIUNTA
    double totalEnergyConsumption_; // Consumo energetico totale dalle 00:00

public:
    // Costruttore: inizializza il dispositivo con un ID, un nome e un valore di consumo energetico.
    DomoticDevice(const std::string &name, double powerConsumption);

    // Metodo per accendere il dispositivo. Gestisce anche lo stato interno.
    void turnOn(void);

    // Metodo per spegnere il dispositivo. Può essere sovrascritto da classi derivate.
    void turnOff(void);

    // Imposta l’orario di accensione e spegnimento per il dispositivo.
    void setTimer(const Time &startTime, const Time &offTime);

    // Calcola il consumo energetico in base alle ore di funzionamento.
    // Chiarimento: Il consumo viene calcolato considerando un assorbimento costante.
    double calculateEnergyConsumption(const Time &startTime, const Time &offTime);

    // Distruttore virtuale per supportare l'ereditarietà.
    virtual ~DomoticDevice();

    // Getter per ID, nome, potenza, stato ed orario e consumo energetico totale dalle 00:00.
    std::size_t getId(void) const { return id_; }
    std::string &getName(void) const { return name_; }
    double getPowerConsumption(void) const { return powerConsumption_; }
    bool isDeviceOn(void) const { return isOn_; }
    Time getStartTime(void) const { return startTime_;}
    Time getOffTime(void) const { return offTime_;}

    double getTotalEnergyConsumption(void) const { return totalEnergyConsumption_; } // AGGIUNTA

    // Setter per startTime_ and offTime_.
    void setStartTime(Time startTime) { startTime_ = startTime; }
    void setOffTime(Time offTime) { offTime_ = offTime; }
};

// Ritorna lo stato del dispositivo in formato leggibile.
std::ostream &operator<<(std::ostringstream &os, const DomoticDevice &obj);

#endif // DOMOTICDEVICE_H