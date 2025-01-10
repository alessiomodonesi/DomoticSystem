#include "DomoticSystem.h"
#include "FixedCycleDevice.h"

int main()
{
    DomoticSystem system;

    // Creazione di dispositivi e aggiunta al sistema.
    system.addDevice(std::unique_ptr<DomoticDevice>(new DomoticDevice("Impianto fotovoltaico", +1.5)));
    system.addDevice(std::unique_ptr<DomoticDevice>(new FixedCycleDevice("Lavatrice", -2.0, 110)));

    // Rimuovere un dispositivo.
    system.removeDevice(std::hash<std::string>{}("Lavatrice"));

    // Eseguire un comando.
    system.executeCommand("set Impianto fotovoltaico on");
    return 0;
}