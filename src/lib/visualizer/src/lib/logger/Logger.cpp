//============================================================================
// Name        : Logger.cpp
// Description : Static helper class for logging 
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 03/05/2022
// Version     : 1.0
//============================================================================

#include "Logger.h"

#include <cmath>
#include "../../objects/crossroad/CrossRoadObject.h"
#include "../../objects/entities/busStop/BusStopEntity.h"
#include "../../objects/entities/light/LightEntity.h"
#include "../../objects/entities/vehicle/VehicleEntity.h"
#include "../../objects/road/RoadObject.h"
#include "../nlohmann-json/json.hpp"
#include "../utils/Id.h"
#include "../xml-validator/Validator.h"

void Logger::logAsJson(const Simulation &sim, std::ostream &outStream) {
    REQUIRE(!sim.getRoads().empty(), "Simulation roads should not be empty");
    REQUIRE(sim.properlyInitialized(), "Simulation is properly initialized");
    // create json array for roads
    nlohmann::json roadsJson = nlohmann::json::array();
    for (const std::pair<const id, RoadObject> &roadPair : sim.getRoads()) {
        // Create json array for cars
        nlohmann::json carsJson = nlohmann::json::array();
        // For all cars
        for (const std::pair<const id, VehicleEntity> &vehiclePair : sim.getVehicles()) {
            std::pair<id, double> temp = sim.getWorld().at(vehiclePair.first);
            // If car is not on current road, ignore
            if (temp.first != roadPair.second.getId()) continue;
            // Else create car object and append to car array

            // Transelate string to type
            std::string carType;
            EVehicleEntityTypes type = vehiclePair.second.getType();

            if (type == EVehicleEntityTypes::kCar) {
                carType = "car";
            } else if (type == EVehicleEntityTypes::kBus) {
                carType = "bus";
            } else if (type == EVehicleEntityTypes::kFireTruck) {
                carType = "firetruck";
            } else if (type == EVehicleEntityTypes::kAmbulance) {
                carType = "ambulance";
            } else if (type == EVehicleEntityTypes::kPoliceCruiser) {
                carType = "police_cruiser";
            }

            nlohmann::json carJson = nlohmann::json::object({{"x", temp.second}, {"type", carType}});
            carsJson.insert(carsJson.end(), carJson);
        }

        // Create json array for lights
        nlohmann::json lightsJson = nlohmann::json::array();
        // For all lights
        for (const std::pair<const id, LightEntity> &lightPair : sim.getLights()) {
            std::pair<id, double> temp = sim.getWorld().at(lightPair.first);
            // If light is not on current road, ignore
            if (temp.first != roadPair.second.getId()) continue;
            // Else create light object and append to light array
            nlohmann::json lightJson = nlohmann::json::object({{"x", sim.getWorld().at(lightPair.first).second},
                                                               // FIXME remove placeholder
                                                               {"green", (lightPair.second.isGreen()) ? 1 : 0},
                                                               {"xs", 50},
                                                               {"xs0", 15}});
            lightsJson.insert(lightsJson.end(), lightJson);
        }

        // FIXME: temp fix for busstops: hijacked the lights logic
        // For all busstops
        for (const std::pair<const id, BusStopEntity> &busStopPair : sim.getBusstops()) {
            std::pair<id, double> temp = sim.getWorld().at(busStopPair.first);
            // If busstop is not on current road, ignore
            if (temp.first != roadPair.second.getId()) continue;
            // Else create light object and append to light array
            nlohmann::json lightJson = nlohmann::json::object({{"x", sim.getWorld().at(busStopPair.first).second},
                                                               // FIXME remove placeholder
                                                               {"green", 0}});
            lightsJson.insert(lightsJson.end(), lightJson);
        }

        // FIXME: temp fix for crossroads: hijacked the lights logic
        // For all crossRoads
        for (const std::pair<const id, CrossRoadObject> &crossRoadPair : sim.getCrossRoads()) {
            std::pair<id, double> temp = sim.getWorld().at(crossRoadPair.first);
            // If busstop is not on current road, ignore
            if (temp.first != roadPair.second.getId()) continue;
            // Else create light object and append to light array
            nlohmann::json lightJson = nlohmann::json::object({{"x", sim.getWorld().at(crossRoadPair.first).second},
                                                               // FIXME remove placeholder
                                                               {"green", 1}});
            lightsJson.insert(lightsJson.end(), lightJson);
        }

        // Insert cars and light arrays, road name and length into road object
        nlohmann::json roadJson = nlohmann::json::object({{"name", roadPair.second.getName()},
                                                          {"length", roadPair.second.getLength()},
                                                          {"cars", carsJson},
                                                          {"lights", lightsJson}});
        // Insert road object into roads array
        roadsJson.insert(roadsJson.end(), roadJson);
    }
    // Place roads and current timestamp in frame object
    nlohmann::json frame = nlohmann::json::object({{"roads", roadsJson}, {"time", sim.getIteration() * (1.0 / 60.0)}});

    // Output finished json frame to ostream.
    outStream << frame.dump() << "\n";
}

void Logger::logSimpleOutput(const Simulation &sim, std::ostream &outStream) {
    REQUIRE(!sim.getRoads().empty(), "Simulation roads should not be empty");
    REQUIRE(sim.properlyInitialized(), "Simulation is properly initialized");
    // FIXME: make stepSize constant for all classes not separated
    outStream << "Time: " << sim.getIteration() * (1.0 / 60.0) << 's' << "\n";

    unsigned int counter = 0;
    for (const std::pair<const id, VehicleEntity> &vehiclePair : sim.getVehicles()) {
        counter++;

        const double vehiclePosition = sim.getWorld().at({vehiclePair.second.getId()}).second;
        const std::string roadName = sim.roadIdToName(sim.getWorld().at(vehiclePair.second.getId()).first);
        const double velocity = vehiclePair.second.getVelocity();

        outStream << "Vehicle " << counter << "\n";
        outStream << "--> road: " << roadName << "\n";
        outStream << "--> position: " << vehiclePosition << "\n";
        outStream << "--> speed: " << velocity << "\n";

        outStream << "\n";
    }
}

void Logger::logAdvancedOutput(const Simulation &sim, std::ostream &outStream, int rowSize) {
    REQUIRE(!sim.getRoads().empty(), "Simulation roads should not be empty");
    REQUIRE(sim.properlyInitialized(), "Simulation is properly initialized");
    REQUIRE(rowSize >= -1, "row size is -1 or larger");

    // calculate size of a char
    double charSize;
    if (rowSize == -1) {
        double currentLargest = 0;
        // get size of the largest road
        for (std::unordered_map<id, RoadObject>::const_iterator it = sim.getRoads().begin(); it != sim.getRoads().end();
             ++it) {
            if (it->second.getLength() > currentLargest) { currentLargest = it->second.getLength(); }
        }
        rowSize = currentLargest;
        charSize = 1;
    } else {
        double currentLargest = 0;
        // get size of the largest road
        for (std::unordered_map<id, RoadObject>::const_iterator it = sim.getRoads().begin(); it != sim.getRoads().end();
             ++it) {
            if (it->second.getLength() > currentLargest) { currentLargest = it->second.getLength(); }
        }
        charSize = currentLargest / rowSize;
    }

    for (const std::pair<const id, RoadObject> &roadPair : sim.getRoads()) {
        std::vector<char> l1(rowSize);
        const std::list<id> &vehiclesOnCurrentRoad = sim.getVehiclesOnRoads().at(roadPair.first);
        for (int i = 0; i < std::round(roadPair.second.getLength() / charSize); ++i) {
            // For all cars
            char vehicleChar = '=';  // last vehicle we find is this one;

            for (std::list<id>::const_iterator it = vehiclesOnCurrentRoad.begin(); it != vehiclesOnCurrentRoad.end();
                 ++it) {
                if (std::round(sim.getWorld().at(*it).second / charSize) == i) {
                    // show car on this location
                    vehicleChar = vehicleTypeToLetter(sim.getVehicles().at(*it).getType());
                }
            }

            l1[i] = vehicleChar;
        }

        std::vector<char> l3(rowSize);
        const std::list<id> &busstopsOnCurrentRoad = sim.getBusstopsOnRoads().at(roadPair.first);
        for (int i = 0; i < std::round(roadPair.second.getLength() / charSize); i++) {
            // For all busstops
            bool shouldPush = false;
            for (std::list<id>::const_iterator it = busstopsOnCurrentRoad.begin(); it != busstopsOnCurrentRoad.end();
                 ++it) {
                if (std::round(sim.getWorld().at(*it).second / charSize) == i) {
                    // show bus (B) on this location
                    shouldPush = true;
                }
            }

            if (shouldPush)
                l3[i] = 'B';
            else
                l3[i] = ' ';
        }

        std::vector<char> l2(rowSize);
        const std::list<id> &lightsOnCurrentRoad = sim.getLightsOnRoads().at(roadPair.first);
        for (int i = 0; i < std::round(roadPair.second.getLength() / charSize); i++) {
            // for all lights on this road
            char pushChar = ' ';

            for (std::list<id>::const_iterator it = lightsOnCurrentRoad.begin(); it != lightsOnCurrentRoad.end();
                 ++it) {
                // if light exists
                if (std::round(sim.getWorld().at(*it).second / charSize) == i) {
                    // place G for green light or R for red light
                    if (sim.getLights().at(*it).isGreen()) {
                        pushChar = 'B';
                    } else {
                        pushChar = 'R';
                    }
                }
            }

            l2[i] = pushChar;
            if (l2[i] == ' ' && l3[i] != ' ') { l2[i] = '|'; }
        }

        std::vector<std::string> outputCats;
        outputCats.push_back(roadPair.second.getName());
        outputCats.push_back(" > verkeerslichten");
        outputCats.push_back(" > bushaltes");

        unsigned int longest = 0;

        for (unsigned int i = 1; i < outputCats.size(); ++i) {
            if (outputCats[i].length() > outputCats[longest].length()) { longest = i; }
        }

        longest = outputCats[longest].length();

        unsigned int spacesToAdd;

        spacesToAdd = longest - outputCats[0].length() + 2;
        outStream << outputCats[0] << std::string(spacesToAdd, ' ') << '|' << ' ';
        for (std::vector<char>::const_iterator it = l1.begin(); it != l1.end(); ++it) {
            if (*it == '\0') break;
            outStream << *it;
        }
        outStream << '\n';

        spacesToAdd = longest - outputCats[1].length() + 2;
        outStream << outputCats[1] << std::string(spacesToAdd, ' ') << '|' << ' ';
        for (std::vector<char>::const_iterator it = l2.begin(); it != l2.end(); ++it) {
            if (*it == '\0') break;
            outStream << *it;
        }
        outStream << '\n';

        spacesToAdd = longest - outputCats[2].length() + 2;
        outStream << outputCats[2] << std::string(spacesToAdd, ' ') << '|' << ' ';
        for (std::vector<char>::const_iterator it = l3.begin(); it != l3.end(); ++it) {
            if (*it == '\0') break;
            outStream << *it;
        }
        outStream << '\n';
        outStream << '\n';
    }

    outStream << '\n';
}

char Logger::vehicleTypeToLetter(EVehicleEntityTypes type) {
    switch (type) {
        case kCar:
            return 'A';
            break;

        case kBus:
            return 'B';
            break;

        case kFireTruck:
            return 'V';
            break;

        case kAmbulance:
            return 'Z';
            break;

        case kPoliceCruiser:
            return 'P';
            break;

        default:
            throw std::runtime_error("Did you break it?");
    }
}

unsigned int Logger::getCharPosition(const double position, const double charSize) {
    return std::round(position / charSize);
}