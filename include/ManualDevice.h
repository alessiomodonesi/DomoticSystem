#ifndef MANUALDEVICE_H
#define MANUALDEVICE_H

#include "DomoticDevice.h"

class ManualDevice : public DomoticDevice
{
private:
    std::string offTime;

public:
    ManualDevice(const std::string &name, int id, double powerConsumption);

    void setTimer(const std::string &startTime, const std::string &endTime) override;
    void removeTimer();

    std::string getOffTime() const;
};

#endif // MANUALDEVICE_H