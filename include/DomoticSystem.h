// @author Tommaso Silvestrin

#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H

#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <memory>
#include <fstream>

#include "Logger.h"
#include "Time.h"

// Dichiarazione anticipata della classe DomoticDevice
class DomoticDevice;

// Classe per gestire l'intero sistema domotico.
// Collega tutti i dispositivi e controlla il consumo totale per evitare sovraccarichi.
class DomoticSystem
{
private:
    std::vector<std::unique_ptr<DomoticDevice>> devices_;                                             // Lista di dispositivi gestiti dal sistema
    std::unordered_map<std::string, std::function<void(const std::vector<std::string> &)>> commands_; // Lista comandi disponibili per l'interfaccia utente
    double maxPowerConsumption_;                                                                      // Potenza massima consentita (kW)
    double dailySystemConsumption_;                                                                   // Consumo energetico totale del sistema dalle 00:00
    static constexpr double DEFAULT_POWER_CONSUMPTION = 3.5;                                          // Potenza massima consentita di default

    // Inizializza i comandi presenti nell'interfaccia utente
    void initializeCommands(void);

    // Calcola il consumo corrente sommando i consumi di tutti i dispositivi accesi.
    double calculateCurrentConsumption(void) const;

    // Gestisce situazioni di sovraccarico spegnendo i dispositivi in ordine inverso.
    void handleOverConsumption(void);

public:
    // Costruttore: inizializza il sistema con un limite massimo di potenza.
    DomoticSystem(double powerConsumption = DEFAULT_POWER_CONSUMPTION);

    // Aggiunge un dispositivo alla lista gestita.
    void addDevice(std::unique_ptr<DomoticDevice> device);

    // Rimuove un dispositivo dalla lista tramite il suo ID.
    void removeDevice(std::size_t id);

    // Esegue un comando dato come input.
    void executeCommand(const std::string &input);

    // COMANDI PER IL DEBUG:

    /*  reset time:
        Resetta il tempo del sistema, riportandolo all’orario 00:00.
        Riporta tutti i dispositivi alle condizioni iniziali.
        Gli eventuali timer aggiunti dopo l’avvio del sistema vengono mantenuti.
    */
    void resetTime(void);

    /*  reset timers:
        Rimuove i timer di tutti i dispositivi.
        Tutti i dispositivi rimangono nel loro stato attuale.
    */
    void resetTimers(void);

    /* reset all, riporta il sistema alle condizioni iniziali:
        L’orario viene impostato a 00:00,
        Tutti i timer vengono rimossi,
        Tutti i dispositivi vengono spenti.
    */
    void resetAll(void);

    // Getter per dailySystemConsumption_ e storedTime_
    double getDailySystemConsumption(void) const { return dailySystemConsumption_; }

    // Mostra la lista di tutti i dispositivi.
    friend std::ostream &operator<<(std::ostringstream &os, const std::vector<std::unique_ptr<DomoticDevice>> &devices_);
};

#endif // DOMOTICSYSTEM_H