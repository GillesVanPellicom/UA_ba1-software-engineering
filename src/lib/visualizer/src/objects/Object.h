//============================================================================
// Name        : Object.h
// Description : Object class for the Objects in the simulation universe
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/11
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_OBJECT_H
#define SE_PROJECT_OBJECT_H

#include "../../contract/Contract.h"
#include "../lib/utils/Id.h"

/// Abstract class to represent all objects inside the simulation
class Object {
    const id _id;
    const Object *_initCheck;

  public:
    /**
     * Abstract class to represent all objects inside the simulation\n
     * ENSURE(properlyInitialized(), "Object is properly initialized");
     * @param id id to be given to the object
     */
    explicit Object(unsigned int id);

    /**
     * Copy constructor for Object class
     * @param other object to copy
     */
    Object(const Object &other);

    // getters and setters

    /**
     * REQUIRE(properlyInitialized(), "Object is properly initialized");
     * @return id of the object
     */
    const unsigned int &getId() const;

    bool properlyInitialized() const;
};

#endif  // SE_PROJECT_OBJECT_H
