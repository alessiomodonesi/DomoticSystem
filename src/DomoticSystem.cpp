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

// Inizializza i comandi presenti nell'interfaccia utente.
void DomoticSystem::initializeCommands(void)
{
    this->commands_["set"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() == 2)
        {
            if (params[0] == "time") // set time ${TIME}, va a una specifica ora del giorno.
            {
                // parse params[1], dovrebbe essere in formato HH:MM secondo l'esempio del prof
                // storedTime_.setTime(hours, minutes);
            }
            else
            {
                auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
                if (it != this->devices_.end()) // Se trovo il device.
                {
                    DomoticDevice *device = it->get();
                    if (params[1] == "on") // set ${DEVICENAME} on, accende il dispositivo
                        device->turnOn();
                    else if (params[1] == "off") // set ${DEVICENAME} off, spegne il dispositivo
                        device->turnOff();

                    else if (Time::isTime(params[1])) // set ${DEVICENAME} ${START}, imposta l’orario di accensione per un FixedDomoticDevice.
                    {
                        // Notare che il terzo parametro è opzionale e solo usato per device manuali.
                        if (FixedCycleDevice *fixedDevice = dynamic_cast<FixedCycleDevice *>(device))
                            fixedDevice->setTimer(Time::toTime(params[1]));
                    }
                    else
                    {
                        // params[1] non è { "on", "off", Time }.
                    }
                }
                else
                    throw std::runtime_error("device not found");
            }
        }
        else if (params.size() == 3) // set ${DEVICENAME} ${START} [${STOP}], imposta l’orario di accensione e spegnimento per un DomoticDevice.
        {
            auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
            if (it != this->devices_.end()) // Se trovo il device.
            {
                DomoticDevice *device = it->get();
                device->setTimer(Time::toTime(params[1]), Time::toTime(params[2]));
            }
            else
            {
                // Device non trovato.
            }
        }
        else
        {
            // Parametri errati nel comando.
        }
    };

    this->commands_["rm"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() == 1) // rm ${DEVICENAME}, rimuove il timer associato al dispositivo.
        {
            auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
            if (it != this->devices_.end()) // Se trovo il device.
            {
                DomoticDevice *device = it->get();
                if (FixedCycleDevice *fixedDevice = dynamic_cast<FixedCycleDevice *>(device))
                    fixedDevice->stopCycle();
                else
                    device->setOffTime(Time(-1, -1));
            }
            else
            {
                // Device non trovato.
            }
        }
        else
        {
            // Parametri errati nel comando.
        }
    };

    this->commands_["show"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() == 0) // show, mostra la lista di tutti i dispositivi con la produzione/consumo energetico di ciascuno e la produzione/consumo energetico totale del sistema dalle 00:00.
            std::cout << this->devices_ << std::endl;
        else if (params.size() == 1) // show ${DEVICENAME}, mostra a schermo produzione/consumo energetico di uno specifico dispositivo.
        {
            auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
            if (it != this->devices_.end()) // Se trovo il device.
            {
                DomoticDevice *device = it->get();
                std::cout << device->showCurrentEnergyConsumption(device->getStartTime()) << std::endl;
            }
            else
            {
                // Device non trovato.
            }
        }
        else
        {
            // Parametri errati nel comando.
        }
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
                resetAll();
            }
            else if (params[0] == "time")
            {
                /*  reset time:
                    resetta il tempo del sistema, riportandolo all’orario 00:00.
                    Riporta tutti i dispositivi alle condizioni iniziali.
                    Gli eventuali timer aggiunti dopo l’avvio del sistema vengono mantenuti.
                */
                resetTime();
            }
            else if (params[0] == "timers")
            {
                /*  reset timers:
                    Rimuove i timer di tutti i dispositivi.
                    Tutti i dispositivi rimangono nel loro stato attuale.
                */
                resetTimers();
            }
            else
            {
                // Parametri errati nel comando.
            }
        }
        else
        {
            // Parametri errati nel comando.
        }
    };
};

// Esegue un comando dato come input.
void DomoticSystem::executeCommand(const std::string &input)
{
    std::string copy = input;

    // Trasforma tutti i caratteri della stringa `copy` in minuscolo.
    std::transform(copy.begin(), copy.end(), copy.begin(), [](unsigned char c)
                   { return std::tolower(c); });

    // Utilizza un `std::istringstream` per analizzare la stringa trasformata `copy`.
    // Questo permette di dividere il comando principale dai suoi parametri.
    std::istringstream stream(copy);
    std::string command;

    // Estrae il primo termine della stringa (il comando) fino al primo spazio.
    // Esempio: "set device1 on" -> command = "set"
    std::getline(stream, command, ' ');

    std::vector<std::string> params; // Vettore per memorizzare i parametri del comando.
    std::string word;

    // Estrae tutti i termini rimanenti nella stringa (i parametri) separati da spazi.
    // Ogni parola successiva al comando principale viene aggiunta a `params`.
    // Esempio: "set device1 on" -> params = ["device1", "on"]
    while (std::getline(stream, word, ' '))
        params.push_back(word);

    // Cerca il comando nella mappa `commands_`, che associa comandi (stringhe)
    // a funzioni (lambdas) che implementano il loro comportamento.
    auto it = commands_.find(command);

    if (it != commands_.end()) // Se il comando è trovato, chiama la funzione associata con i parametri estratti.
        it->second(params);    // Esegue il comportamento del comando.
    else
    {
        // Comando non trovato.
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