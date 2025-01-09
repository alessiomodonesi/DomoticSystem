// @author Tommaso Silvestrin

#include <sstream>

#include "DomoticSystem.h"
#include "FixedCycleDevice.h"

// Costruttore: inizializza il sistema con un limite massimo di potenza.
DomoticSystem::DomoticSystem(double powerConsumption)
    : maxPowerConsumption_{powerConsumption}, dailySystemConsumption_{0}
{
    if (maxPowerConsumption_ < 0.5 || maxPowerConsumption_ > 6.0)
        throw std::invalid_argument("maxPowerConsumption must be [0.5 kW, 6.0 kW]");

    initializeCommands();
}

// Calcola il consumo corrente sommando i consumi di tutti i dispositivi accesi.
double DomoticSystem::calculateCurrentConsumption(void) const
{
    double totalConsumption = 0;
    for (const auto &device : this->devices_)
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
    while (calculateCurrentConsumption() > this->maxPowerConsumption_)
    {
        auto it = std::find_if(this->devices_.rbegin(), this->devices_.rend(), overConsumption());
        // rbegin e rend (reverse) servono per cercare in senso invertito, dato che quando si spegne un dispositivo si parte dall'ultimo acceso

        if (it != this->devices_.rend())
            (*it)->turnOff(); // Accedi all'oggetto puntato dal unique_ptr.
        else
            throw std::runtime_error("Overconsumption cannot be resolved: all devices are off.");
    }
}

// Aggiunge un dispositivo alla lista gestita.
void DomoticSystem::addDevice(std::unique_ptr<DomoticDevice> device)
{
    this->devices_.push_back(std::move(device));
}

// function object predicato per il find_if nel metodo successivo
class idIsPresent
{
    std::size_t ID;

public:
    idIsPresent(std::size_t id) : ID{id} {}

    bool operator()(std::unique_ptr<DomoticDevice> &device) const
    {
        return device->getId() == ID;
    }
};

// Rimuove un dispositivo dalla lista tramite il suo ID.
void DomoticSystem::removeDevice(std::size_t id)
{
    auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(id));
    if (it != this->devices_.end())
        this->devices_.erase(it);
    else
        throw std::runtime_error("device not found");
}

// Inizializza i comandi presenti nell'interfaccia utente.
void DomoticSystem::initializeCommands(void)
{

    this->commands_["set"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() == 2)
        {
            if (params[0] == "time") // Imposta il tempo del sistema, non di un device
            {
                // parse params[1], dovrebbe essere in formato HH:MM secondo l'esempio del prof

                // storedTime_.setTime(hours, minutes);
            }
            else
            {
                auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
                if (it != this->devices_.end()) // Trova il device
                {
                    DomoticDevice *device = it->get();
                    if (params[1] == "on")
                        device->turnOn();
                    else if (params[1] == "off")
                        device->turnOff();

                    else if (params[1] == "controlloFormattazioneTempo") // se invece e' specificato un tempo, controlla tipo di device
                    {

                        // DomoticDevice::setTimer() / FixedCycleDevice::setTimer()
                        // (notare che il terzo parametro e' opzionale e solo usato per device manuali)

                        if (FixedCycleDevice *fixedDevice = dynamic_cast<FixedCycleDevice *>(device))
                            fixedDevice->setTimer(device->getStartTime());
                        else
                            device->setTimer(device->getStartTime(), device->getOffTime());
                    }
                    else
                    {
                        // log errore
                    }
                }
                else
                    throw std::runtime_error("device not found");
            }
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

    this->commands_["rm"] = [this](const std::vector<std::string> &params)
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

    this->commands_["show"] = [this](const std::vector<std::string> &params)
    {
        // std::cout << *this;
    };

    this->commands_["reset"] = [this](const std::vector<std::string> &params)
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

// Esegue un comando dato come input.
void DomoticSystem::executeCommand(const std::string &input)
{
    std::string copy = input;
    std::transform(copy.begin(), copy.end(), copy.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    std::istringstream stream(copy);
    std::string command;
    std::getline(stream, command, ' '); // Prendo il primo termine della stringa input, il "comando"

    std::vector<std::string> params;
    std::string word;

    // Suddivido la seconda parte della stringa input, i parametri
    while (std::getline(stream, word, ' '))
    {
        params.push_back(word);
    }

    auto it = commands_.find(command);
    if (it != commands_.end())
    {
        it->second(params); // Passa i parametri alla funzione nella mappa comandi
    }
    else
    {
        // log errore
    }
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
    this->storedTime_.resetTime();
}

// Rimuove i timer di tutti i dispositivi.
void DomoticSystem::resetTimers(void)
{
    for (const auto &device : this->devices_)
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
    double dailySystemConsumption = 0.0;
    for (const auto &device : devices_)
    {
        // Usa l'overload di << per DomoticDevice
        os << *device << "\n";

        // Somma il consumo energetico
        dailySystemConsumption += device->getDailyConsumption();
    }

    os << "\nTotal System Energy Consumption: " << dailySystemConsumption << " kW\n";
    return os;
}