//============================================================================
// Name        : LightEntity.h
// Description : LightEntity class for the lights in the simulation universe
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 11/03/2022
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_LIGHTENTITY_H
#define SE_PROJECT_LIGHTENTITY_H

#include <unordered_map>

#include "../Entity.h"

/// Represents a traffic light
class LightEntity : protected Entity {
    const unsigned int _cycle = 0;
    double _timeLastChange = 0;
    bool _isGreen = false;

  public:
    /**
     * represents a traffic light \n
     * REQUIRE(stepSize > 0, "stepSize must be greater than zero");\n
     * ENSURE(properlyInitialized(), "Object is properly initialized");
     * @param id id to be given to the object
     * @param cycle amount of time between the switch from green to red or vice
     * versa
     * @param stepSize in-simulation timesteps between two frames
     */
    LightEntity(unsigned int id, unsigned int cycle, double stepSize);

    // FIXME temp
    LightEntity(unsigned int id, unsigned int cycle, double stepSize, bool isGreen);

    void update();

    // getters and setters
    /**
     * REQUIRE(properlyInitialized(), "Object is properly initialized");
     * @return bool is light currently green?
     */
    const bool &isGreen() const;

    /**
     * REQUIRE(properlyInitialized(), "Object is properly initialized");
     * @return current cycle
     */
    const unsigned int &getCycle() const;

    /**
     * REQUIRE(properlyInitialized(), "Object is properly initialized");
     * @return last time light colour changed
     */
    const double &getTimeLastChange() const;
};

#endif  // SE_PROJECT_LIGHTENTITY_H
