//============================================================================
// Name        : Entity.cpp
// Description : Entity class implementation
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/11
// Version     : 1.0
//============================================================================

#include "Entity.h"

Entity::Entity(unsigned int id, double stepSize) : Object(id), stepSize(stepSize) {
    REQUIRE(stepSize > 0, "stepSize is larger than zero");
    ENSURE(properlyInitialized(), "Object is properly initialized");
}
