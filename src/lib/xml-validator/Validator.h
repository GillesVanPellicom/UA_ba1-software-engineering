//============================================================================
// Name        : Validator.h
// Description : Definition of the validator class
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/06
// Version     : 1.0
//============================================================================

#ifndef SE_PROJECT_VALIDATOR_H
#define SE_PROJECT_VALIDATOR_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

// Note: This is a static class and thus does not need a properlyInitialized

/// Static class to validate XML input stream.
class Validator {
  public:
    enum class EObjectTypes { kRoad, kLight, kVehicle, kVehicleGenerator, kBusStop, kCrossRoad };

    enum class ERoadAttributes { kName, kLength };

    enum class ELightAttributes { kRoadName, kPosition, kCycleTime };

    enum class EVehicleAttributes { kRoadName, kPosition, kType };

    enum class EVehicleGeneratorAttributes { kRoadName, kFrequency, kType };

    enum class EBusStopAttributes { kRoadName, kPosition, kHaltTime };

    enum class ECrossRoadAttributes { kFirstRoadPosition, kSecondRoadPosition, kFirstRoad, kSecondRoad, kLights };

    typedef std::variant<ERoadAttributes, ELightAttributes, EVehicleAttributes, EVehicleGeneratorAttributes,
                         EBusStopAttributes, ECrossRoadAttributes>
      EAttributes;

    typedef std::unordered_map<EObjectTypes, std::vector<std::unordered_map<EAttributes, std::string>>> ValMap;
    typedef std::unordered_map<EAttributes, std::string> Object;

    /**
     * Validates the xmlInput and if valid returns the valMap. If the xml was not valid then an empty valmap is
     * returned.
     * @param istream   text stream containing the xml text
     * @param errStream stream to write the error messages to
     * @return ValMap that contains the correct values for the simulation parser if valid, else it is empty.
     */
    static ValMap validate(std::istream &istream, std::ostream &errStream);

  private:
    static Object findRoadWithName(const std::string &name, const ValMap &config);
};

/// allowed attributes for all the objects
const std::unordered_map<Validator::EObjectTypes, std::vector<std::string>> requiredAttributes = {
  {Validator::EObjectTypes::kRoad, {"naam", "lengte"}},
  {Validator::EObjectTypes::kLight, {"baan", "positie", "cyclus"}},
  {Validator::EObjectTypes::kVehicle, {"baan", "positie", "type"}},
  {Validator::EObjectTypes::kVehicleGenerator, {"baan", "frequentie", "type"}},
  {Validator::EObjectTypes::kBusStop, {"baan", "positie", "wachttijd"}},
  {Validator::EObjectTypes::kCrossRoad, {"baan"}},
};

const std::unordered_map<Validator::EObjectTypes, std::vector<std::string>> optionalAttributes = {
  {Validator::EObjectTypes::kCrossRoad, {"lichten"}},
};

/// allowed vehicle types for the input files
const std::vector<std::string> vehicleTypes = {"auto", "bus", "brandweerwagen", "ziekenwagen", "politiecombi"};

#endif  // SE_PROJECT_VALIDATOR_H
