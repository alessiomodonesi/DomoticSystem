CLASSE PRINCIPALE system:   

CLASSE GENERALE device:
    - string name, int ID, double power (produzione/consumo di potenza all'ora), bool isOn
    - virtual void toggle (bool status): accensione o spegnimenti
    - virtual void total_power () const: consumo energetico
    - metodi setter, getter (?)

CLASSE DERIVATA disp. manuali: 
    - pair<string, string> timer
    - override di void toggle (bool status)
    - override di virtual void total_power () const
    - void set_timer (string start, string stop)

CLASSE DERIVATA disp. ciclo:
    - int cycle_duration
    - override di void toggle (bool status)
    - override di virtual void total_power () const