//============================================================================
// Name        : CrossRoadObject.cpp
// Description : CrossRoadObject class implementation
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/11
// Version     : 1.0
//============================================================================

#include "CrossRoadObject.h"

#include <string>

CrossRoadObject::CrossRoadObject(const id id_, const id counterPart) : Object(id_), counterPart(counterPart) {
    ENSURE(properlyInitialized(), "Object is properly initialized");
}

id CrossRoadObject::getCounterPart() {
    REQUIRE(properlyInitialized(), "Object is properlyInitialized");
    return counterPart;
}
