// @author Tommaso Silvestrin

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "DomoticSystem.h"
#include "DomoticDevice.h"

// Costruttore: inizializza il sistema con un limite massimo di potenza.
DomoticSystem::DomoticSystem(double maxPowerConsumption)
    : maxPowerConsumption{maxPowerConsumption}, photovoltaicProduction{0} {}

// Calcola il consumo corrente sommando i consumi di tutti i dispositivi accesi.
double DomoticSystem::calculateCurrentConsumption () const
{
    double totalConsumption = 0;
    for (const auto& device : devices)
    {
        if(device.isOn())
        {
            totalConsumption += device.getPowerConsumption();
        }
    }
    return totalConsumption;
}

// Function object per il predicato del find_if nel metodo successivo
class overConsumption
{
    public:
        bool operator() (const std::shared_ptr<DomoticDevice>& device) const
        {
            return device.isOn();
        }
}

// Gestisce situazioni di sovraccarico spegnendo i dispositivi in ordine inverso.
void DomoticSystem::handleOverConsumption ()
{
    while (calcultedCurrentConsumption() > (maxPowerConsumption + photovoltaicProduction))
    {
        auto it = std::find_if(devices.rbegin(), devices.rend(), overConsumption());
        // rbegin e rend (reverse) servono per cercare in senso invertito, dato che quando si spegne un dispositivo si parte dall'ultimo acceso

        if (it != devices.rend())
        {
            *it.turnOff();
        }
        else
        {
            throw std::runtime_error("All devices are off");
        }
    }
}

// Aggiunge un dispositivo alla lista gestita.
void DomoticSystem::addDevice(const std::shared_ptr<DomoticDevice>& device)
{
    devices.push_back(device);
}

// function object predicato per il find_if nel metodo successivo
class idIsPresent
{
    int ID;
    public:
        idIsPresent(int id) : ID{id} {}

        bool operator()(const std::shared_ptr<DomoticDevice> &device) const
        {
            return device.getID() == ID;
        }
}

// Rimuove un dispositivo dalla lista tramite il suo ID.
void DomoticSystem::removeDevice (int id)
{
    auto it = std::find_if(devices.begin(), devices.end(), idIsPresent(id));
    if (it != devices.end())
    {
        devices.erase(it);
    }
    else
    {
        throw std::runtime_error("Device not found");
    }
}

// Imposta la produzione fotovoltaica.
void DomoticSystem::setPhotovoltaicProduction (double production)
{
    photoVoltaicProduction = production;
}

// Esegue un comando dato come input.
void DomoticSystem::executeCommand (const std::string& command)
{

}

// Mostra lo stato attuale del sistema.
void DomoticSystem::displaySystemStatus () const
{
    for (const auto& device : devices)
    {
        std::cout << device.getStatus() << std::endl;
    }
    std::cout << "Current total consumption: " << calculateCurrentConsumption() << " kW" << std::endl;
}

// Registra un evento in un log.
void DomoticSystem::logEvent(const std::string& event) const
{
    static const std::string logFileName = "domoticSystem.txt";

    std::ofstream logFile(logFileName, std::ios_base::app);
}