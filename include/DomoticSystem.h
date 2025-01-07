// @author Tommaso Silvestrin

#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "DomoticDevice.h"
#include "Time.h"

// Classe per gestire l'intero sistema domotico.
// Collega tutti i dispositivi e controlla il consumo totale per evitare sovraccarichi.
class DomoticSystem
{
private:
    std::unique_ptr<std::vector<DomoticDevice>> devices_;    // Lista di dispositivi gestiti dal sistema
    double maxPowerConsumption_;                             // Potenza massima consentita (kW)
    static constexpr double DEFAULT_POWER_CONSUMPTION = 3.5; // Potenza massima consentita di default

    // Gestisce situazioni di sovraccarico spegnendo i dispositivi in ordine inverso.
    void handleOverConsumption(void);

    // Calcola il consumo corrente sommando i consumi di tutti i dispositivi accesi.
    double calculateCurrentConsumption(void) const;

public:
    // Costruttore: inizializza il sistema con un limite massimo di potenza.
    DomoticSystem(double powerConsumption = DEFAULT_POWER_CONSUMPTION);

    // Aggiunge un dispositivo alla lista gestita.
    void addDevice(const DomoticDevice &device);

    // Rimuove un dispositivo dalla lista tramite il suo ID.
    void removeDevice(int id);

    // Esegue un comando dato come input.
    void executeCommand(const std::string &command);

    // Mostra lo stato attuale del sistema.
    void displaySystemStatus(void) const;

    // Registra un evento in un log.
    void logEvent(const std::string &event) const;

    // COMANDI PER IL DEBUG:

    // Resetta il tempo del sistema.
    void resetTime(void);

    // Rimuove i timer di tutti i dispositivi.
    void stopAllCycle(void);

    // Riporta il sistema alle condizioni iniziali.
    void resetAll(void);
};

// Mostra la lista di tutti i dispositivi.
std::ostream &operator<<(std::ostringstream &os, const DomoticSystem &obj);

#endif // DOMOTICSYSTEM_H