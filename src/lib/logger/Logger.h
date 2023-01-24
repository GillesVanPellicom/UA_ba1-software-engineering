//============================================================================
// Name        : JsonLog.cpp
// Description : Static helper class for logging 
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 03/05/2022
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_JSONLOG_H
#define SE_PROJECT_JSONLOG_H

#include "../../Simulation.h"

#include <list>
#include <string>

/**
 * @brief Static helper class for logging \n
 * @throws std::runtime_error If xml input is invalid
 * @throws std::runtime_error If an object is unreachable
 */
class Logger {
  public:
    /**
     * Prints the simulation to the given ostream in a simple text format \n
     * REQUIRE(!getRoads().empty(), "Simulation roads should not be empty"); \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @param sim instance of simulation
     * @param outStream outputStream to write to
     */
    static void logAsJson(const Simulation &sim, std::ostream &outStream);

    /**
     * Prints the simulation to the given ostream in a json format. \n
     * (used for: https://github.com/jonatcln/pse-traffic-simulation-visualizer). \n
     * REQUIRE(!getRoads().empty(), "Simulation roads should not be empty"); \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @param sim instance of simulation
     * @param outStream outputStream to write to
     */
    static void logSimpleOutput(const Simulation &sim, std::ostream &outStream);

    /**
     * Prints the simulation to the given ostream in complex text format. \n
     * REQUIRE(!getRoads().empty(), "Simulation roads should not be empty"); \n
     * REQUIRE(properlyInitialized(), "Simulation is properly initialized");
     * @param sim instance of simulation
     * @param outStream outputStream to write to
     */
    static void logAdvancedOutput(const Simulation &sim, std::ostream &outStream, int rowSize = -1);

  private:
    static char vehicleTypeToLetter(EVehicleEntityTypes type);

    static unsigned int getCharPosition(const double position, const double charSize);
};

#endif  // SE_PROJECT_JSONLOG_H