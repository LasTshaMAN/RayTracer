#ifndef __OBJECTS_MANAGER__
#define __OBJECTS_MANAGER__

#include <vector>
#include <unordered_map>
#include <string>

#include "objects\ObjectInstance.h"
#include "light_sources\LightSource.h"
#include "materials\Material.h"

class DataManager {
public:
    const std::vector<ObjectInstance *>& getObjectInstances() const;
    const std::vector<LightSource *>& getLightSources() const;

private:
    void loadStandartObjects();
    void loadObjectInstances();
    void loadLightSources();

    void loadMesh(const jsoncons::json& objectInstance);
    void loadStandartSimpleMesh(const jsoncons::json& standartObject);
    void loadStandartAcceleratedMesh(const jsoncons::json& standartObject);

private:
    std::vector<ObjectInstance *> objectInstances;
    std::vector<LightSource *> lightSources;

    std::unordered_map<std::string, StandartObject *> standartObjectsMap;
    std::unordered_map<std::string, ObjectInstance *> objectInstancesMap;

public:
    static DataManager& getDataManager();
private:
    DataManager();
    ~DataManager();

    static DataManager instance;
};

#endif