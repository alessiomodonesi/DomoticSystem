#include "DomoticSystem.h"

int main()
{
    // Creazione di dispositivi e aggiunta al sistema.
    DomoticSystem.addDevice(std::unique_ptr<DomoticDevice>(new DomoticDevice("Impianto fotovoltaico", +1.5)));
    DomoticSystem.addDevice(std::unique_ptr<DomoticDevice>(new FixedDevice("Lavatrice", -2.0, 110)));

    // Rimuovere un dispositivo.
    DomoticSystem.removeDevice(std::hash<std::string>{}("Lavatrice"));
    return 0;
}