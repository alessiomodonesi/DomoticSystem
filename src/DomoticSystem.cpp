#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "DomoticSystem.h"
#include "DomoticDevice.h"

DomoticSystem::DomoticSystem(double maxPowerConsumption)
    : maxPowerConsumption{maxPowerConsumption}, photovoltaicProduction{0} {}

double DomoticSystem::calculateCurrentConsumption () const
{
    double totalConsumption = 0;
    for (const auto& device : devices)
    {
        if(device.isOn())
        {
            totalConsumption += device.getPowerConsumption();
        }
    }
    return totalConsumption;
}

// function object per il predicato del find_if nel metodo successivo
class overConsumption
{
    public:
        bool operator() (const std::shared_ptr<DomoticDevice>& device) const
        {
            return device.isOn();
        }
}

void DomoticSystem::handleOverConsumption ()
{
    while (calcultedCurrentConsumption() > (maxPowerConsumption + photovoltaicProduction))
    {
        auto it = std::find_if(devices.rbegin(), devices.rend(), overConsumption());
        // la r prima di begin e end serve per cercare in senso inverso perché quando si spegne un dispositivo si parte dall'ultimo acceso

        if (it != devices.rend())
        {
            *it.turnOff();
        }
        else
        {
            throw std::runtime_error ("All devices are off");
        }
    }
}

void DomoticSystem::addDevice(const std::shared_ptr<DomoticDevice>& device)
{
    devices.push_back(device);
}

// function object predicato per il find_if nel metodo successivo
class idIsPresent
{
    int ID;
    public:
        idIsPresent(int id) : ID{id} {}

        bool operator()(const std::shared_ptr<DomoticDevice> &device) const
        {
            return device.getID() == ID;
        }
}

void DomoticSystem::removeDevice (int id)
{
    auto it = std::find_if(devices.begin(), devices.end(), idIsPresent(id));
    if (it != devices.end())
    {
        devices.erase(it);
    }
    else
    {
        throw std::runtime_error("Device not found");
    }
}

void DomoticSystem::setPhotovoltaicProduction (double production)
{
    photoVoltaicProduction = production;
}

void DomoticSystem::executeCommand (const std::string &command)
{

}

void DomoticSystem::displaySystemStatus () const
{
    for (const auto& device : devices)
    {
        std::cout << device.getStatus() << std::endl;
    }
    std::cout << "Current total consumption: " << calculateCurrentConsumption() << " kW" << std::endl;
}

void DomoticSystem::logEvent(const std::string &event) const
{

}