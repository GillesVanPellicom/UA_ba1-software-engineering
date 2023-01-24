//============================================================================
// Name        : RoadObject.h
// Description : RoadObject class for the roads in the simulation universe
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/11
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_ROADOBJECT_H
#define SE_PROJECT_ROADOBJECT_H

#include <string>
#include <unordered_map>

#include "../Object.h"

// Note: there are no pre/post- conditions because the parameter types solve all
// of them

/// Represents a road
class RoadObject : public Object {
    const std::string _name;
    const unsigned int _length;

  public:
    /**
     * Represents a road \n
     * ENSURE(properlyInitialized(), "Object is properly initialized");
     * @param id id to given to the object
     * @param name name of the road
     * @param length length of the road
     */
    RoadObject(const unsigned int &id, const std::string &name, const unsigned int &length);

    /**
     * REQUIRE(properlyInitialized(), "Object is properlyInitialized");
     * @return name of the road
     */
    const std::string &getName() const;

    /**
     * REQUIRE(properlyInitialized(), "Object is properlyInitialized");
     * @return length of the road
     */
    const unsigned int &getLength() const;
};

#endif  // SE_PROJECT_ROADOBJECT_H
