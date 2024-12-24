#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H

#include <vector>
#include <memory>
#include <string>

#include "DomoticDevice.h"

// Classe per gestire l'intero sistema domotico.
// Collega tutti i dispositivi e controlla il consumo totale per evitare sovraccarichi.
class DomoticSystem
{
private:
    std::vector<std::shared_ptr<DomoticDevice>> devices; // Lista di dispositivi gestiti dal sistema
    double maxPowerConsumption;                          // Potenza massima consentita (kW)
    double photovoltaicProduction;                       // Produzione energetica dell'impianto fotovoltaico (kW)

    // Calcola il consumo corrente sommando i consumi di tutti i dispositivi accesi.
    double calculateCurrentConsumption() const;

    // Gestisce situazioni di sovraccarico spegnendo i dispositivi in ordine inverso.
    // Domanda: Come possiamo escludere dispositivi critici come il frigorifero da questa logica?
    void handleOverConsumption();

public:
    // Costruttore: inizializza il sistema con un limite massimo di potenza.
    DomoticSystem(double maxPowerConsumption);

    // Aggiunge un dispositivo alla lista gestita.
    void addDevice(const std::shared_ptr<DomoticDevice> &device);

    // Rimuove un dispositivo dalla lista tramite il suo ID.
    // Domanda: Come gestire errori se l'ID fornito non esiste?
    void removeDevice(int id);

    // Imposta la produzione fotovoltaica.
    void setPhotovoltaicProduction(double production);

    // Esegue un comando dato come input.
    // Chiarimento: Il metodo supporta una sintassi specifica per i comandi testuali.
    void executeCommand(const std::string &command);

    // Mostra lo stato attuale del sistema.
    void displaySystemStatus() const;

    // Registra un evento in un log.
    void logEvent(const std::string &event) const;
};

#endif // DOMOTICSYSTEM_H