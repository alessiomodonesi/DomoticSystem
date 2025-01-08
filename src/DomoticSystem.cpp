// @author Tommaso Silvestrin

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>

#include "DomoticSystem.h"

// Costruttore: inizializza il sistema con un limite massimo di potenza.
DomoticSystem::DomoticSystem(double powerConsumption)
    : maxPowerConsumption_{powerConsumption}, totalSystemEnergyConsumption_{0} {    // AGGIUNTA
        if (maxPowerConsumption_ < 0.5 || maxPowerConsumption_ > 6.0)
            throw std::invalid_argument("maxPowerConsumption must be [0.5 kW, 6.0 kW]");
    }

// Calcola il consumo corrente sommando i consumi di tutti i dispositivi accesi.
double DomoticSystem::calculateCurrentConsumption(void) const
{
    double totalConsumption = 0;
    for (const auto &device : devices_)
    {
        if(device->isOn())
            totalConsumption += device.getPowerConsumption();
    }
    return totalConsumption;
}

// Function object per il predicato del find_if nel metodo successivo
class overConsumption
{
    public:
        bool operator()(const std::vector<std::unique_ptr<DomoticDevice>> &devices__) const
        {
            for (const auto &device : devices_)
                return device->isOn();
        }
};

// Gestisce situazioni di sovraccarico spegnendo i dispositivi in ordine inverso.
void DomoticSystem::handleOverConsumption(void)
{
    while (calculateCurrentConsumption() > maxPowerConsumption_)
    {
        auto it = std::find_if(devices_.rbegin(), devices_.rend(), overConsumption());
        // rbegin e rend (reverse) servono per cercare in senso invertito, dato che quando si spegne un dispositivo si parte dall'ultimo acceso

        if (it != devices_.rend())
            *it->turnOff();
        else
            throw std::runtime_error("all devices are off");
    }
}

// Aggiunge un dispositivo alla lista gestita.
void DomoticSystem::addDevice(std::unique_ptr<DomoticDevice> device)
{
    devices_.push_back(std::move(device));
}

// function object predicato per il find_if nel metodo successivo
class idIsPresent
{
    std::size_t ID;
    public:
        idIsPresent(std::size_t id) : ID{id} {}

        bool operator()(std::unique_ptr<DomoticDevice> device) const
        {
            return device->getId() == ID;
        }
};

// Rimuove un dispositivo dalla lista tramite il suo ID.
void DomoticSystem::removeDevice(std::size_t id)
{
    auto it = std::find_if(devices_.begin(), devices_.end(), idIsPresent(id));
    if (it != devices_.end())
        devices_.erase(it);
    else
        throw std::runtime_error("device not found");
}

// Esegue un comando dato come input.
void DomoticSystem::executeCommand(const std::string &command) {}

// Mostra lo stato attuale del sistema.
void DomoticSystem::displaySystemStatus(void) const
{
    for (const auto& device : devices_)
        std::cout << device->getStatus() << std::endl;

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

// COMANDI PER IL DEBUG

// Resetta il tempo del sistema.
void DomoticSystem::resetTime(void) {}

// Rimuove i timer di tutti i dispositivi.
void DomoticSystem::resetTimers(void) {
    for (const auto &device : devices_) {
        // Rimuove i timer dei device FixedCycle.
        if (auto fixedDevice = dynamic_cast<FixedCycleDevice*>(device.get()))
            fixedDevice->stopCycle();
        
        // Setta offTime_ a NULL per i device non FixedCycle.
        else {
            if (device->getOffTime() != NULL)
                device->setOffTime(NULL);
        }      
    }
}

// Riporta il sistema alle condizioni iniziali.
void DomoticSystem::resetAll(void) {}

// Mostra la lista di tutti i dispositivi.
std::ostream &operator<<(std::ostringstream &os, const DomoticSystem &obj) {
    std::ostringstream show;
    for (const auto &device : devices_)
        show << device;

    // Manca il conteggio e la stampa di totalSystemEnergyConsumption_ che va fatto sommando i totalEnergyConsumption_ di ogni device
    
    return status.str();
}