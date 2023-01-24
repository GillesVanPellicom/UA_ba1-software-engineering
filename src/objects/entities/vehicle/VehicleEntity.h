//============================================================================
// Name        : VehicleEntity.h
// Description : VehicleEntity class for the vehicles in the simulation universe
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 11/03/2022
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_VEHICLEENTITY_H
#define SE_PROJECT_VEHICLEENTITY_H

#include <cmath>
#include <iostream>
#include <unordered_map>

#include "../../../lib/mini/ini.h"
#include "../../../lib/path/path.h"
#include "../../../lib/utils/Utils.h"
#include "../Entity.h"

enum EVehicleEntityTypes { kCar, kBus, kFireTruck, kAmbulance, kPoliceCruiser };

/// converts the type of the vehicle to the name of the section in the ini
/// constants file
static std::unordered_map<EVehicleEntityTypes, std::string> vehicleEntityEnumToIniFileSubsectionNameConverter = {
  {kCar, "vehicleEntityCar"},
  {kBus, "vehicleEntityBus"},
  {kFireTruck, "vehicleEntityFireTruck"},
  {kAmbulance, "vehicleEntityAmbulance"},
  {kPoliceCruiser, "vehicleEntityPoliceCruiser"}};

/// Represents a vehicle
class VehicleEntity : public Entity {
    const EVehicleEntityTypes type;

    double length;            // l -> length in meters  (const)
    double velocity{};        // v -> velocity in m/s
    double targetVelocity{};  // v_max -> velocity to be maintained in m/s
    double maxVelocity;       // V_max -> maximal velocity in m/s   (const)
    double acceleration{};    // a -> current acceleration in m/s^2
    double maxAcceleration;   // a_max -> maximum acceleration of the engine in
                              // m/s^2   (const)
    double maxBrakeForce;     // b_max -> maximal brake force of the car in Newtons
                              // (const)
    double minimalFollow;     // f_min -> minimal distance to a car in front in
                              // meters  (const)
    double brakeDistance{};   // Δx_s -> distance to braking zone + stopDistance in
                              // meters
    double stopDistance{};    // Δx_s0 -> distance to position to be stopped in meters
    double brakeForce{};      // s -> current force applied to brakes in Newtons

    // Bus variables
    int ticksStopped{-1};  // amount of ticks a bus has been stopped
    // bool ignoreStops = false;  // boolean used when the haltime of a busstop is reached
    int busStop{-1};

  public:
    /**
     * Represents a vehicle \n
     * ENSURE(stepSize > 0, "stepSize is larger than zero"); \n
     * ENSURE(length > 0, "length is greater than zero"); \n
     * ENSURE(maxVelocity > 0, "maxVelocity is greater than zero"); \n
     * ENSURE(maxAcceleration > 0, "maxAcceleration is greater than zero"); \n
     * ENSURE(maxBrakeForce > 0, "maxBreakforce is greater than zero"); \n
     * ENSURE(minimalFollow > 0, "minimalFollow is greater than zero"); \n
     * ENSURE(properlyInitialized(), "Object is properly initialized"); \n
     * @param id entity identification number
     * @param stepSize simulation stepsize
     * @param length vehicle length measured from nose
     * @param maxVelocity maximal velocity
     * @param maxAcceleration maximal acceleration
     * @param maxBrakeForce maximal brake force
     * @param minimalFollow minimal follow distance between vehicles
     */
    VehicleEntity(unsigned int id, double stepSize, EVehicleEntityTypes type);

    /**
     * Ticks the vehicle and updates the values accordingly with the stepSize \n
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * REQUIRE((distToLight > 0 && distToLight != std::numeric_limits<double>::infinity()) ||
              distToLight == std::numeric_limits<double>::infinity(),
            "distToLight is positive or light is not found");
     * REQUIRE(((velNextCar >= 0 && distToCar > 0) && distToCar != std::numeric_limits<double>::infinity()) ||
        (distToCar == std::numeric_limits<double>::infinity() && velNextCar == std::numeric_limits<double>::infinity()),
        "car is found and data is set or car is not found and data is empty");
     * REQUIRE((distToBusStop == std::numeric_limits<double>::infinity() && busHaltTime ==
     std::numeric_limits<int>::infinity()) || (type == EVehicleEntityTypes::kBus), "Type is not a bus and busattributes
     are not set");
     * @param distToCar
     * @param velOfNextCar
     * @param distToLight
     * @return the change in position
     */
    double update(double distToCar, double velOfNextCar, double distToLight, double distToBusStop, int busHaltTime,
                  unsigned int busStopId, const bool priorityVehicleInFront);

    // getters and setters

    // ╔════════════════════════════════════════╗
    // ║            Lin/max constants           ║
    // ╚════════════════════════════════════════╝

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @return vehicle length
     */
    const double &getLength() const;

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @return maximum acceleration
     */
    const double &getMaxAcceleration() const;

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @return maximum velocity
     */
    const double &getMaxVelocity() const;

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @return maximal brake force
     */
    const double &getMaxBrakeForce() const;

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @return minimal follow distance
     */
    const double &getMinimalFollow() const;

    // ╔════════════════════════════════════════╗
    // ║        Vehicle state parameters        ║
    // ╚════════════════════════════════════════╝
    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @return type of vehicle
     */
    EVehicleEntityTypes getType() const;

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @return current velocity
     */
    const double &getVelocity() const;

    /**
     * REQUIRE(velocity_ > 0, "velocity is greater than zero"); \n
     * REQUIRE(properlyInitialized(), "object is properly initialized"); \n
     * ENSURE(velocity > 0, "velocity is greater than zero");
     * @param velocity_
     */
    void setVelocity(const double &velocity_);

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @return target velocity
     */
    const double &getTargetVelocity() const;

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @param targetVelocity_
     */
    void setTargetVelocity(const double &targetVelocity_);

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @return current acceleration
     */
    const double &getAcceleration() const;

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @param acceleration_
     */
    void setAcceleration(const double &acceleration_);

    /**
     * REQUIRE(brakeDistance_ > 0, "brakeDistance is greater than zero"); \n
     * REQUIRE(properlyInitialized(), "object is properly initialized"); \n
     * ENSURE(brakeDistance > 0, "brakeDistnce is greater than zero");
     * @return brake distance
     */
    const double &getBrakeDistance() const;

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @param brakeDistance
     */
    void setBrakeDistance(const double &brakeDistance);

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @return stop distance
     */
    const double &getStopDistance() const;

    /**
     * REQUIRE(stopDistance_ > 0, "stopdistance is greater than zero"); \n
     * REQUIRE(properlyInitialized(), "object is properly initialized"); \n
     * ENSURE(stopDistance > 0, "stopDistance is greater than zero");
     * @param stopDistance
     */
    void setStopDistance(const double &stopDistance);

    /**
     * REQUIRE(properlyInitialized(), "object is properly initialized");
     * @return brake force
     */
    const double &getBrakeForce() const;

    /**
     * REQUIRE(brakeForce_ > 0, "brakeForce is greater than zero"); \n
     * REQUIRE(properlyInitialized(), "object is properly initialized"); \n
     * ENSURE(brakeForce > 0, "brakeForce is greater than zero");
     * @param brakeForce
     */
    void setBrakeForce(const double &brakeForce);
};

#endif  // SE_PROJECT_VEHICLEENTITY_H
