//============================================================================
// Name        : RoadObject.cpp
// Description : RoadObject class implementation
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/11
// Version     : 1.0
//============================================================================

#include "RoadObject.h"

#include <string>

RoadObject::RoadObject(const unsigned int &id, const std::string &name, const unsigned int &length) :
    Object(id), _name(name), _length(length) {
    ENSURE(properlyInitialized(), "Object is properly initialized");
}

const std::string &RoadObject::getName() const {
    REQUIRE(properlyInitialized(), "Object is properlyInitialized");
    return _name;
}

const unsigned int &RoadObject::getLength() const {
    REQUIRE(properlyInitialized(), "Object is properlyInitialized");
    return _length;
}
