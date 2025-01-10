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
            std::cerr << "Overconsumption cannot be resolved: all devices are off." << std::endl;
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
        std::cerr << "Device not found" << std::endl;
}

// Mostra la lista di tutti i dispositivi.
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

    os << "Total System Energy Consumption: " << dailySystemConsumption << " kWh\n";
    return os;
}

// Inizializza i comandi presenti nell'interfaccia utente.
void DomoticSystem::initializeCommands(void)
{
    this->commands_["set"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() == 2)
        {
            if (params[0] == "time") // set time ${TIME}, va a una specifica ora del giorno.
            {
                std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                NOW.setTime(Time::toTime(params[1]).getHours(), Time::toTime(params[1]).getMinutes());
            }
            else
            {
                auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
                if (it != this->devices_.end()) // Se trovo il device.
                {
                    DomoticDevice *device = it->get();
                    if (params[1] == "on") // set ${DEVICENAME} on, accende il dispositivo
                    {
                        device->turnOn();
                        std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                        std::cout << "[" << NOW << "] Il dispositivo " << device->getName() << " si è acceso" << std::endl;
                    }
                    else if (params[1] == "off") // set ${DEVICENAME} off, spegne il dispositivo
                    {
                        device->turnOff();
                        std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                        std::cout << "[" << NOW << "] Il dispositivo " << device->getName() << " si è spento" << std::endl;
                    }
                    else if (Time::isTime(params[1])) // set ${DEVICENAME} ${START}, imposta l’orario di accensione per un FixedDomoticDevice.
                    {
                        // Notare che il terzo parametro è opzionale e solo usato per device manuali.
                        if (FixedCycleDevice *fixedDevice = dynamic_cast<FixedCycleDevice *>(device))
                        {
                            std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                            fixedDevice->setTimer(Time::toTime(params[1]));
                            std::cout << "[" << NOW << "] Impostato un timer per il dispositivo " << device->getName() << " dalle " << Time::toTime(params[1]) << std::endl;
                        }
                    }
                    else
                        std::cerr << "Invalid command" << std::endl;
                }
                else
                    std::cerr << "Device not found" << std::endl;
            }
        }
        else if (params.size() == 3) // set ${DEVICENAME} ${START} [${STOP}], imposta l’orario di accensione e spegnimento per un DomoticDevice.
        {
            auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
            if (it != this->devices_.end()) // Se trovo il device.
            {
                std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                DomoticDevice *device = it->get();
                device->setTimer(Time::toTime(params[1]), Time::toTime(params[2]));
                std::cout << "[" << NOW << "] Impostato un timer per il dispositivo "
                          << device->getName() << " dalle " << Time::toTime(params[1]) << " alle " << Time::toTime(params[2]) << std::endl;
            }
            else
                std::cerr << "Device not found" << std::endl;
        }
        else
            std::cerr << "Invalid command" << std::endl;
    };

    this->commands_["rm"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() == 1) // rm ${DEVICENAME}, rimuove il timer associato al dispositivo.
        {
            auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
            if (it != this->devices_.end()) // Se trovo il device.
            {
                std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                DomoticDevice *device = it->get();
                if (FixedCycleDevice *fixedDevice = dynamic_cast<FixedCycleDevice *>(device))
                {
                    fixedDevice->stopCycle();
                    std::cout << "[" << NOW << "] Rimosso il timer dal dispositivo " << device->getName() << std::endl;
                }
                else
                {
                    device->setOffTime(Time(-1, -1));
                    std::cout << "[" << NOW << "] Rimosso il timer dal dispositivo " << device->getName() << std::endl;
                }
            }
            else
                std::cerr << "Device not found" << std::endl;
        }
        else
            std::cerr << "Invalid command" << std::endl;
    };

    this->commands_["show"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() == 0) // show, mostra la lista di tutti i dispositivi con la produzione/consumo energetico di ciascuno e la produzione/consumo energetico totale del sistema dalle 00:00.
        {
            std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
            std::cout << this->devices_ << std::endl;
        }
        else if (params.size() == 1) // show ${DEVICENAME}, mostra a schermo produzione/consumo energetico di uno specifico dispositivo.
        {
            auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
            if (it != this->devices_.end()) // Se trovo il device.
            {
                std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                DomoticDevice *device = it->get();
                device->showCurrentEnergyConsumption(std::cout, *device);
            }
            else
                std::cerr << "Device not found" << std::endl;
        }
        else
            std::cerr << "Invalid command" << std::endl;
    };

    this->commands_["reset"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() == 1)
        {
            if (params[0] == "all")
            {
                /* reset all, riporta il sistema alle condizioni iniziali:
                    l’orario viene impostato a 00:00,
                    tutti i timer vengono rimossi,
                    tutti i dispositivi vengono spenti.
                */
                std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                resetAll();
            }
            else if (params[0] == "time")
            {
                /*  reset time:
                    resetta il tempo del sistema, riportandolo all’orario 00:00.
                    Riporta tutti i dispositivi alle condizioni iniziali.
                    Gli eventuali timer aggiunti dopo l’avvio del sistema vengono mantenuti.
                */
                std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                resetTime();
            }
            else if (params[0] == "timers")
            {
                /*  reset timers:
                    Rimuove i timer di tutti i dispositivi.
                    Tutti i dispositivi rimangono nel loro stato attuale.
                */
                std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                resetTimers();
            }
            else
                std::cerr << "Invalid command" << std::endl;
        }
        else
            std::cerr << "Invalid command" << std::endl;
    };
    this->commands_["clear"] = [this](const std::vector<std::string> &params)
    { system("clear"); };
    this->commands_["exit"] = [this](const std::vector<std::string> &params)
    { exit(0); };
};

// Esegue un comando dato come input.
void DomoticSystem::executeCommand(const std::string &input)
{
    std::string command;             // Comando principale (es. "set", "show")
    std::vector<std::string> params; // Parametri del comando

    // Usa un istringstream per analizzare la stringa di input
    std::istringstream stream(input);
    stream >> command; // Legge la prima parola come comando

    // Trasforma solo il comando in minuscolo
    std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c)
                   { return std::tolower(c); });

    // Raccoglie i parametri rispettando stringhe tra virgolette
    std::string param;
    while (stream >> std::ws) // Ignora spazi iniziali
    {
        if (stream.peek() == '"') // Riconosce stringhe tra virgolette
        {
            stream.ignore();                  // Ignora il carattere "
            std::getline(stream, param, '"'); // Legge fino alla chiusura delle virgolette
        }
        else
            stream >> param; // Legge un parametro "normale"

        params.push_back(param); // Aggiunge il parametro al vettore
    }

    // Cerca il comando nella mappa commands_
    auto it = commands_.find(command);
    if (it != commands_.end()) // Comando trovato
    {
        try
        {
            it->second(params); // Esegue la funzione associata al comando
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error during comand execution: " << e.what() << std::endl;
        }
    }
    else // Comando non trovato
        std::cerr << "Invalid command: " << command << std::endl;
}

// COMANDI PER IL DEBUG

// Resetta il tempo del sistema.
void DomoticSystem::resetTime(void)
{
    NOW = Time(0, 0);
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
void DomoticSystem::resetAll(void)
{
    this->resetTime();
    this->resetTimers();

    for (const auto &device : this->devices_)
        device->turnOff();
}