//============================================================================
// Name        : Simulation.h
// Description : Simulation class that holds all the object in a simulation universe and defines the logic
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/11
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_SIMULATION_H
#define SE_PROJECT_SIMULATION_H

// third party
#include "lib/contract/Contract.h"
#include "lib/nlohmann-json/json.hpp"

// local types
#include "lib/utils/Id.h"
#include "lib/xml-validator/Validator.h"
#include "objects/crossroad/CrossRoadObject.h"
#include "objects/entities/busStop/BusStopEntity.h"
#include "objects/entities/light/LightEntity.h"
#include "objects/entities/vehicle/VehicleEntity.h"
#include "objects/entities/vehicleGenerator/VehicleGeneratorEntity.h"
#include "objects/road/RoadObject.h"

// types
#include <string>

// collections
#include <list>
#include <unordered_map>

/**
 * @brief Main simulation definitions\n
 * @throws std::runtime_error If xml input is invalid
 * @throws std::runtime_error If an object is unreachable
 */
class Simulation {
    // ╔════════════════════════════════════════╗
    // ║               Databases                ║
    // ╚════════════════════════════════════════╝

    std::unordered_map<id, RoadObject> roads;                          // stores the roads by id
    std::unordered_map<id, LightEntity> lights;                        // stores the lights by id
    std::unordered_map<id, BusStopEntity> busstops;                    // stores the busstops by id
    std::unordered_map<id, VehicleEntity> vehicles;                    // stores the vehicles by id
    std::unordered_map<id, VehicleGeneratorEntity> vehicleGenerators;  // stores the vehicleGenerators by id
    std::unordered_map<id, CrossRoadObject> crossRoads;                // stores the crossRoads by id

    // helper databases
    std::unordered_map<std::string, id> roadMap;              // stores the roadId by roadName
    std::unordered_map<id, std::list<id>> vehiclesOnRoads;    // stores arrays of vehicleIds by roadId
    std::unordered_map<id, std::list<id>> lightsOnRoads;      // stores arrays of lightIds by roadId
    std::unordered_map<id, std::list<id>> busStopsOnRoads;    // stores arrays of busStopIds by roadId
    std::unordered_map<id, std::list<id>> crossRoadsOnRoads;  // stores arrays of crossRoadIds by roadId

    std::unordered_map<id, std::pair<id, double>>
      world;  // stores the roadId along with position on the road by objectId

    Id idGen;  // generates a new unique id every time it is called

    // function members

    /**
     * Adds a vehicle to the simulation \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized"); \n
     * REQUIRE(getRoads().find(roadId) != getRoads().end(), "roadId is vlaid"); \n
     * REQUIRE(position >= 0, "position is positive or zero");
     * @param roadId id of the road to spawn the vehicle on
     * @param position position to spawn the vehicle at
     * @param type type of the vehicle
     * @return failed true/false
     */
    bool spawnVehicle(const id roadId, const double position, const EVehicleEntityTypes type);

    /**
     * Adds a crossroad with paired smart lights to the simulation \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * REQUIRE(getRoads().find(road) != getRoads().end(), "roadId is vlaid");
     * REQUIRE(pos >= 1, "position is positive or one");
     * @param roadIdOne id of the road to create the crossroad
     * @param posOne position to create the crossroad
     * @param roadIdTwo id of the corresponding road to create the crossroad
     * @param posTwo corresponding position to create the crossroad
     * @param simStepSize stepsize the simulation runs at
     * @return failed true/false
     */
    bool spawnLightCrossroad(const id roadIdOne, const double posOne, const id roadIdTwo, const double posTwo,
                             const double cylce);

    /**
    * Spaqns a light on a specified road
    * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
    * REQUIRE(getRoads().find(road) != getRoads().end(), "roadId is valid");
    * REQUIRE(pos >= 0, "position is positive or zero");
    * @param road id of the road to create the crossroad
    * @param pos position to create the crossroad
    * @param cycle seconds between light change
    * @param isGreen does the light spawn in as a green light? (default false)
    * @return failed true/false
    */
    bool spawnLight(const id road, const double pos, const int cycle, bool isGreen = false);

    /**
     * Moves a vehicle from one road to another \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized"); \n
     * REQUIRE(getRoads().find(roadId) != getRoads().end(), "roadId is vlaid"); \n
     * REQUIRE(getWorld().at(vehicleId).first != roadId, "road to move to is a different one"); \n
     * REQUIRE(position >= 0, "position is positive or zero"); \n
     * ENSURE(getVehicles().find(vehicleId) != getVehicles().end(), "vehicle is present in the database"); \n
     * ENSURE(getWorld().find(vehicleId) != getWorld().end(), "vehicle is found in the world"); \n
     * ENSURE(getWorld().at(vehicleId).first == roadId, "vehicle is on the new road"); \n
     * ENSURE(getWorld().at(vehicleId).second == position, "vehicle has the new position");
     * @param roadId id of the road to move the vehicle to
     * @param position position of the vehicle on the new road
     * @param vehicleId id of the vehicle to move
     * @return false when the position on the destinaion road is occupied
     * @return true when the vehicle is moved to a different road
     */
    bool moveVehicle(const id roadId, const double position, const id vehicleId);

    /// Deletes a vehicle from the simulation (if it is not present, nothing
    /// happens).
    bool deleteVehicle(const id vehicleId);

    /// Returns the vehicleId of the vehicle in front of the given vehicle. If
    /// there is none the given vehicleId is returned.
    id getVehicleInFront(const id vehicleId) const;

    /// Returns the lightId of the light in front of the given vehicle. If there is
    /// none the given vehicleId is returned.
    id getLightInFront(const id vehicleId) const;

    /// Returns the busStopId of the busStop in front of the given vehicle. If there is none the given vehicleId is
    /// returned.
    id getBusStopInFront(const id vehicleId) const;

    /// Converts the type of vehicle from a string to an enum
    EVehicleEntityTypes vehicleTypeStringToEnumVariant(const std::string &str) const;

  public:
    /// Converts a roadName to its unique id.
    const std::string &roadIdToName(unsigned int id) const;

    /**
     * Initializes a new instance of the simulation with the properties defined by
     * the xmlStream. \n
     * REQUIRE(stepSize > 0, "Stepsize cannot be negative or zero"); \n
     * ENSURE(properlyInitialized(), "Simulation is properly initialized"); \n
     * @exception runtime_error when the xmlStream is not valid. Writes the errors
     * to the errorStream
     * @param xmlStream input stream containing xml to be parsed into the
     * simulation
     * @param stepSize  in-simulation timesteps between two frames
     * @param errStream stream to write error messages to
     */
    Simulation(std::istream &xmlStream, double stepSize, std::ostream &errStream);

    /**
     * REQUIRE(stepSize > 0, "Stepsize cannot be negative or zero");\n
     * ENSURE(properlyInitialized(), "Simulation is properly initialized");
     * @param stepSize  in-simulation timesteps between two frames
     * @param errStream stream to write error messages to
     */
    Simulation(double stepSize, std::ostream &errStream);

    /**
     * Parses the given valMap and primes the simulation. (Valmap is generated by the Validator). \n
     * REQUIRE(getRoads().empty() && getVehicles().empty() && getLights().empty() && getVehicleGenerators().empty() &&
     * getRoadMap().empty() && getVehiclesOnRoads().empty() && getLightsOnRoads().empty(), "Simulation is empty"); \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized"); \n
     * REQUIRE(!valMap.empty(), "Valmap contains validated input"); \n
     * ENSURE(!getRoads().empty(), "Roads database cannot be empty");
     * @param valMap valMap generated by the validator to be loaded into the
     * simulation
     */
    void parse(const Validator::ValMap &valMap);

    /**
     * Ticks the simulation forward by one timeStep. \n
     * REQUIRE(!roads.empty(), "Simulation roads is not empty"); \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized"); \n
     * ENSURE(getIteration() == prevIteration + 1, "The simulation is ticked");
     */
    void godTick();

    // ╔════════════════════════════════════════╗
    // ║          Getters and setters           ║
    // ╚════════════════════════════════════════╝

    /**
     * Returns the road database \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return road database
     */
    const std::unordered_map<id, RoadObject> &getRoads() const;

    /**
     * Returns the lights database \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return lights
     */
    const std::unordered_map<id, LightEntity> &getLights() const;

    /**
     * Returns the vehicles database \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return
     */
    const std::unordered_map<id, VehicleEntity> &getVehicles() const;

    /**
     * Returns the vehicle generator database \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return
     */
    const std::unordered_map<id, VehicleGeneratorEntity> &getVehicleGenerators() const;

    /**
     * Returns the vehicle generator database \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return
     */
    const std::unordered_map<id, BusStopEntity> &getBusstops() const;

    /**
     * Returns the vehicle generator database \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return
     */
    const std::unordered_map<id, CrossRoadObject> &getCrossRoads() const;

    /**
     * Returns the roadmap database \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return roadmap (roadName to roadId)
     */
    const std::unordered_map<std::string, id> &getRoadMap() const;

    /**
     * Returns the vehiclesOnRoads database (roadId to array of vehicleIds) \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return vehiclesOnRoads database (roadId to array of vehicleIds)
     */
    const std::unordered_map<id, std::list<id>> &getVehiclesOnRoads() const;

    /**
     * Returns the lightsOnRoads database (roadId to array of lightIds) \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return lightsOnRoads database (roadId to array of lightIds)
     */
    const std::unordered_map<id, std::list<id>> &getLightsOnRoads() const;

    /**
     * Returns the busstopsOnRoads database (roadId to array of lightIds) \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return lightsOnRoads database (roadId to array of lightIds)
     */
    const std::unordered_map<id, std::list<id>> &getBusstopsOnRoads() const;

    /**
     * Returns the world database (objectId to roadId + positionOnRoad) \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return world database (objectId to roadId + positionOnRoad)
     */
    const std::unordered_map<id, std::pair<id, double>> &getWorld() const;

    /**
     * Returns the current iteration the simulation is at \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @return iteration the simulation is on
     */
    const unsigned int &getIteration() const;

    // ╔════════════════════════════════════════╗
    // ║               Contracts                ║
    // ╚════════════════════════════════════════╝

    bool properlyInitialized() const;

  private:
    // time between two ticks (simulation time, not realtime)
    const double stepSize;

    const Simulation *_initCheck;

    // amount of times the simulation has been updated
    unsigned int iteration = 0;
};

#endif  // SE_PROJECT_SIMULATION_H
