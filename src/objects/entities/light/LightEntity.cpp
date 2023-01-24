//============================================================================
// Name        : LightEntity.cpp
// Description : LightEntity class implementation
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 11/03/2022
// Version     : 1.0
//============================================================================

#include "LightEntity.h"

LightEntity::LightEntity(unsigned int id, unsigned int cycle, double stepSize) : Entity(id, stepSize), _cycle(cycle) {
    REQUIRE(stepSize > 0, "stepSize must be greater than zero");
    ENSURE(properlyInitialized(), "Object is properly initialized");
}

LightEntity::LightEntity(unsigned int id, unsigned int cycle, double stepSize, bool isGreen) : Entity(id, stepSize), _cycle(cycle), _isGreen(isGreen) {
    REQUIRE(stepSize > 0, "stepSize must be greater than zero");
    ENSURE(properlyInitialized(), "Object is properly initialized");
}

const double &LightEntity::getTimeLastChange() const {
    REQUIRE(properlyInitialized(), "Object is properly initialized");
    return _timeLastChange;
}

void LightEntity::update() {
    REQUIRE(properlyInitialized(), "Object is properly initialized");
    // if last colour change time > current cycle
    if (_timeLastChange > _cycle) {
        // switch light colour
        if (_isGreen)
            _isGreen = false;
        else
            _isGreen = true;
        // last change reverts to 0
        _timeLastChange = 0;

    } else
        _timeLastChange += stepSize;
}

const bool &LightEntity::isGreen() const {
    REQUIRE(properlyInitialized(), "Object is properly initialized");
    return _isGreen;
}

const unsigned int &LightEntity::getCycle() const {
    REQUIRE(properlyInitialized(), "Object is properly initialized");
    return _cycle;
}
