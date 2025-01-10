#include <iostream>
#include <memory>

#include "DomoticSystem.h"
#include "DomoticDevice.h"
#include "FixedCycleDevice.h"

int main()
{
    // Creazione del sistema domotico
    DomoticSystem system(3.5); // 3.5 kW come soglia di potenza massima

    // Creazione ed aggiunta dei dispositivi al sistema domotico:
    // Impianto fotovoltaico
    system.addDevice(std::unique_ptr<DomoticDevice>(new DomoticDevice("Impianto fotovoltaico", 1.5)));

    // Dispositivi manuali (M)
    system.addDevice(std::unique_ptr<DomoticDevice>(new DomoticDevice("Pompa di Calore", -2.0)));
    system.addDevice(std::unique_ptr<DomoticDevice>(new DomoticDevice("Scaldabagno", -1.0)));
    system.addDevice(std::unique_ptr<DomoticDevice>(new DomoticDevice("Frigorifero", -0.4)));

    // Dispositivi a ciclo prefissato (CP)
    system.addDevice(std::unique_ptr<DomoticDevice>(new FixedCycleDevice("Lavatrice", -2.0, 110)));
    system.addDevice(std::unique_ptr<DomoticDevice>(new FixedCycleDevice("Lavastoviglie", -1.5, 195)));
    system.addDevice(std::unique_ptr<DomoticDevice>(new FixedCycleDevice("Tapparelle Elettriche", -0.3, 1)));
    system.addDevice(std::unique_ptr<DomoticDevice>(new FixedCycleDevice("Forno a Microonde", -0.8, 2)));
    system.addDevice(std::unique_ptr<DomoticDevice>(new FixedCycleDevice("Asciugatrice", -0.5, 60)));
    system.addDevice(std::unique_ptr<DomoticDevice>(new FixedCycleDevice("Televisore", -0.2, 60)));

    // Creazione dell'input per l'utente
    std::string input;
    while (true)
    {
        std::cout << "Inserisci un comando: ";
        std::getline(std::cin, input);
        system.executeCommand(input);
    }

    return 0;
}