// @author Alessio Modonesi

#include <iostream>
#include "Time.h"

// Orario corrente del sistema
Time NOW = {0, 0};

// Costruttore
Time::Time(int h, int m)
{
    setTime(h, m);
}

// Va a una specifica ora del giorno.
void Time::setTime(int h, int m)
{
    // Considera -1, -1 come un orario nullo
    if (h == -1 && m == -1)
    {
        hours_ = -1;
        minutes_ = -1;
        return;
    }

    // Controlla che ore e minuti siano validi
    if (h < 0 || h > 23)
    {
        // std::cerr << "DEBUG: Orario non valido (ore): h = " << h << std::endl;
        throw std::out_of_range("Le ore devono essere comprese tra 0 e 23.");
    }
    if (m < 0 || m > 59)
    {
        // std::cerr << "DEBUG: Orario non valido (minuti): m = " << m << std::endl;
        throw std::out_of_range("I minuti devono essere compresi tra 0 e 59.");
    }

    // Imposta ore e minuti
    hours_ = h;
    minutes_ = m;
}

// Resetta il tempo del sistema, riportandolo all’orario 00:00.
void Time::resetTime(void)
{
    setTime(0, 0);
}

// Calcola l'offTime di un FixedCycleDevice.
Time Time::calculateOffTime(const Time &startTime, const int cycleDuration)
{
    int h = (int)cycleDuration / 60;
    int m = cycleDuration - (h * 60);

    h += startTime.getHours();
    m += startTime.getMinutes();

    if (m > 59)
    {
        h += 1;
        m -= 60;
    }

    return Time(h, m);
}

// Controlla la formattazione di un orario.
bool Time::isTime(const std::string &time) // Time deve essere in formato HH:MM
{
    if (time.size() != 5 || time[2] != ':') // Controlla la lunghezza e il separatore
        return false;

    try
    {
        int h = std::stoi(time.substr(0, 2));
        int m = std::stoi(time.substr(3, 2));
        return (h >= 0 && h <= 23) && (m >= 0 && m <= 59);
    }
    catch (const std::exception &)
    {
        return false; // Restituisce false se std::stoi fallisce
    }
}

// Restituisce un orario formattato.
Time Time::toTime(const std::string &time) // Time deve essere in formato HH:MM
{
    // Verifica se l'input è un orario valido
    if (!Time::isTime(time))
        throw std::invalid_argument("Formato del tempo invalido: " + time);

    // Estrae le ore e i minuti
    int h = std::stoi(time.substr(0, 2));
    int m = std::stoi(time.substr(3, 2));
    return Time(h, m);
}

// HELPER FUNCTION
Time operator-(const Time &a, const Time &b)
{
    int h = a.getHours() - b.getHours();
    int m = a.getMinutes() - b.getMinutes();

    if (m < 0) // Scambio 1 ora con 60 minuti
    {
        m += 60;
        h -= 1;
    }

    return Time(h, m);
}

bool operator>(const Time &a, const Time &b)
{
    if (a.getHours() == b.getHours())
        return a.getMinutes() > b.getMinutes();
    else
        return a.getHours() > b.getHours();
}

bool operator>=(const Time &a, const Time &b)
{
    if (a.getHours() == b.getHours())
        return a.getMinutes() >= b.getMinutes();
    else
        return a.getHours() >= b.getHours();
}

bool operator<(const Time &a, const Time &b)
{
    if (a.getHours() == b.getHours())
        return a.getMinutes() < b.getMinutes();
    else
        return a.getHours() < b.getHours();
}

bool operator<=(const Time &a, const Time &b)
{
    if (a.getHours() == b.getHours())
        return a.getMinutes() <= b.getMinutes();
    else
        return a.getHours() <= b.getHours();
}

bool operator==(const Time &a, const Time &b)
{
    return (a.getHours() == b.getHours()) && (a.getMinutes() == b.getMinutes());
}

bool operator!=(const Time &a, const Time &b)
{
    return (a.getHours() != b.getHours()) || (a.getMinutes() != b.getMinutes());
}

std::ostream &operator<<(std::ostream &os, const Time &obj)
{
    std::string offsetH = "", offsetM = "";
    if (obj.getHours() < 10)
        offsetH = "0";
    if (obj.getMinutes() < 10)
        offsetM = "0";

    os << offsetH << obj.getHours() << ":" << offsetM << obj.getMinutes();
    return os;
}