//============================================================================
// Name        : Entity.h
// Description : Entity class for all the movable objects in the simulation universe
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/11
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_ENTITY_H
#define SE_PROJECT_ENTITY_H

#include <string>

#include "../Object.h"

/// Abstract super class to represent all objects that can be ticked (entities)
class Entity : public Object {
  protected:
    const double stepSize;

  public:
    /**
     * Abstract super class to represent all objects that can be ticked (entities) \n
     * REQUIRE(stepSize > 0, "stepSize is larger than zero");\n
     * ENSURE(properlyInitialized(), "Object is properly initialized");
     * @param id id to be given to the object
     * @param stepSize in-simulation timesteps between two frames
     */
    Entity(unsigned int id, double stepSize);
};

#endif  // SE_PROJECT_ENTITY_H
