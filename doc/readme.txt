Per implementare il sistema domotico richiesto abbiamo deciso di implementare le seguenti classi
- DomoticSystem
- DomoticDevice
- FixedCiclyDevice
- Time
- Logger
- main

DomoticSystem:
- Questa classe gestisce i vari dispositivi, tutti i comandi che l'utente può inserire ed esegue tutti i
controlli richieste dalla consegna.
- Definisce un vector di unique_ptr ai vari dispositivi e una mappa contenente tutti i possibili comandi