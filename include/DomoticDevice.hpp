#ifndef DOMOTICDEVICE_HPP
#define DOMOTICDEVICE_HPP

#include <string>

class DomoticDevice
{
protected:
    std::string name;
    std::string id;
    double power; // Positive for production, negative for consumption

public:
    DomoticDevice(const std::string &name, const std::string &id, double power);
    virtual ~DomoticDevice() = default;

    const std::string &getName() const;
    const std::string &getId() const;
    double getPower() const;

    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual bool isOn() const = 0;
};

#endif // DOMOTICDEVICE_HPP