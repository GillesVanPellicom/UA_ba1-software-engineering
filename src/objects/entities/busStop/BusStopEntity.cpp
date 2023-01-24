//============================================================================
// Name        : BusStopEntity.cpp
// Description : BusStopEntity class implementation
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 11/03/2022
// Version     : 1.0
//============================================================================

#include "BusStopEntity.h"

BusStopEntity::BusStopEntity(unsigned int id, unsigned int haltTime, double stepSize) :
    Entity(id, stepSize), _haltTime(haltTime) {
    REQUIRE(stepSize > 0, "stepSize must be greater than zero");
    ENSURE(properlyInitialized(), "Object is properly initialized");
}

const unsigned int &BusStopEntity::getHaltTime() const {
    REQUIRE(properlyInitialized(), "Object is properly initialized");
    return _haltTime;
}
