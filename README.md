## Membri del gruppo

- [Modonesi Alessio](https://github.com/alessiomodonesi)
- [Popusoi David](https://github.com/davidpopusoi)
- [Silvestrin Tommaso](https://github.com/tommasosilvestrin)

Cosa ha sviluppato **Modonesi Alessio**: class DomoticSystem, main.cpp.

Cosa ha sviluppato **Popusoi David**: class FixedCycleDevice, struct Time.

Cosa ha sviluppato **Silvestrin Tommaso**: class DomoticDevice, class Logger.

## Come compilare con Cmake
>
> ~/DomoticSystem/$ mkdir build
>
> ~/DomoticSystem/$ cd build
>
> ~/DomoticSystem/build/$ cmake ..
>
> ~/DomoticSystem/build/$ make
>
> ~/DomoticSystem/build/$ ./main

## Class Headers

- [DomoticSystem.h](./include/DomoticSystem.h)
- [DomoticDevice.h](./include/DomoticDevice.h)
- [FixedCycleDevice.h](./include/FixedCycleDevice.h)
- [Logger.h](./include/Logger.h)
- [Time.h](./include/Time.h)

## Class Implementation

- [DomoticSystem.cpp](./src/DomoticSystem.cpp)
- [DomoticDevice.cpp](./src/DomoticDevice.cpp)
- [FixedCycleDevice.cpp](./src/FixedCycleDevice.cpp)
- [Logger.cpp](./src/Logger.cpp)
- [Time.cpp](./src/Time.cpp)
- [main.cpp](./src/main.cpp)

## Execution Guide

Compilare il programma con la [CMakeLists](./CMakeLists.txt) presente nella directory principale (in alternativa usare [build.sh](./build.sh)).

Per l'esecuzione:

- Scrivere tra virgolette i nomi dei device composti da più parole (es. "Impianto fotovoltaico");

- La policy di spegnimento per overconsuming esclude il Frigorifero e seleziona per lo spegnimento il dispositivo con start time minore;

- Nel file [log.txt](./log.txt) vengono salvati comandi di input e risposte del programma;

- Sono presenti 3 file di log contenenti un esempio di esecuzione:
  - [ex1.txt](./doc/ex1.txt)
  - [ex2.txt](./doc/ex2.txt)
  - [ex3.txt](./doc/ex3.txt)

- Sono presenti inoltre i comandi "clear" e "exit" per semplificare l'utilizzo del programma.
