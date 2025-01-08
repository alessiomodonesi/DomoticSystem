// @author Alessio Modonesi

#include "Time.h"

// Costruttore
Time::Time(int h, int m)
{
    setTime(h, m);
}

// Va a una specifica ora del giorno.
void Time::setTime(int h, int m)
{
    if (h < 0 || h > 23)
        throw std::out_of_range("Le ore devono essere comprese tra 0 e 23.");

    if (m < 0 || m > 59)
        throw std::out_of_range("I minuti devono essere compresi tra 0 e 59.");

    hours_ = h;
    minutes_ = m;
}

// Resetta il tempo del sistema, riportandolo all’orario 00:00.
void Time::resetTime(void)
{
    setTime(0, 0);
}

// Restituisce un orario formattato.
Time Time::formattingTime(const Time &startTime, const int cycleDuration)
{
    int h = (int)cycleDuration / 60;
    int m = cycleDuration - (h * 60);

    h += Time::startTime.hour_();
    m += Time::startTime.minutes_();

    if (m > 59)
    {
        h += 1;
        m -= 60;
    }

    return new Time(h, m);
}

// HELPER FUNCTION

Time operator-(const Time &a, const Time &b)
{
    int minA = (a.getHours() * 60) + a.getMinutes();
    int minB = (b.getHours() * 60) + b.getMinutes();
    int min = minB - minA;
    int h = (int)min / 60;
    int m = min - (h * 60);
    return new Time(h, m);
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
        ;
    offsetM = "0";

    os << offsetH << obj.getHours() << ":" << offsetM << obj.getMinutes() << std::endl;
    return os;
}