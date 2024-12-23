#ifndef FIXEDCYCLEDEVICE_HPP
#define FIXEDCYCLEDEVICE_HPP

#include "DomoticDevice.hpp"
#include <chrono>

class FixedCycleDevice : public DomoticDevice
{
private:
    bool state;                    // True if on, false if off
    std::chrono::minutes duration; // Fixed cycle duration
    std::chrono::system_clock::time_point startTime;

public:
    FixedCycleDevice(const std::string &name, const std::string &id, double power, std::chrono::minutes duration);
    void turnOn() override;
    void turnOff() override;
    bool isOn() const override;
};

#endif // FIXEDCYCLEDEVICE_HPP