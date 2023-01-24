//============================================================================
// Name        : Simulation.cpp
// Description : Definition of the Simulation class
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/11
// Version     : 1.0
//============================================================================

#include "Simulation.h"

#include <iostream>
#include <random>
#include "lib/utils/Utils.h"

// ╔════════════════════════════════════════╗
// ║             Constructors               ║
// ╚════════════════════════════════════════╝

Simulation::Simulation(double stepSize, std::ostream &errStream) : stepSize(stepSize), _initCheck(this) {
    REQUIRE(stepSize > 0, "Stepsize cannot be negative or zero");
    ENSURE(properlyInitialized(), "Simulation is properly initialized");
}

Simulation::Simulation(std::istream &xmlStream, double stepSize, std::ostream &errStream) :
    stepSize(stepSize), _initCheck(this) {
    REQUIRE(stepSize > 0, "Stepsize cannot be negative or zero");

    // validate the xml and parse the returned valmap
    const Validator::ValMap valMap = Validator::validate(xmlStream, errStream);

    if (valMap.empty())
        throw std::runtime_error("[Simulation] Cannot start simulation because the xml stream is not "
                                 "valid or the xml was empty (see errorStream for validator output).");

    parse(valMap);

    ENSURE(properlyInitialized(), "Simulation is properly initialized");
}

// ╔════════════════════════════════════════╗
// ║        Public function members         ║
// ╚════════════════════════════════════════╝

void Simulation::parse(const Validator::ValMap &valMap) {
    REQUIRE(getRoads().empty() && getVehicles().empty() && getLights().empty() && getVehicleGenerators().empty() &&
              getRoadMap().empty() && getVehiclesOnRoads().empty() && getLightsOnRoads().empty(),
            "Simulation is empty");
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    REQUIRE(!valMap.empty(), "Valmap contains validated input");

    // Initialize roads first since other objects reference roads
    for (const Validator::Object &road : valMap.at(Validator::EObjectTypes::kRoad)) {
        const id currentId = idGen.next();
        // extract attributes
        const std::string name = road.at(Validator::EAttributes(Validator::ERoadAttributes::kName));
        const unsigned int length = std::stoi(road.at(Validator::EAttributes(Validator::ERoadAttributes::kLength)));

        // insert object into correct map
        roads.insert({currentId, RoadObject(currentId, name, length)});

        // keep a map of roadName mapped to id
        roadMap.insert({name, currentId});

        vehiclesOnRoads[currentId] = {};
        lightsOnRoads[currentId] = {};
        busStopsOnRoads[currentId] = {};
        crossRoadsOnRoads[currentId] = {};
    }

    for (const std::pair<Validator::EObjectTypes, std::vector<Validator::Object>> objects : valMap) {
        // get type of current object class
        const Validator::EObjectTypes type = objects.first;

        // skip the roads
        if (type == Validator::EObjectTypes::kRoad) { continue; }

        // go over all the objects in this class
        for (const Validator::Object &object : objects.second) {
            // generate id for object
            const id currentId = idGen.next();

            if (type == Validator::EObjectTypes::kLight) {
                const unsigned int lightCycle =
                  std::stoi(object.at(Validator::EAttributes(Validator::ELightAttributes::kCycleTime)));

                const unsigned int position =
                  std::stoi(object.at(Validator::EAttributes(Validator::ELightAttributes::kPosition)));

                const id roadId = roadMap.at(object.at(Validator::EAttributes(Validator::ELightAttributes::kRoadName)));

                spawnLight(roadId, position, lightCycle);

            } else if (type == Validator::EObjectTypes::kVehicle) {
                const unsigned int position =
                  std::stoi(object.at(Validator::EAttributes(Validator::EVehicleAttributes::kPosition)));

                const id roadId =
                  roadMap.at(object.at(Validator::EAttributes(Validator::EVehicleAttributes::kRoadName)));

                // convert string to EntityTypeEnum
                const EVehicleEntityTypes vehicleType = vehicleTypeStringToEnumVariant(
                  object.at(Validator::EAttributes(Validator::EVehicleAttributes::kType)));

                spawnVehicle(roadId, position, vehicleType);

            } else if (type == Validator::EObjectTypes::kVehicleGenerator) {
                const unsigned int generatorFrequency =
                  std::stoi(object.at(Validator::EAttributes(Validator::EVehicleGeneratorAttributes::kFrequency)));

                const id roadId =
                  roadMap.at(object.at(Validator::EAttributes(Validator::EVehicleGeneratorAttributes::kRoadName)));

                const EVehicleEntityTypes vehicleType = vehicleTypeStringToEnumVariant(
                  object.at(Validator::EAttributes(Validator::EVehicleGeneratorAttributes::kType)));

                vehicleGenerators.insert(
                  {currentId, VehicleGeneratorEntity(currentId, stepSize, generatorFrequency, vehicleType)});
                // FIXME 0 is a placeholder since at the moment vehicle generators can
                // only spawn here.
                world.insert({currentId, {roadId, 0}});

            } else if (type == Validator::EObjectTypes::kBusStop) {
                const id roadId =
                  roadMap.at(object.at(Validator::EAttributes(Validator::EBusStopAttributes::kRoadName)));

                const unsigned int position =
                  std::stoi(object.at(Validator::EAttributes(Validator::EBusStopAttributes::kPosition)));

                const unsigned int haltTime =
                  std::stoi(object.at(Validator::EAttributes(Validator::EBusStopAttributes::kHaltTime)));

                busstops.insert({currentId, BusStopEntity(currentId, haltTime, stepSize)});
                world.insert({currentId, {roadId, position}});

                busStopsOnRoads[roadId].push_back(currentId);

            } else if (type == Validator::EObjectTypes::kCrossRoad) {
                // get id of two roads
                const id roadIdOne =
                  roadMap.at(object.at(Validator::EAttributes(Validator::ECrossRoadAttributes::kFirstRoad)));
                const id roadIdTwo =
                  roadMap.at(object.at(Validator::EAttributes(Validator::ECrossRoadAttributes::kSecondRoad)));

                // get positions on both roads
                const unsigned int posFirstRoad =
                  std::stoi(object.at(Validator::EAttributes(Validator::ECrossRoadAttributes::kFirstRoadPosition)));
                const unsigned int posSecondRoad =
                  std::stoi(object.at(Validator::EAttributes(Validator::ECrossRoadAttributes::kSecondRoadPosition)));

                // get lightCycle time: -1 if no lights wanted
                const int lightCycle =
                  Utils::stoi(object.at(Validator::EAttributes(Validator::ECrossRoadAttributes::kLights)));

                if (lightCycle != -1) {
                    spawnLightCrossroad(roadIdOne, posFirstRoad, roadIdTwo, posSecondRoad, lightCycle);
                } else {
                    // generate second id for crossRoadCounterPart
                    const id crossRoadTwoId = idGen.next();

                    crossRoads.insert({currentId, CrossRoadObject(currentId, crossRoadTwoId)});
                    crossRoads.insert({crossRoadTwoId, CrossRoadObject(crossRoadTwoId, currentId)});

                    world.insert({currentId, {roadIdOne, posFirstRoad}});
                    world.insert({crossRoadTwoId, {roadIdTwo, posSecondRoad}});

                    crossRoadsOnRoads[roadIdOne].push_back(currentId);
                    crossRoadsOnRoads[roadIdTwo].push_back(crossRoadTwoId);
                }

            } else {
                throw std::runtime_error("[Simulation::parse()] this object type is unreachable (validator broken?).");
            }
        }
    }

    ENSURE(!getRoads().empty(), "Roads database cannot be empty");
}

void Simulation::godTick() {
    REQUIRE(!roads.empty(), "Simulation roads is not empty");
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");

    const unsigned int prevIteration = iteration;

    // go over all vehicles
    for (std::unordered_map<id, VehicleEntity>::iterator vehicleEntry = vehicles.begin();
         vehicleEntry != vehicles.end();) {
        // get the entry with position from world
        std::pair<id, double> &vehicleLocationEntry = world.at(vehicleEntry->first);

        // get id of vehicle in front
        const id vehicleInFront = getVehicleInFront(vehicleEntry->first);
        const id lightInFront = getLightInFront(vehicleEntry->first);

        // set default values for all parameters
        double distToLight = std::numeric_limits<double>::infinity(),
               distToVehicle = std::numeric_limits<double>::infinity(),
               velVehicleInFront = std::numeric_limits<double>::infinity(),
               distToBusStop = std::numeric_limits<double>::infinity();
        int busHaltTime = std::numeric_limits<int>::infinity();
        id busStopInFront = std::numeric_limits<id>::infinity();
        bool priorityVehicleInFront = false;

        if (lightInFront != vehicleEntry->first && !lights.at(lightInFront).isGreen()) {
            distToLight = world.at(lightInFront).second - world.at(vehicleEntry->first).second;
        }
        if (vehicleInFront != vehicleEntry->first) {
            distToVehicle = world.at(vehicleInFront).second - world.at(vehicleEntry->first).second -
                            vehicles.at(vehicleInFront).getLength();
            velVehicleInFront = vehicles.at(vehicleInFront).getVelocity();
            const EVehicleEntityTypes vehicleType = vehicles.at(vehicleInFront).getType();
            const VehicleEntity &vehicleInFrontObject = vehicles.at(vehicleInFront);

            if ((vehicleType == EVehicleEntityTypes::kAmbulance || vehicleType == EVehicleEntityTypes::kFireTruck ||
                vehicleType == EVehicleEntityTypes::kPoliceCruiser) &&
                  vehicleInFrontObject.getAcceleration() == vehicleInFrontObject.getMaxAcceleration()) {
                priorityVehicleInFront = true;
            }
        }
        if (vehicleEntry->second.getType() == EVehicleEntityTypes::kBus) {
            busStopInFront = getBusStopInFront(vehicleEntry->first);

            if (busStopInFront != vehicleEntry->first) {
                distToBusStop = world.at(busStopInFront).second - world.at(vehicleEntry->first).second;
                busHaltTime = busstops.at(busStopInFront).getHaltTime();
            }
        }

        const double oldPos = vehicleLocationEntry.second;

        // FIXME distances wrong when turned at crossroads.
        vehicleLocationEntry.second +=
          vehicleEntry->second.update(distToVehicle, velVehicleInFront, distToLight, distToBusStop, busHaltTime,
                                      busStopInFront, priorityVehicleInFront);

        const double newPos = vehicleLocationEntry.second;

        // crossroad logic

        std::vector<id> validCrossRoadIds;
        // make list of all crossroads in traversed range on road this tick
        for (std::list<id>::const_iterator crossRoadsId = crossRoadsOnRoads.at(vehicleLocationEntry.first).begin();
             crossRoadsId != crossRoadsOnRoads.at(vehicleLocationEntry.first).end(); ++crossRoadsId) {
            const double currentCrossRoadPos = world.at(*crossRoadsId).second;

            if (currentCrossRoadPos > oldPos && currentCrossRoadPos < newPos) {
                // crossroad in range
                validCrossRoadIds.push_back(*crossRoadsId);
            }
        }

        // Find closest crossroad to car
        if (!validCrossRoadIds.empty()) {
            id currentLargest = validCrossRoadIds[0];
            for (unsigned int i = 1; i < validCrossRoadIds.size(); ++i) {
                // FIXME: skipped all previous crossroads

                if (world.at(currentLargest).second < world.at(validCrossRoadIds[i]).second) {
                    currentLargest = validCrossRoadIds[i];
                }
            }

            // choose direction to proceed in
            // god damn std::rand() not providing enough entropy...
            // rand gen source: grepper
            std::random_device rd;   // obtain a random number from hardware
            std::mt19937 gen(rd());  // seed the generator
            std::uniform_int_distribution distr(0, 1);

            const int n = distr(gen);
            // 0 = turn, 1 = straight ahead
            if (n == 0) {
                const id otherCrossRoadId = crossRoads.at(currentLargest).getCounterPart();
                const id newRoadId = world.at(otherCrossRoadId).first;
                const double newVehiclePos = world.at(otherCrossRoadId).second;

                moveVehicle(newRoadId, newVehiclePos, vehicleEntry->first);
            }
        }

        // get the length of the road the vehicle is on
        const double roadLength = roads.at(vehicleLocationEntry.first).getLength();

        // delete vehicle if it is out of bound
        if (vehicleLocationEntry.second > roadLength) {
            const id vehicleId = vehicleEntry->first;

            // delete vehicle from object storage and from world entry
            vehicleEntry = vehicles.erase(vehicleEntry);
            deleteVehicle(vehicleId);

        } else {
            ++vehicleEntry;
        }
    }

    // got over all lights
    for (std::unordered_map<id, LightEntity>::iterator lightEntry = lights.begin(); lightEntry != lights.end();
         ++lightEntry) {
        lightEntry->second.update();
    }

    // for all vehicle generators
    for (std::unordered_map<id, VehicleGeneratorEntity>::iterator vehicleGeneratorEntity = vehicleGenerators.begin();
         vehicleGeneratorEntity != vehicleGenerators.end(); ++vehicleGeneratorEntity) {
        // update generator and check if vehicle spawn is needed
        const bool spawnNeeded = vehicleGeneratorEntity->second.update();

        if (!spawnNeeded) continue;

        const EVehicleEntityTypes vehicleType = vehicleGeneratorEntity->second.getVehicleType();

        // FIXME length hardcoded
        const id roadId = world.at(vehicleGeneratorEntity->second.getId()).first;

        const std::list<id> vehicleIds = vehiclesOnRoads.at(roadId);

        bool canSpawn = true;
        for (std::list<id>::const_iterator it = vehicleIds.begin(); it != vehicleIds.end(); ++it) {
            const double length = vehicles.at(*it).getLength();
            const double otherVehiclePosition = world.at(*it).second;

            if (otherVehiclePosition <= length * 2) {
                canSpawn = false;
                break;
            }
        }
        if (canSpawn)
            // spawn vehicle
            spawnVehicle(roadId, 0, vehicleType);
    }

    ++iteration;

    ENSURE(getIteration() == prevIteration + 1, "The simulation is ticked");
}

// private function members

// ╔════════════════════════════════════════╗
// ║                 Helpers                ║
// ╚════════════════════════════════════════╝

bool Simulation::spawnVehicle(const unsigned int roadId, const double position, const EVehicleEntityTypes type) {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    REQUIRE(getRoads().find(roadId) != getRoads().end(), "roadId is vlaid");
    REQUIRE(position >= 0, "position is positive or zero");

    const unsigned int roadLength = roads.at(roadId).getLength();

    // check if spawn position is within road length
    if (position > roadLength) { return false; }

    const unsigned int currentId = idGen.next();
    vehicles.insert({currentId, {currentId, stepSize, type}});

    world.insert({currentId, {roadId, position}});

    vehiclesOnRoads[roadId].push_back(currentId);
    return true;
}

bool Simulation::spawnLightCrossroad(const id roadIdOne, const double posOne, const id roadIdTwo, const double posTwo,
                                     const double cycle) {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    REQUIRE(getRoads().find(roadIdOne) != getRoads().end(), "roadIdOne is valid");
    REQUIRE(posOne >= 1, "position one is 1 or more");
    REQUIRE(getRoads().find(roadIdOne) != getRoads().end(), "roadIdTwo is valid");
    REQUIRE(posOne >= 1, "position two is 1 or more");

    // generate id's for crossroads
    id idOne = idGen.next();
    id idTwo = idGen.next();

    // insert crossroad and counterpart into simulation
    crossRoads.insert({idOne, CrossRoadObject(idOne, idTwo)});
    crossRoads.insert({idTwo, CrossRoadObject(idTwo, idOne)});

    world.insert({idOne, {roadIdOne, posOne}});
    world.insert({idTwo, {roadIdTwo, posTwo}});

    crossRoadsOnRoads[roadIdOne].push_back(idOne);
    crossRoadsOnRoads[roadIdTwo].push_back(idTwo);

    // insert "paired" lights one meter before each crossroad
    spawnLight(roadIdOne, posOne - 1, cycle);
    spawnLight(roadIdTwo, posTwo - 1, cycle, true);

    return true;
}

bool Simulation::spawnLight(const id road, const double pos, const int cycle, bool isGreen) {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    REQUIRE(getRoads().find(road) != getRoads().end(), "roadId is valid");
    REQUIRE(pos >= 0, "position is positive or zero");

    const id currentId = idGen.next();

    LightEntity light = LightEntity(currentId, cycle, stepSize, isGreen);

    lights.insert({currentId, light});

    world.insert({currentId, {road, pos}});
    lightsOnRoads[road].push_back(currentId);
    return true;
}

bool Simulation::moveVehicle(const id roadId, const double position, const id vehicleId) {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    REQUIRE(getRoads().find(roadId) != getRoads().end(), "roadId is vlaid");
    REQUIRE(getWorld().at(vehicleId).first != roadId, "road to move to is a different one");
    REQUIRE(position >= 0, "position is positive or zero");
    bool result = true;

    const id oldRoadId = world.at(vehicleId).first;

    // check if the vehicle will collide with existing vehicle
    for (std::list<id>::const_iterator otherVehicle = vehiclesOnRoads.at(roadId).begin();
         otherVehicle != vehiclesOnRoads.at(roadId).end(); ++otherVehicle) {
        if (world.at(*otherVehicle).second - vehicles.at(*otherVehicle).getLength() - position <
              vehicles.at(vehicleId).getLength() &&
            world.at(*otherVehicle).second >=
              position - (vehicles.at(vehicleId).getLength() + vehicles.at(*otherVehicle).getMinimalFollow())) {
            result = false;
        }
    }

    if (result) {
        world[vehicleId] = {roadId, position + vehicles.at(vehicleId).getLength()};
        std::list<id> &vehicleIds = vehiclesOnRoads[oldRoadId];

        for (std::list<id>::iterator it = vehicleIds.begin(); it != vehicleIds.end(); ++it) {
            if (*it == vehicleId) {
                vehicleIds.erase(it);
                break;
            }
        }

        vehiclesOnRoads[roadId].push_back(vehicleId);
    }

    ENSURE(getVehicles().find(vehicleId) != getVehicles().end(), "vehicle is present in the database");
    ENSURE(getWorld().find(vehicleId) != getWorld().end(), "vehicle is found in the world");

    return result;
}

bool Simulation::deleteVehicle(const id vehicleId) {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    REQUIRE(getWorld().find(vehicleId) != getWorld().end(), "vehicle is present in the world");

    const id roadId = world.at(vehicleId).first;
    world.erase(vehicleId);
    vehicles.erase(vehicleId);

    std::list<id> &vehicleIds = vehiclesOnRoads[roadId];
    vehicleIds.erase(std::remove(vehicleIds.begin(), vehicleIds.end(), vehicleId));

    return true;

    ENSURE(getVehicles().find(vehicleId) == getVehicles().end(), "vehicle is deleted from the database");
    ENSURE(getWorld().find(vehicleId) == getWorld().end(), "vehicle is deleted from the world");
}

id Simulation::getVehicleInFront(const id vehicleId) const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");

    const double currPosition = world.at(vehicleId).second;

    // get all vehicles on same road
    const id roadId = world.at(vehicleId).first;
    const std::list<id> &vehicleIds = vehiclesOnRoads.at(roadId);

    id vehicleInFront = vehicleId;
    double posVehicleInFront = INFINITY;
    for (std::list<id>::const_iterator it = vehicleIds.begin(); it != vehicleIds.end(); ++it) {
        if (*it == vehicleId) continue;

        const double otherPosition = world.at(*it).second;

        if (otherPosition > currPosition && posVehicleInFront > otherPosition) {
            vehicleInFront = *it;
            posVehicleInFront = otherPosition;
        }
    }
    return vehicleInFront;
}

id Simulation::getLightInFront(const id vehicleId) const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");

    const double currPosition = world.at(vehicleId).second;

    // get lights on the same road
    const id roadId = world.at(vehicleId).first;
    const std::list<id> &lightIds = lightsOnRoads.at(roadId);

    id lightInFront = vehicleId;
    // set position to infinity so the first light's position is always smaller
    double posLightInFront = INFINITY;
    // for all lights
    for (std::list<id>::const_iterator it = lightIds.begin(); it != lightIds.end(); ++it) {
        const double lightPos = world.at(*it).second;

        if (lightPos >= currPosition && posLightInFront > lightPos) {
            lightInFront = *it;
            posLightInFront = lightPos;
        }
    }

    return lightInFront;
}

id Simulation::getBusStopInFront(const id vehicleId) const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");

    const double currPosition = world.at(vehicleId).second;

    // get lights on the same road
    const id roadId = world.at(vehicleId).first;
    const std::list<id> &busStopIds = busStopsOnRoads.at(roadId);

    id busStopInFront = vehicleId;
    // set position to infinity so the first light's position is always smaller
    double posBusStopInFront = INFINITY;
    // for all lights
    for (std::list<id>::const_iterator it = busStopIds.begin(); it != busStopIds.end(); ++it) {
        const double busStopPos = world.at(*it).second;

        if (busStopPos >= currPosition && posBusStopInFront > busStopPos) {
            busStopInFront = *it;
            posBusStopInFront = busStopPos;
        }
    }
    return busStopInFront;
}

EVehicleEntityTypes Simulation::vehicleTypeStringToEnumVariant(const std::string &str) const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");

    EVehicleEntityTypes type;

    if (str == "auto") {
        type = EVehicleEntityTypes::kCar;
    } else if (str == "bus") {
        type = EVehicleEntityTypes::kBus;
    } else if (str == "brandweerwagen") {
        type = EVehicleEntityTypes::kFireTruck;
    } else if (str == "ziekenwagen") {
        type = EVehicleEntityTypes::kAmbulance;
    } else if (str == "politiecombi") {
        type = EVehicleEntityTypes::kPoliceCruiser;
    } else {
        throw std::runtime_error("[Simulation::vehicleTypeStringToEnumVariant] The type provided does not exist: " +
                                 str);
    }

    return type;
}

const std::string &Simulation::roadIdToName(id id) const { return roads.at(id).getName(); }

// ╔════════════════════════════════════════╗
// ║          Getters and setters           ║
// ╚════════════════════════════════════════╝

const std::unordered_map<id, RoadObject> &Simulation::getRoads() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return roads;
}

const std::unordered_map<std::string, id> &Simulation::getRoadMap() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return roadMap;
}

const std::unordered_map<id, std::list<id>> &Simulation::getVehiclesOnRoads() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return vehiclesOnRoads;
}

const std::unordered_map<id, std::list<id>> &Simulation::getLightsOnRoads() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return lightsOnRoads;
}

const std::unordered_map<id, std::list<id>> &Simulation::getBusstopsOnRoads() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return busStopsOnRoads;
}

const std::unordered_map<id, LightEntity> &Simulation::getLights() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return lights;
}

const std::unordered_map<id, VehicleEntity> &Simulation::getVehicles() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return vehicles;
}

const std::unordered_map<id, VehicleGeneratorEntity> &Simulation::getVehicleGenerators() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return vehicleGenerators;
}

const std::unordered_map<id, BusStopEntity> &Simulation::getBusstops() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return busstops;
}

const std::unordered_map<id, CrossRoadObject> &Simulation::getCrossRoads() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return crossRoads;
}

const std::unordered_map<id, std::pair<id, double>> &Simulation::getWorld() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return world;
}

const unsigned int &Simulation::getIteration() const {
    REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    return iteration;
}

// ╔════════════════════════════════════════╗
// ║               Contracts                ║
// ╚════════════════════════════════════════╝

bool Simulation::properlyInitialized() const { return _initCheck == this; }
