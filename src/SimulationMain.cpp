//============================================================================
// Name        : SimulationMain.cpp
// Description : TrafficSimulation entrypoint
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/03
// Version     : 1.0
//============================================================================

// third-party libraries
#include "lib/pugixml/pugixml.hpp"

// local libraries
#include <fstream>
#include <iostream>

#include "Simulation.h"
#include "lib/xml-validator/Validator.h"

// containers
#include <map>
#include <string>

#include "./lib/logger/Logger.h"
#include "./lib/path/path.h"

// TODO: Edit all classes to account for contract publicity

int main(int argc, char **argv) {
    std::ifstream file(path::resFolderPath + "xml/input4.xml");
    Simulation sim(file, 1.0 / 60.0, std::cerr);

    for (unsigned int i = 0; i < 30000; ++i) {
        Logger::logAsJson(sim, std::cout);
        sim.godTick();
    }
    Logger::logAsJson(sim, std::cout);

    return 0;
}
