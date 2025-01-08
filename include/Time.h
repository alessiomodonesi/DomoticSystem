#ifndef TIME_H
#define TIME_H

#include <string>

struct Time {
    int hours_;
    int minutes_;

    // Costruttore
    Time(int h = 0, int m = 0);

    // Va a una specifica ora del giorno. 
    void setTime(int h, int m);

    // Resetta il tempo del sistema, riportandolo all’orario 00:00.
    void resetTime(void);

    // Restituisce un orario formattato.
    Time formattingTime(const Time &startTime, const int cycleDuration);

    // Getter per hour e minute.
    int getHours(void) const { return hours_; }  
    int getMinutes(void) const { return minutes_; }  
};

// HELPER FUNCTION
Time operator-(const Time &a, const Time &b);
bool operator==(const Time &a, const Time &b);
std::ostream &operator<<(std::ostringstream &os, const Time &obj);

#endif // TIME_H
