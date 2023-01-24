//============================================================================
// Name        : Object.cpp
// Description : Object class implementation
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/11
// Version     : 1.0
//============================================================================

#include "Object.h"

Object::Object(unsigned int id) : _id(id), _initCheck(this) {
    ENSURE(properlyInitialized(), "Object is properly initialized");
}

Object::Object(const Object &other): _id(other._id) {
    _initCheck = other._initCheck;
}

// getters and setters
const unsigned int &Object::getId() const {
    REQUIRE(properlyInitialized(), "Object is properly initialized");
    return _id;
}

bool Object::properlyInitialized() const { return _initCheck; }
