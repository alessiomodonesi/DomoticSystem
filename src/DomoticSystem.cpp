// @author Tommaso Silvestrin

#include <sstream>

#include "DomoticSystem.h"
#include "FixedCycleDevice.h"

// Costruttore: inizializza il sistema con un limite massimo di potenza.
DomoticSystem::DomoticSystem(double powerConsumption)
    : maxPowerConsumption_{powerConsumption}, dailySystemConsumption_{0}
{
    if (maxPowerConsumption_ < 0.5 || maxPowerConsumption_ > 6.0)
        throw std::invalid_argument("maxPowerConsumption dev'essere [0.5 kW, 6.0 kW]");

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
            std::cerr << "L'overconsumption non può essere risolta: tutti i dispositivi sono spenti" << std::endl;
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
        std::cerr << "Dispositivo non trovato" << std::endl;
}

// Mostra la lista di tutti i dispositivi.
std::ostream &operator<<(std::ostream &os, const std::vector<std::unique_ptr<DomoticDevice>> &devices_)
{
    double dailySystemProduction = 0.0;
    double dailySystemConsumption = 0.0;

    for (const auto &device : devices_)
    {
        if (device->getPowerConsumption() > 0)
            dailySystemProduction += device->getDailyConsumption(); // Somma la produzione energetica
        else
            dailySystemConsumption += device->getDailyConsumption(); // Somma il consumo energetico
    }

    os << "Attualmente il sistema ha prodotto " << dailySystemProduction << " kWh e consumato " << dailySystemProduction << " kWh\n"
       << "Nello specifico:" << "\n\n";

    for (const auto &device : devices_)
        os << *device << "\n"; // Usa l'overload di << per DomoticDevice
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
                // Converte l'orario fornito in un oggetto Time.
                Time targetTime = Time::toTime(params[1]);

                // Controlla se l'orario fornito è successivo all'orario attuale.
                if (targetTime > NOW)
                {
                    std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;

                    for (int h = NOW.getHours(); h <= targetTime.getHours(); ++h)
                    {
                        int startMinute = (h == NOW.getHours()) ? NOW.getMinutes() : 0;
                        int endMinute = (h == targetTime.getHours()) ? targetTime.getMinutes() : 59;

                        for (int m = startMinute; m <= endMinute; ++m)
                        {
                            NOW.setTime(h, m); // Aggiorna l'orario.

                            // Controlla lo stato di ogni dispositivo e aggiorna in base all'orario corrente
                            for (const auto &device : devices_)
                            {
                                // Controlla se il dispositivo è acceso
                                if (device->isDeviceOn())
                                {
                                    // Spegni il dispositivo se l'orario attuale corrisponde a "offTime"
                                    if (device->getOffTime().getHours() == h && device->getOffTime().getMinutes() == m)
                                    {
                                        device->turnOff();
                                        std::cout << "[" << NOW << "] Il dispositivo " << device->getName() << " si è spento" << std::endl;
                                    }
                                }
                                else
                                {
                                    // Accendi il dispositivo se l'orario attuale corrisponde a "startTime"
                                    if (device->getStartTime().getHours() == h && device->getStartTime().getMinutes() == m)
                                    {
                                        device->turnOn();
                                        std::cout << "[" << NOW << "] Il dispositivo " << device->getName() << " si è acceso" << std::endl;
                                    }
                                }
                            }

                            // Stampa finale alla fine del ciclo per l'ultimo minuto
                            if (h == targetTime.getHours() && m == endMinute)
                                std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                        }
                    }
                }
                else
                    std::cerr << "[" << NOW << "] Errore: l'orario " << targetTime
                              << " non è successivo all'orario attuale" << std::endl;
            }
            else
            {
                auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
                if (it != this->devices_.end()) // Se trovo il device.
                {
                    DomoticDevice *device = it->get();
                    if (params[1] == "on") // set ${DEVICENAME} on, accende il dispositivo
                    {
                        std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                        device->turnOn();
                        std::cout << "[" << NOW << "] Il dispositivo " << device->getName() << " si è acceso" << std::endl;
                    }
                    else if (params[1] == "off") // set ${DEVICENAME} off, spegne il dispositivo
                    {
                        std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                        device->turnOff();
                        std::cout << "[" << NOW << "] Il dispositivo " << device->getName() << " si è spento" << std::endl;
                    }
                    else if (Time::isTime(params[1])) // set ${DEVICENAME} ${START}, imposta l’orario di accensione per un dispositivo.
                    {
                        if (FixedCycleDevice *fixedDevice = dynamic_cast<FixedCycleDevice *>(device))
                        {
                            std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                            fixedDevice->setTimer(Time::toTime(params[1]));
                            std::cout << "[" << NOW << "] Impostato un timer per il dispositivo " << device->getName() << " dalle " << Time::toTime(params[1]) << std::endl;
                        }
                        else
                        {
                            std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                            device->setTimer(Time::toTime(params[1]), Time(-1, -1)); // Notare che il terzo parametro è opzionale e solo usato per device manuali.
                            std::cout << "[" << NOW << "] Impostato un timer per il dispositivo " << device->getName() << " dalle " << Time::toTime(params[1]) << std::endl;
                        }
                    }
                    else
                        std::cerr << "Comando non trovato" << std::endl;
                }
                else
                    std::cerr << "Dispositivo non trovato" << std::endl;
            }
        }
        else if (params.size() == 3) // set ${DEVICENAME} ${START} [${STOP}], imposta l’orario di accensione e spegnimento per un DomoticDevice.
        {
            auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
            if (it != this->devices_.end()) // Se trovo il device.
            {
                DomoticDevice *device = it->get();
                if (FixedCycleDevice *fixedDevice = dynamic_cast<FixedCycleDevice *>(device))
                    std::cerr << "Comando non valido per il dispositivo " << fixedDevice->getName() << std::endl;
                else
                {
                    std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                    device->setTimer(Time::toTime(params[1]), Time::toTime(params[2]));
                    std::cout << "[" << NOW << "] Impostato un timer per il dispositivo "
                              << device->getName() << " dalle " << Time::toTime(params[1]) << " alle " << Time::toTime(params[2]) << std::endl;
                }
            }
            else
                std::cerr << "Dispositivo non trovato" << std::endl;
        }
        else
            std::cerr << "Comando non trovato" << std::endl;
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
                std::cerr << "Dispositivo non trovato" << std::endl;
        }
        else
            std::cerr << "Comando non trovato" << std::endl;
    };

    this->commands_["show"] = [this](const std::vector<std::string> &params)
    {
        if (params.size() == 0) // show, mostra la lista di tutti i dispositivi con la produzione/consumo energetico di ciascuno e la produzione/consumo energetico totale del sistema dalle 00:00.
        {
            std::cout << "[" << NOW << "] L'orario attuale è " << NOW << "\n"
                      << std::endl;
            std::cout << this->devices_ << std::endl;
        }
        else if (params.size() == 1) // show ${DEVICENAME}, mostra a schermo produzione/consumo energetico di uno specifico dispositivo.
        {
            auto it = std::find_if(this->devices_.begin(), this->devices_.end(), idIsPresent(std::hash<std::string>{}(params[0])));
            if (it != this->devices_.end()) // Se trovo il device.
            {
                std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                DomoticDevice *device = it->get();

                if (device->getPowerConsumption() > 0)
                    std::cout << "[" << NOW << "] Il dispositivo " << device->getName() << " oggi ha prodotto "
                              << device->getDailyConsumption() << " kWh" << std::endl;
                else
                    std::cout << "[" << NOW << "] Il dispositivo " << device->getName() << " oggi ha consumato "
                              << device->getDailyConsumption() << " kWh" << std::endl;
            }
            else
                std::cerr << "Dispositivo non trovato" << std::endl;
        }
        else
            std::cerr << "Comando non trovato" << std::endl;
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
                std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                std::cout << "[" << NOW << "] Tutti i timer sono stati rimossi" << std::endl;
                std::cout << "[" << NOW << "] Tutti i dispositivi sono stati ripristinati" << std::endl;
            }
            else if (params[0] == "time")
            {
                /*  reset time:
                    resetta il tempo del sistema, riportandolo all’orario 00:00.
                    Riporta tutti i dispositivi alle condizioni iniziali.
                    Gli eventuali timer aggiunti dopo l’avvio del sistema vengono mantenuti.
                */
                resetTime();
                std::cout << "[" << NOW << "] L'orario attuale è " << NOW << std::endl;
                std::cout << "[" << NOW << "] Tutti i dispositivi sono stati ripristinati" << std::endl;
            }
            else if (params[0] == "timers")
            {
                /*  reset timers:
                    Rimuove i timer di tutti i dispositivi.
                    Tutti i dispositivi rimangono nel loro stato attuale.
                */
                std::cout << "[" << NOW << "] Tutti i timer sono stati rimossi" << std::endl;
                resetTimers();
            }
            else
                std::cerr << "Comando non trovato" << std::endl;
        }
        else
            std::cerr << "Comando non trovato" << std::endl;
    };
    this->commands_["clear"] = [this](const std::vector<std::string> &params)
    {
        if (system("clear") != 0)
            std::cerr << "Warning: il comando 'clear' non è stato eseguito correttamente" << std::endl;
    };
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
            std::cerr << "Errore durante l'esecuzione del comando: " << e.what() << std::endl;
        }
    }
    else // Comando non trovato
        std::cerr << "Comando non trovato: " << command << std::endl;
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