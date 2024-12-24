#include "../include/DomoticSystem.h"
#include <vector>
#include <memory>
#include <string>
#include "DomoticDevice.h"

DomoticSystem::DomoticSystem(double maxPowerConsumption)
    : maxPowerConsumption{maxPowerConsumption}, photovoltaicProduction{0} {}

void DomoticSystem::addDevidce(const std::shared_ptr<DomoticDevice> &device)
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

double DomoticSystem::calculateCurrentConsumption () const
{
    double totalConsumption = 0;
    for (const auto& device : devices)
    {
        if(devide.isOn())
        {
            totalConsumption += device.getPowerConsumption();
        }
    }
    return totalConsumption;
}

class overConsumption
{
    public:
        bool operator() (const std::shared_ptr<DomoticDevice>& device) const
        {
            return device.isOn();
        }
}

void DomoticSystem::handleOverConsumption () // DA FINIRE
{
    while (calcultedCurrentConsumption() > (maxPowerConsumption + photovoltaicProduction))
    {
        auto it = std::find_if(devices.rbegin(), devices.rend(), overConsumption());

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
        std::cout << devide.getStatus() << std::endl;
    }
    std::cout << "Current total consumption: " << calculateCurrentConsumption() << " kW" << std::endl;
}

void DomoticSystem::logEvent(const std::string &event) const
{

}