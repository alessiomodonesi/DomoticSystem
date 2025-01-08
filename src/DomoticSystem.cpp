// @author Tommaso Silvestrin

#include <sstream>

#include "DomoticSystem.h"
#include "FixedCycleDevice.h"

// Costruttore: inizializza il sistema con un limite massimo di potenza.
DomoticSystem::DomoticSystem(double powerConsumption)
    : maxPowerConsumption_{powerConsumption}, totalSystemEnergyConsumption_{0}
{
    if (maxPowerConsumption_ < 0.5 || maxPowerConsumption_ > 6.0)
        throw std::invalid_argument("maxPowerConsumption must be [0.5 kW, 6.0 kW]");

    initializeCommands();
}

// Inizializza i comandi presenti nell'interfaccia utente.
void DomoticSystem::initializeCommands(void)
{

    commands_["set"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() == 2)
        {
            if (params[0] == "time") // quando i comandi sono passati dovrebbero essere tutti messi in lower case
            {
                // parse params[1], dovrebbe essere in formato HH:MM secondo l'esempio del prof

                // storedTime_.setTime(hours, minutes);
            }

            // trova device

            // se on/off secondo parametro
            // DomoticDevice::turnOn() / DomoticDevice::turnOff()

            // se invece e' specificato un tempo, controlla tipo di device
            // DomoticDevice::setTimer() / FixedCycleDevice::setTimer()
            // (notare che il terzo parametro e' opzionale e solo usato per device manuali)
        }
        else if (params.size() >= 3)
        {
            // trova device e controlla che sia DomoticDevice, altrimenti ignora, dato che FixedCycleDevice e' gestito sopra
            // DomoticDevice::setTimer()
        }
        else
        {
            // log errore
        }
    };

    commands_["rm"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() > 0)
        {
            // trova device e controlla che sia FixedCycleDevice
            // FixedCycleDevice::stopCycle()
            // log
        }
        else
        {
            // log errore
        }
    };

    commands_["show"] = [this](const std::vector<std::string> &params)
    {
        // std::cout << *this;
    };

    commands_["reset"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() > 0)
        {
            if (params[0] == "timers") // quando i comandi sono passati dovrebbero essere tutti messi in lower case
            {
                resetTimers();
                // log
            }

            else if (params[0] == "all") // quando i comandi sono passati dovrebbero essere tutti messi in lower case
            {
                resetAll();
                // log
            }
            else
            {
                // log errore
            }
        }
        else
        {
            resetTime();
            // log
        }
    };
}

// Calcola il consumo corrente sommando i consumi di tutti i dispositivi accesi.
double DomoticSystem::calculateCurrentConsumption(void) const
{
    double totalConsumption = 0;
    for (const auto &device : devices_)
    {
        if (device->isDeviceOn())
            totalConsumption += device->getPowerConsumption();
    }
    return totalConsumption;
}

// Function object per il predicato del find_if nel metodo successivo
class overConsumption
{
public:
    bool operator()(const std::unique_ptr<DomoticDevice> &device) const
    {
        return device->isDeviceOn(); // Accede all'oggetto puntato senza copiarlo
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
            (*it)->turnOff(); // Accedi all'oggetto puntato dal unique_ptr.
        else
            throw std::runtime_error("Overconsumption cannot be resolved: all devices are off.");
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
void DomoticSystem::executeCommand(const std::string &input)
{
    std::istringstream stream(input);
    std::string command;
    std::getline(stream, command, ' '); //

    std::vector<std::string> params;
}

// Registra un evento in un log.
void DomoticSystem::logEvent(const std::string &event) const
{
    constexpr const char *fileName = "log.txt";
    std::ofstream logFile(fileName, std::ios_base::app);

    if (!logFile.is_open())
        throw std::runtime_error("Cannot open log file: " + std::string(fileName));

    logFile << event << '\n';
}

// COMANDI PER IL DEBUG

// Resetta il tempo del sistema.
void DomoticSystem::resetTime(void)
{
    storedTime_.resetTime();
}

// Rimuove i timer di tutti i dispositivi.
void DomoticSystem::resetTimers(void)
{
    for (const auto &device : devices_)
    {
        // Rimuove i timer dei device FixedCycle.
        if (auto fixedDevice = dynamic_cast<FixedCycleDevice *>(device.get()))
            fixedDevice->stopCycle();

        // Setta offTime_ a NULL per i device non FixedCycle.
        else
        {
            if (device->getOffTime() != Time(-1, -1))
                device->setOffTime(Time(-1, -1));
        }
    }
}

// Riporta il sistema alle condizioni iniziali.
std::ostream &operator<<(std::ostream &os, const std::vector<std::unique_ptr<DomoticDevice>> &devices_)
{
    double totalSystemEnergyConsumption = 0.0;

    for (const auto &device : devices_)
    {
        // Usa l'overload di << per DomoticDevice
        os << *device << "\n";

        // Somma il consumo energetico
        totalSystemEnergyConsumption += device->getTotalEnergyConsumption();
    }

    os << "Total System Energy Consumption: " << totalSystemEnergyConsumption << " kW\n";
    return os;
}