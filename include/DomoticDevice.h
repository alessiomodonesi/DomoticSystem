// @author Alessio Modonesi

#ifndef DOMOTICDEVICE_H
#define DOMOTICDEVICE_H

#include "DomoticSystem.h"

// Classe base per rappresentare un dispositivo domotico generico.
// Questa classe offre funzionalità comuni come accensione/spegnimento,
// gestione timer, e calcolo del consumo energetico.
class DomoticDevice : public DomoticSystem
{
protected:
    const std::size_t id_;          // ID univoco del dispositivo
    const std::string name_;        // Nome del dispositivo
    const double powerConsumption_; // Potenza consumata (negativa) o prodotta (positiva)
    double dailyConsumption_;       // Consumo energetico totale dalle ore 00:00
    bool isOn_{false};              // Stato del dispositivo: acceso o spento
    Time startTime_{-1, -1};        // Orario di accensione automatica
    Time offTime_{-1, -1};          // Orario di spegnimento automatico (opzionale)

public:
    // Costruttore: inizializza il dispositivo con un ID, un nome e un valore di consumo energetico.
    DomoticDevice(const std::string &name, double powerConsumption);

    // Distruttore virtuale per supportare l'ereditarietà.
    virtual ~DomoticDevice();

    // Metodo per accendere il dispositivo. Gestisce anche lo stato interno.
    virtual void turnOn(void);

    // Metodo per spegnere il dispositivo. Può essere sovrascritto da classi derivate.
    void turnOff(void);

    // Imposta l’orario di accensione e spegnimento per il dispositivo.
    void setTimer(const Time &startTime, const Time &offTime);

    // Calcola il consumo energetico in base alle ore di funzionamento.
    // Chiarimento: Il consumo viene calcolato considerando un assorbimento costante.
    double calculateEnergyConsumption(const Time &startTime, const Time &offTime) const;

    // Mostra a schermo produzione/consumo energetico di uno specifico dispositivo.
    double showCurrentEnergyConsumption(const Time &startTime) const;

    // Getter per ID, nome, potenza, stato ed orario e consumo energetico totale dalle 00:00.
    std::size_t getId(void) const { return id_; }
    const std::string &getName(void) const { return name_; }
    double getPowerConsumption(void) const { return powerConsumption_; }
    double getDailyConsumption(void) const { return dailyConsumption_; }
    bool isDeviceOn(void) const { return isOn_; }
    Time getStartTime(void) const { return startTime_; }
    Time getOffTime(void) const { return offTime_; }

    // Setter per startTime_ and offTime_
    void setStartTime(Time startTime) { startTime_ = startTime; }
    void setOffTime(Time offTime) { offTime_ = offTime; }
};

// Ritorna lo stato del dispositivo in formato leggibile.
std::ostream &operator<<(std::ostream &os, const DomoticDevice &device);

#endif // DOMOTICDEVICE_H