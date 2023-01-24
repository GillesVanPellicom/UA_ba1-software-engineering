//============================================================================
// Name        : VehicleGeneratorEntity.cpp
// Description : VehicleGeneratorEntity class implementation
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 11/03/2022
// Version     : 1.0
//============================================================================

#include "VehicleGeneratorEntity.h"

// Note: most of the preconditions are solved by the parameter types
// Note: properlyInitialized is not possible because, these classes live in a
// unordered map, that can be reordered in memory thus the pointer is no longer
// valid

VehicleGeneratorEntity::VehicleGeneratorEntity(const unsigned int &id, double stepSize, const unsigned int &frequency,
                                               EVehicleEntityTypes type) :
    Entity(id, stepSize), _frequency(frequency), _type(type), _timeLastChange(frequency) {
    REQUIRE(stepSize > 0, "stepSize is larger than zero");
    REQUIRE(frequency > 0, "frequency is larger than zero");
    ENSURE(properlyInitialized(), "The object is properly initialized");
    ENSURE(getFrequency() > 0, "frequency is larger than zero");
    ENSURE(getTimeLastChange() == frequency, "timeLastChange is started correctly");
}

bool VehicleGeneratorEntity::update() {
    REQUIRE(properlyInitialized(), "The object is properly initialized");
    if (_timeLastChange > _frequency) {
        _timeLastChange = 0;
        return true;
    } else {
        _timeLastChange += stepSize;
        return false;
    }
}

const unsigned int &VehicleGeneratorEntity::getFrequency() const {
    REQUIRE(properlyInitialized(), "The object is properly initialized");
    return _frequency;
}

const double &VehicleGeneratorEntity::getTimeLastChange() const {
    REQUIRE(properlyInitialized(), "Object is properly initialized");
    return _timeLastChange;
}

EVehicleEntityTypes VehicleGeneratorEntity::getVehicleType() const {
    REQUIRE(properlyInitialized(), "Object is properly initialized");
    return _type;
}
