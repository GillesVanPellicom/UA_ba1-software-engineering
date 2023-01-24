//============================================================================
// Name        : VehicleGeneratorEntity.h
// Description : VehicleGeneratorEntity class for the vehicle generators in the simulation universe
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/11
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_VEHICLEGENERATORENTITY_H
#define SE_PROJECT_VEHICLEGENERATORENTITY_H

#include "../Entity.h"
#include "../vehicle/VehicleEntity.h"

/// Represents a vehicle spawner
class VehicleGeneratorEntity : public Entity {
    const unsigned int _frequency;
    const EVehicleEntityTypes _type;
    double _timeLastChange = 0;

  public:
    /**
     * Represents a vehicle spawner \n
     * REQUIRE(stepSize > 0 , "stepSize is larger than zero"); \n
     * REQUIRE(frequency > 0, "frequency is larger than zero"); \n
     * ENSURE(properlyInitialized(), "The object is properly initialized"); \n
     * ENSURE(getFrequency() > 0, "frequency is larger than zero"); \n
     * ENSURE(getTimeLastChange() == 0, "timeLastChange is started correctly");
     * @param id id to be given to the object
     * @param stepSize in-simulation timesteps between two frames
     * @param frequency how often a car needs to be spawned
     */
    VehicleGeneratorEntity(const unsigned int &id, double stepSize, const unsigned int &frequency,
                           EVehicleEntityTypes type);

    /**
     * REQUIRE(properlyInitialized(), "The object is properly initialized");
     * @return spawnNeeded boolean indicating whether a new vehicle needs to be spawned
     */
    bool update();

    /**
     * REQUIRE(properlyInitialized(), "Object is properly initialized");
     * @return frequency of vehicle spawns
     */
    const unsigned int &getFrequency() const;

    /**
     * REQUIRE(properlyInitialized(), "Object is properly initialized");
     * @return last time there was a spawn on this generator
     */
    const double &getTimeLastChange() const;

    /**
     * REQUIRE(properlyInitialized(), "Object is properly initialized");
     * @return vehicle type of the generator
     */
    EVehicleEntityTypes getVehicleType() const;
};

#endif  // SE_PROJECT_VEHICLEGENERATORENTITY_H
