#ifndef MANUALDEVICE_HPP
#define MANUALDEVICE_HPP

#include "DomoticDevice.hpp"

class ManualDevice : public DomoticDevice
{
private:
    bool state; // True if on, false if off

public:
    ManualDevice(const std::string &name, const std::string &id, double power);
    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;
};

#endif // MANUALDEVICE_HPP