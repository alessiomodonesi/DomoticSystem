// @author Tommaso Silvestrin

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "DomoticSystem.h"
#include "DomoticDevice.h"
#include "Time.h"

// Costruttore: inizializza il sistema con un limite massimo di potenza.
DomoticSystem::DomoticSystem(double powerConsumption)
    : maxPowerConsumption_{powerConsumption} {
        if (maxPowerConsumption_ < 0.5 || maxPowerConsumption_ > 6.0)
            throw std::invalid_argument("maxPowerConsumption must be [0.5 kW, 6.0 kW]");
    }

// Function object per il predicato del find_if nel metodo successivo
class overConsumption
{
    public:
        bool operator()(const std::unique_ptr<std::vector<DomoticDevice>> &devices__) const
        {
            return device.isOn();
        }
}

// Gestisce situazioni di sovraccarico spegnendo i dispositivi in ordine inverso.
void DomoticSystem::handleOverConsumption(void)
{
    while (calcultedCurrentConsumption() > maxPowerConsumption_)
    {
        auto it = std::find_if(devices_.rbegin(), devices_.rend(), overConsumption());
        // rbegin e rend (reverse) servono per cercare in senso invertito, dato che quando si spegne un dispositivo si parte dall'ultimo acceso

        if (it != devices_.rend())
            *it.turnOff();
        else
            throw std::runtime_error("all devices are off");
    }
}

// Calcola il consumo corrente sommando i consumi di tutti i dispositivi accesi.
double DomoticSystem::calculateCurrentConsumption(void) const
{
    double totalConsumption = 0;
    for (const auto &device : devices_)
    {
        if(device.isOn())
            totalConsumption += device.getPowerConsumption();
    }
    return totalConsumption;
}

// Aggiunge un dispositivo alla lista gestita.
void DomoticSystem::addDevice(const DomoticDevice &device)
{
    devices_.push_back(device);
}

// function object predicato per il find_if nel metodo successivo
class idIsPresent
{
    int ID;
    public:
        idIsPresent(int id) : ID{id} {}

        bool operator()(const DomoticDevice &device) const
        {
            return device.getID() == ID;
        }
}

// Rimuove un dispositivo dalla lista tramite il suo ID.
void DomoticSystem::removeDevice (int id)
{
    auto it = std::find_if(devices_.begin(), devices_.end(), idIsPresent(id));
    if (it != devices_.end())
        devices_.erase(it);
    else
        throw std::runtime_error("device not found");
}

// Esegue un comando dato come input.
void DomoticSystem::executeCommand (const std::string &command) {}

// Mostra lo stato attuale del sistema.
void DomoticSystem::displaySystemStatus(void) const
{
    for (const auto& device : devices_)
        std::cout << device.getStatus() << std::endl;

    std::cout << "Current total consumption: " << calculateCurrentConsumption() << " kW" << std::endl;
}

// Registra un evento in un log.
void DomoticSystem::logEvent(const std::string &event) const
{
    static const std::string fileName = "log.txt";
    std::ofstream logFile(fileName, std::ios_base::app);

    if (logFile.is_open())
        logFile << event << std::endl;
    else
        throw std::runtime_error("cannot open log file");
}

// COMANDI PER IL DEBUG:

// Resetta il tempo del sistema.
void resetTime(void) {}

// Rimuove i timer di tutti i dispositivi.
void stopAllCycle(void) {}

// Riporta il sistema alle condizioni iniziali.
void resetAll(void) {}

// Mostra la lista di tutti i dispositivi.
std::ostream &operator<<(std::ostringstream &os, const DomoticSystem &obj) {}