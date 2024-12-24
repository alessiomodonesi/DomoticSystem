#ifndef CYCLEDEVICE_H
#define CYCLEDEVICE_H

#include "DomoticDevice.h"

class CycleDevice : public DomoticDevice
{
private:
    double cycleDuration; // In ore
    std::string startTime;

public:
    CycleDevice(const std::string &name, int id, double powerConsumption, double cycleDuration);

    void setTimer(const std::string &startTime, const std::string &endTime = "") override;
    void stopCycle();

    double getCycleDuration() const;
    std::string getStartTime() const;
};

#endif // CYCLEDEVICE_H