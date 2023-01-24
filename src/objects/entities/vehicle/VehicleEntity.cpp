//============================================================================
// Name        : VehicleEntity.cpp
// Description : VehicleEntity class implementation
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 11/03/2022
// Version     : 1.0
//============================================================================

#include "VehicleEntity.h"

VehicleEntity::VehicleEntity(unsigned int id, double stepSize, EVehicleEntityTypes type) :
    Entity(id, stepSize), type(type) {
    targetVelocity = 20.0;

    const mINI::INIFile file(path::resFolderPath + "config/constants.ini");
    mINI::INIStructure ini;
    file.read(ini);

    length = Utils::stod(ini[vehicleEntityEnumToIniFileSubsectionNameConverter[type]]["length"]);
    maxVelocity = Utils::stod(ini[vehicleEntityEnumToIniFileSubsectionNameConverter[type]]["vMax"]);
    maxAcceleration = Utils::stod(ini[vehicleEntityEnumToIniFileSubsectionNameConverter[type]]["aMax"]);
    maxBrakeForce = Utils::stod(ini[vehicleEntityEnumToIniFileSubsectionNameConverter[type]]["bMax"]);
    minimalFollow = Utils::stod(ini[vehicleEntityEnumToIniFileSubsectionNameConverter[type]]["fMin"]);

    ENSURE(stepSize > 0, "stepSize is larger than zero");
    ENSURE(length > 0, "length is greater than zero");
    ENSURE(maxVelocity > 0, "maxVelocity is greater than zero");
    ENSURE(maxAcceleration > 0, "maxAcceleration is greater than zero");
    ENSURE(maxBrakeForce > 0, "maxBreakforce is greater than zero");
    ENSURE(minimalFollow > 0, "minimalFollow is greater than zero");
    ENSURE(properlyInitialized(), "Object is properly initialized");
}

double VehicleEntity::update(double distToCar, double velNextCar, double distToLight, double distToBusStop,
                             int busHaltTime, unsigned int busStopId, const bool priorityVehicleInFront) {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    REQUIRE((distToLight > 0 && distToLight != std::numeric_limits<double>::infinity()) ||
              distToLight == std::numeric_limits<double>::infinity(),
            "distToLight is positive or light is not found");
    REQUIRE(
      ((velNextCar >= 0 && distToCar > 0) && distToCar != std::numeric_limits<double>::infinity()) ||
        (distToCar == std::numeric_limits<double>::infinity() && velNextCar == std::numeric_limits<double>::infinity()),
      "car is found and data is set or car is not found and data is empty");
    REQUIRE((distToBusStop == std::numeric_limits<double>::infinity() &&
             busHaltTime == std::numeric_limits<int>::infinity()) ||
              (type == EVehicleEntityTypes::kBus),
            "Type is not a bus and busattributes are not set");

    if (priorityVehicleInFront) {
        distToCar = std::numeric_limits<double>::infinity();
        velNextCar = std::numeric_limits<double>::infinity();
    }

    if (type == EVehicleEntityTypes::kPoliceCruiser || type == EVehicleEntityTypes::kAmbulance || type == EVehicleEntityTypes::kFireTruck) {
        distToLight = std::numeric_limits<double>::infinity();
    }

    // halt at current stop for haltTime
    if (ticksStopped != -1 && ticksStopped * stepSize < busHaltTime) {
        // std::cout << "timeStopped: " << ticksStopped * stepSize << '\n';
        ++ticksStopped;
        return 0;
    }

    // determine closest obstacle
    double distToObstacle;
    // if we passed the time of the current stop, then other obstacles are of interest
    if (busStop == (int) busStopId) {
        distToObstacle = std::min(distToCar, distToLight);
    } else {
        distToObstacle = std::min({distToCar, distToLight, distToBusStop});
    }

    double result;

    // calculate velocity
    if (velocity + acceleration * stepSize < 0) {
        result = -std::pow(velocity, 2) / (2 * acceleration);
        velocity = 0;
    } else {
        velocity += acceleration * stepSize;
        result = velocity * stepSize + acceleration * (std::pow(stepSize, 2) / 2);
    }

    // calculate velocity difference
    const double velDiff = velocity - velNextCar;

    // calculate δ
    double delta = 0;
    if (distToObstacle == distToCar) {
        delta = (minimalFollow +
                 std::max(0.0, velocity + (velocity * velDiff) / 2 * std::sqrt(maxAcceleration * maxBrakeForce))) /
                distToCar;
    }

    // calculate acceleration
    acceleration = maxAcceleration * (1 - std::pow(velocity / targetVelocity, 4) - std::pow(delta, 2));

    // check for obstacles
    if ((distToObstacle == distToLight || distToObstacle == distToBusStop) && distToObstacle <= 50) {
        if (distToObstacle <= 15.0) {
            // stop the vehicle
            acceleration = -maxBrakeForce * velocity / targetVelocity;
        } else if (distToObstacle >= 15.0) {
            // slow down the vehicle
            targetVelocity = 0.4 * maxVelocity;
        }
    } else {
        targetVelocity = maxVelocity;
    }

    // check if bus is stopped for busstop that it hasn't done yet
    if (distToObstacle == distToBusStop && distToBusStop < 15 && velocity < 0.1 && busStop == -1) {
        ticksStopped = 0;
        busStop = busStopId;
    }

    // if we drive ahead of the current busstop
    if (busStop != -1 && (int) busStopId != busStop) {
        ticksStopped = -1;
        busStop = -1;
    }

    ENSURE(velocity >= 0, "velocity is greater or equal to zero");
    ENSURE(result != INFINITY, "result is not infinity (division by zero)");

    // return relative position change
    return result;
}

// getters and setters
// ==== Max constants ====
const double &VehicleEntity::getLength() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return length;
}

const double &VehicleEntity::getMaxVelocity() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return maxVelocity;
}

const double &VehicleEntity::getMaxBrakeForce() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return maxBrakeForce;
}

const double &VehicleEntity::getMinimalFollow() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return minimalFollow;
}

const double &VehicleEntity::getMaxAcceleration() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return maxAcceleration;
}

// ==== World parameters ====
EVehicleEntityTypes VehicleEntity::getType() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return type;
}

const double &VehicleEntity::getVelocity() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return velocity;
}

void VehicleEntity::setVelocity(const double &velocity_) {
    REQUIRE(velocity_ >= 0, "velocity is greater than zero");
    REQUIRE(properlyInitialized(), "object is properly initialized");
    VehicleEntity::velocity = velocity_;
    ENSURE(velocity >= 0, "velocity is greater than zero");
}

const double &VehicleEntity::getTargetVelocity() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return targetVelocity;
}

void VehicleEntity::setTargetVelocity(const double &targetVelocity_) {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    VehicleEntity::targetVelocity = targetVelocity_;
}

const double &VehicleEntity::getAcceleration() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return acceleration;
}

void VehicleEntity::setAcceleration(const double &acceleration_) {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    VehicleEntity::acceleration = acceleration_;
}

const double &VehicleEntity::getBrakeDistance() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return brakeDistance;
}

void VehicleEntity::setBrakeDistance(const double &brakeDistance_) {
    REQUIRE(brakeDistance_ > 0, "brakeDistance is greater than zero");
    REQUIRE(properlyInitialized(), "object is properly initialized");
    VehicleEntity::brakeDistance = brakeDistance_;
    ENSURE(brakeDistance > 0, "brakeDistnce is greater than zero");
}

const double &VehicleEntity::getStopDistance() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return stopDistance;
}

void VehicleEntity::setStopDistance(const double &stopDistance_) {
    REQUIRE(stopDistance_ >= 0, "stopdistance is greater than zero");
    REQUIRE(properlyInitialized(), "object is properly initialized");
    VehicleEntity::stopDistance = stopDistance_;
    ENSURE(stopDistance >= 0, "stopDistance is greater than zero");
}

const double &VehicleEntity::getBrakeForce() const {
    REQUIRE(properlyInitialized(), "object is properly initialized");
    return brakeForce;
}

void VehicleEntity::setBrakeForce(const double &brakeForce_) {
    REQUIRE(brakeForce_ >= 0, "brakeForce is greater than zero");
    REQUIRE(properlyInitialized(), "object is properly initialized");
    VehicleEntity::brakeForce = brakeForce_;
    ENSURE(brakeForce >= 0, "brakeForce is greater than zero");
}
