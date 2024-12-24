#ifndef DOMOTICDEVICE_H
#define DOMOTICDEVICE_H

#include <string>

class DomoticDevice
{
protected:
    std::string name;
    int id;
    double powerConsumption; // Positivo per consumo, negativo per produzione
    bool isOn;

public:
    DomoticDevice(const std::string &name, int id, double powerConsumption);
    virtual ~DomoticDevice();

    virtual void turnOn();
    virtual void turnOff();
    virtual void setTimer(const std::string &startTime, const std::string &endTime = "");

    virtual double calculateEnergyConsumption(double hours) const;
    virtual std::string getStatus() const;

    const std::string &getName() const;
    int getId() const;
    double getPowerConsumption() const;
    bool isDeviceOn() const;
};

#endif // DOMOTICDEVICE_H