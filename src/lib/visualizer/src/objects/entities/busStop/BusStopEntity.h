//============================================================================
// Name        : BusStopEntity.h
// Description : BusStopEntity class for the lights in the simulation universe
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 11/03/2022
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_BUSSTOPENTITY_H
#define SE_PROJECT_BUSSTOPENTITY_H

#include <unordered_map>

#include "../Entity.h"

/// Represents a traffic light
class BusStopEntity : protected Entity {
    const unsigned int _haltTime;

  public:
    /**
     * represents a busstop \n
     * REQUIRE(stepSize > 0, "stepSize must be greater than zero"); \n
     * ENSURE(properlyInitialized(), "Object is properly initialized");
     * @param id id to be given to the object
     * @param halfTime amount of wait time for busses
     * @param stepSize in-simulation timesteps between two frames
     */
    BusStopEntity(unsigned int id, unsigned int haltTime, double stepSize);

    void update();

    // getters and setters

    /**
     * REQUIRE(properlyInitialized(), "Object is properly initialized");
     * @return haltTime - time a bus needs to wait
     */
    const unsigned int &getHaltTime() const;
};

#endif  // SE_PROJECT_BUSSTOPENTITY_H
