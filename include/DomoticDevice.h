// @author Alessio Modonesi

#ifndef DOMOTICDEVICE_H
#define DOMOTICDEVICE_H

#include <string>

// Classe base per rappresentare un dispositivo domotico generico.
// Questa classe offre funzionalità comuni come accensione/spegnimento,
// gestione timer, e calcolo del consumo energetico.
class DomoticDevice
{
protected:
    std::string name_{"device_"};   // Nome del dispositivo
    int id_;                        // ID univoco del dispositivo
    double powerConsumption_{0};    // Potenza consumata (positiva) o prodotta (negativa)
    bool isOn_{false};              // Stato del dispositivo: acceso o spento

public:
    // Costruttore: inizializza il dispositivo con un nome, un ID e un valore di consumo energetico.
    DomoticDevice(const std::string &name, double powerConsumption);

    // Distruttore virtuale per supportare l'ereditarietà.
    virtual ~DomoticDevice();

    // Metodo per accendere il dispositivo. Gestisce anche lo stato interno.
    virtual void turnOn();

    // Metodo per spegnere il dispositivo. Può essere sovrascritto da classi derivate.
    virtual void turnOff();

    // Imposta un timer per l'accensione e lo spegnimento del dispositivo.
    // È necessario validare il formato degli orari passati come argomenti
    virtual void setTimer(const std::string &startTime, const std::string &endTime);

    // Calcola il consumo energetico in base alle ore di funzionamento.
    // Chiarimento: Il consumo viene calcolato considerando un assorbimento costante.
    virtual double calculateEnergyConsumption(double hours) const;

    // Ritorna lo stato del dispositivo in formato leggibile.
    virtual std::string getStatus() const;

    // Getter per nome, ID, potenza e stato.
    const std::string &getName(void) const { return name_; }
    int getId(void) const { return id_; }
    double getPowerConsumption(void) const { return powerConsumption_; }
    bool isDeviceOn(void) const { return isOn_; }
};

#endif // DOMOTICDEVICE_H