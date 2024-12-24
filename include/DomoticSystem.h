#ifndef DOMOTICSYSTEM_H
#define DOMOTICSYSTEM_H

#include <vector>
#include <memory>
#include <string>
#include "DomoticDevice.h"

class DomoticSystem
{
private:
    std::vector<std::shared_ptr<DomoticDevice>> devices;
    double maxPowerConsumption;    // kW
    double photovoltaicProduction; // kW

    double calculateCurrentConsumption() const;
    void handleOverConsumption();

public:
    DomoticSystem(double maxPowerConsumption);

    void addDevice(const std::shared_ptr<DomoticDevice> &device);
    void removeDevice(int id);
    void setPhotovoltaicProduction(double production);

    void executeCommand(const std::string &command);

    void displaySystemStatus() const;
    void logEvent(const std::string &event) const;
};

#endif // DOMOTICSYSTEM_H