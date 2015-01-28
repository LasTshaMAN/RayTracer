#include "DataManager.h"

#include "jsoncons\json.hpp"
#include "l3ds\l3ds.h"

#include "objects\standart_objects\Rectangle.h"
#include "objects\standart_objects\Sphere.h"
#include "objects\standart_objects\Point.h"
#include "objects\standart_objects\Disk.h"
#include "objects\standart_objects\meshes\SimpleMesh.h"
#include "objects\standart_objects\meshes\AcceleratedMesh.h"

using namespace std;
using namespace jsoncons;

DataManager DataManager::instance = DataManager();

DataManager::DataManager() {
    loadStandartObjects();
    loadObjectInstances();
    loadLightSources();
}

DataManager::~DataManager() {
    for (auto standartObject = standartObjectsMap.begin(); standartObject != standartObjectsMap.end(); ++standartObject) {
        delete standartObject->second;
    }
    for (auto objectInstance = objectInstances.begin(); objectInstance != objectInstances.end(); ++objectInstance) {
        delete *objectInstance;
    }
    for (auto lightSource = lightSources.begin(); lightSource != lightSources.end(); ++lightSource) {
        delete *lightSource;
    }
}

const vector<ObjectInstance *>& DataManager::getObjectInstances() const {
    return objectInstances;
}

const vector<LightSource *>& DataManager::getLightSources() const {
    return lightSources;
}

void DataManager::loadStandartObjects() {
    string configFileName = "resources/configurations/StandartObjectsConfiguration.json";

    json configFile = json::parse_file(configFileName);

    for (size_t i = 0; i < configFile.size(); ++i) {
        try {
            json& standartObject = configFile[i];
            string name = standartObject["name"].as<string>();
            string className = standartObject["className"].as<string>();
            if (className == "Sphere") {
                standartObjectsMap[name] = new Sphere(standartObject);
            } else if (className == "Rectangle") {
                standartObjectsMap[name] = new Rectangle(standartObject);
            } else if (className == "Point") {
                standartObjectsMap[name] = new Point(standartObject);
            } else if (className == "Disk") {
                standartObjectsMap[name] = new Disk(standartObject);
            } else if (className == "SimpleMesh") {
                loadStandartSimpleMesh(standartObject);
            } else if (className == "AcceleratedMesh") {
                loadStandartAcceleratedMesh(standartObject);
            }

        } catch (const json_exception& e) {
            cerr << "Unable to read " + configFileName << endl;
            cerr << e.what() << endl;
        } catch (const exception& e) {
            cerr << "Unable to read " + configFileName << endl;
            cerr << e.what() << endl;
        }
    }
}

void DataManager::loadObjectInstances() {
    string configFileName = "resources/configurations/ObjectInstancesConfiguration.json";

    json configFile = json::parse_file(configFileName);

    for (size_t i = 0; i < configFile.size(); ++i) {
        try {
            json& objectInstance = configFile[i];
            string standartObjectName = objectInstance["standartObjectName"].as<string>();

            if (standartObjectsMap.find(standartObjectName) == standartObjectsMap.end()) {
                loadMesh(objectInstance);

            } else {
                string name = objectInstance["name"].as<string>();
                ObjectInstance * newObjectInstance = new ObjectInstance(objectInstance,
                    standartObjectsMap.at(standartObjectName));
                objectInstancesMap[name] = newObjectInstance;
                objectInstances.push_back(newObjectInstance);
            }

        } catch (const json_exception& e) {
            cerr << "Unable to read " + configFileName << endl;
            cerr << e.what() << endl;
        } catch (const exception& e) {
            cerr << "Unable to read " + configFileName << endl;
            cerr << e.what() << endl;
        }
    }
}

void DataManager::loadLightSources() {
    string configFileName = "resources/configurations/LightSourcesConfiguration.json";

    json configFile = json::parse_file(configFileName);

    for (size_t i = 0; i < configFile.size(); ++i) {
        try {
            json& lightSource = configFile[i];
            string geometricShape = lightSource["geometricShape"].as<string>();

            lightSources.push_back(new LightSource(lightSource, objectInstancesMap[geometricShape]));
        }
        catch (const json_exception& e) {
            cerr << "Unable to read " + configFileName << endl;
            cerr << e.what() << endl;
        }   catch (const exception& e) {
            cerr << "Unable to read " + configFileName << endl;
            cerr << e.what() << endl;
        }
    }
}

void DataManager::loadMesh(const jsoncons::json& objectInstance) {
    string standartObjectName = objectInstance["standartObjectName"].as<string>();
    
    int i = 0;
    while (standartObjectsMap.find(standartObjectName + "_" + to_string(i)) != standartObjectsMap.end()) {
        ObjectInstance * newObjectInstance = new ObjectInstance(objectInstance,
            standartObjectsMap.at(standartObjectName + "_" + to_string(i)));
        objectInstances.push_back(newObjectInstance);
        ++i;
    }
}

void DataManager::loadStandartSimpleMesh(const json& standartObject) {
    const string& name = standartObject["name"].as<string>();
    const string& meshFileName = standartObject["meshFileName"].as<string>();

    L3DS meshLoader;
    meshLoader.LoadFile(meshFileName.c_str());

    for (int i = 0; i < meshLoader.GetMeshCount(); ++i) {
        LMesh& currentMesh = meshLoader.GetMesh(i);

        standartObjectsMap[name + "_" + to_string(i)] = new SimpleMesh(currentMesh, meshLoader);
    }
}

void DataManager::loadStandartAcceleratedMesh(const json& standartObject) {
    const string& name = standartObject["name"].as<string>();
    const string& meshFileName = standartObject["meshFileName"].as<string>();

    L3DS meshLoader;
    meshLoader.LoadFile(meshFileName.c_str());

    for (int i = 0; i < meshLoader.GetMeshCount(); ++i) {
        LMesh& currentMesh = meshLoader.GetMesh(i);

        standartObjectsMap[name + "_" + to_string(i)] = new AcceleratedMesh(currentMesh, meshLoader);
    }
}

DataManager& DataManager::getDataManager() {
    return instance;
}