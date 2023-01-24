//============================================================================
// Name        : Validator.cpp
// Description : Implementation of the validator class
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/06
// Version     : 1.0
//============================================================================

#include "Validator.h"
#include "../contract/Contract.h"
#include "../pugixml/pugixml.hpp"
#include "../utils/Utils.h"

#include <algorithm>
#include <iostream>

/**
 * Validates the xmlInput and if valid returns the valMap. If the xml was not valid then an empty valmap is
 * returned.
 * @param istream   text stream containing the xml text
 * @param errStream stream to write the error messages to
 * @return ValMap that contains the correct values for the simulation parser if valid, else it is empty.
 */
Validator::ValMap Validator::validate(std::istream &istream, std::ostream &errStream) {
    // parse xml file
    pugi::xml_document doc;
    pugi::xml_parse_result res = doc.load(istream);

    // check for errors
    if (!res) {
        errStream << "[XmlValidator] " << res.description() << "\n";
        return {};
    }

    // parse xml into map
    ValMap config;
    pugi::xml_node root = doc.child("ROOT");

    if (root.empty()) {
        errStream << "[XmlValidator] No flag 'ROOT' found in the input stream."
                  << "\n";
        return {};
    }

    // parse the xml as a non-validated map and check that all attributes can be set
    bool error = false;
    std::vector<std::string> roadNames;
    for (const pugi::xml_node &object : root.children()) {
        // check what type the object has
        if (std::string(object.name()) == "BAAN") {
            // check only valid attributes are set
            bool falseAttribute = false;
            std::vector<std::string> attributes;
            for (const pugi::xml_node &attribute : object.children()) {
                if (std::find(requiredAttributes.at(EObjectTypes::kRoad).begin(),
                              requiredAttributes.at(EObjectTypes::kRoad).end(),
                              attribute.name()) == requiredAttributes.at(EObjectTypes::kRoad).end()) {
                    errStream << "[XmlValidator] Attribute '" << attribute.name()
                              << "' is not known for object type 'road'."
                              << "\n";
                    error = true;
                    falseAttribute = true;
                } else if (std::find(attributes.begin(), attributes.end(), attribute.name()) != attributes.end()) {
                    errStream << "[XmlValidator] Attribute '" << attribute.name()
                              << "' is already set and cannot be set again for type 'road'."
                              << "\n";
                    error = true;
                    falseAttribute = true;
                } else {
                    attributes.emplace_back(attribute.name());
                }
            }

            if (falseAttribute) continue;

            // check missing attributes
            if (attributes.size() != requiredAttributes.at(EObjectTypes::kRoad).size()) {
                errStream << "[XmlValidator] Not all required attributes for type 'road' are set (required: ";
                for (std::vector<std::string>::const_iterator it = requiredAttributes.at(EObjectTypes::kRoad).begin();
                     it != requiredAttributes.at(EObjectTypes::kRoad).end(); ++it) {
                    if (std::next(it) == requiredAttributes.at(EObjectTypes::kRoad).end())
                        errStream << *it << ")."
                                  << "\n";
                    else
                        errStream << *it << ", ";
                }
                error = true;
                continue;
            }

            // extract attributes
            const std::string name = object.child("naam").text().as_string();
            const std::string length = object.child("lengte").text().as_string();

            // check if name of road already exists
            if (std::find(roadNames.begin(), roadNames.end(), name) != roadNames.end()) {
                errStream << "[XmlValidator] Cannot create multiple roads with name: '" << name
                          << "'. Roadname must be unique."
                          << "\n";
                error = true;
                continue;
            }

            roadNames.push_back(name);

            // place attributes in map for export
            config[EObjectTypes::kRoad].push_back(
              {{EAttributes(ERoadAttributes::kName), name}, {EAttributes(ERoadAttributes::kLength), length}});

        } else if (std::string(object.name()) == "VERKEERSLICHT") {
            // check only valid attributes are set
            bool falseAttribute = false;
            std::vector<std::string> attributes;
            for (const pugi::xml_node &attribute : object.children()) {
                if (std::find(requiredAttributes.at(EObjectTypes::kLight).begin(),
                              requiredAttributes.at(EObjectTypes::kLight).end(),
                              attribute.name()) == requiredAttributes.at(EObjectTypes::kLight).end()) {
                    errStream << "[XmlValidator] Attribute '" << attribute.name()
                              << "' is not known for object type 'light'."
                              << "\n";
                    error = true;
                    falseAttribute = true;
                } else if (std::find(attributes.begin(), attributes.end(), attribute.name()) != attributes.end()) {
                    errStream << "[XmlValidator] Attribute '" << attribute.name()
                              << "' is already set and cannot be set again for type 'light'."
                              << "\n";
                    error = true;
                    falseAttribute = true;
                } else {
                    attributes.emplace_back(attribute.name());
                }
            }

            if (falseAttribute) continue;

            // check missing attributes
            if (attributes.size() != requiredAttributes.at(EObjectTypes::kLight).size()) {
                errStream << "[XmlValidator] Not all required attributes for type 'light' are set (required: ";
                for (std::vector<std::string>::const_iterator it = requiredAttributes.at(EObjectTypes::kLight).begin();
                     it != requiredAttributes.at(EObjectTypes::kLight).end(); ++it) {
                    if (std::next(it) == requiredAttributes.at(EObjectTypes::kLight).end())
                        errStream << *it << ")."
                                  << "\n";
                    else
                        errStream << *it << ", ";
                }
                error = true;
                continue;
            }

            // extract attributes
            const std::string road = object.child("baan").text().as_string();
            const std::string pos = object.child("positie").text().as_string();
            const std::string cycleTime = object.child("cyclus").text().as_string();

            // place attributes in map for export
            config[EObjectTypes::kLight].push_back({{EAttributes(ELightAttributes::kRoadName), road},
                                                    {EAttributes(ELightAttributes::kPosition), pos},
                                                    {EAttributes(ELightAttributes::kCycleTime), cycleTime}});

        } else if (std::string(object.name()) == "VOERTUIG") {
            // check only valid attributes are set
            bool falseAttribute = false;
            std::vector<std::string> attributes;
            for (const pugi::xml_node &attribute : object.children()) {
                if (std::find(requiredAttributes.at(EObjectTypes::kVehicle).begin(),
                              requiredAttributes.at(EObjectTypes::kVehicle).end(),
                              attribute.name()) == requiredAttributes.at(EObjectTypes::kVehicle).end()) {
                    errStream << "[XmlValidator] Attribute '" << attribute.name()
                              << "' is not known for object type 'vehicle'."
                              << "\n";
                    error = true;
                    falseAttribute = true;
                } else if (std::find(attributes.begin(), attributes.end(), attribute.name()) != attributes.end()) {
                    errStream << "[XmlValidator] Attribute '" << attribute.name()
                              << "' is already set and cannot be set again for type 'vehicle'."
                              << "\n";
                    error = true;
                    falseAttribute = true;
                } else {
                    attributes.emplace_back(attribute.name());
                }
            }

            if (falseAttribute) continue;

            // check missing attributes
            if (attributes.size() != requiredAttributes.at(EObjectTypes::kVehicle).size()) {
                errStream << "[XmlValidator] Not all required attributes for type 'vehicle' are set (required: ";
                for (std::vector<std::string>::const_iterator it =
                       requiredAttributes.at(EObjectTypes::kVehicle).begin();
                     it != requiredAttributes.at(EObjectTypes::kVehicle).end(); ++it) {
                    if (std::next(it) == requiredAttributes.at(EObjectTypes::kVehicle).end())
                        errStream << *it << ")."
                                  << "\n";
                    else
                        errStream << *it << ", ";
                }
                error = true;
                continue;
            }

            // extract attributes
            const std::string road = object.child("baan").text().as_string();
            const std::string pos = object.child("positie").text().as_string();
            const std::string type = object.child("type").text().as_string();

            // place attributes in map for export
            config[EObjectTypes::kVehicle].push_back({{EAttributes(EVehicleAttributes::kRoadName), road},
                                                      {EAttributes(EVehicleAttributes::kPosition), pos},
                                                      {EAttributes(EVehicleAttributes::kType), type}});

        } else if (std::string(object.name()) == "VOERTUIGGENERATOR") {
            // check only valid attributes are set
            bool falseAttribute = false;
            std::vector<std::string> attributes;
            for (const pugi::xml_node &attribute : object.children()) {
                if (std::find(requiredAttributes.at(EObjectTypes::kVehicleGenerator).begin(),
                              requiredAttributes.at(EObjectTypes::kVehicleGenerator).end(),
                              attribute.name()) == requiredAttributes.at(EObjectTypes::kVehicleGenerator).end()) {
                    errStream << "[XmlValidator] Attribute '" << attribute.name()
                              << "' is not known for object type 'vehicleGenerator'."
                              << "\n";
                    error = true;
                    falseAttribute = true;
                } else if (std::find(attributes.begin(), attributes.end(), attribute.name()) != attributes.end()) {
                    errStream << "[XmlValidator] Attribute '" << attribute.name()
                              << "' is already set and cannot be set again for type 'vehicleGenerator'."
                              << "\n";
                    error = true;
                    falseAttribute = true;
                } else {
                    attributes.emplace_back(attribute.name());
                }
            }

            if (falseAttribute) continue;

            bool missingAttributes = false;
            // check missing attributes
            if (attributes.size() != requiredAttributes.at(EObjectTypes::kVehicleGenerator).size()) {
                errStream
                  << "[XmlValidator] Not all required attributes for type 'vehicleGenerator' are set (required: ";
                for (std::vector<std::string>::const_iterator it =
                       requiredAttributes.at(EObjectTypes::kVehicleGenerator).begin();
                     it != requiredAttributes.at(EObjectTypes::kVehicleGenerator).end(); ++it) {
                    if (std::next(it) == requiredAttributes.at(EObjectTypes::kVehicleGenerator).end())
                        errStream << *it << ")."
                                  << "\n";
                    else
                        errStream << *it << ", ";
                }
                missingAttributes = true;
                error = true;
                continue;
            }

            if (missingAttributes) continue;

            // extract attributes
            const std::string road = object.child("baan").text().as_string();
            const std::string freq = object.child("frequentie").text().as_string();
            const std::string type = object.child("type").text().as_string();

            // place attributes in map for export
            config[EObjectTypes::kVehicleGenerator].push_back(
              {{EAttributes(EVehicleGeneratorAttributes::kRoadName), road},
               {EAttributes(EVehicleGeneratorAttributes::kFrequency), freq},
               {EAttributes(EVehicleGeneratorAttributes::kType), type}});

        } else if (std::string(object.name()) == "BUSHALTE") {
            // check only valid attributes are set
            bool falseAttribute = false;
            std::vector<std::string> attributes;
            for (const pugi::xml_node &attribute : object.children()) {
                // check if attribute can be set (required or optional)
                if (std::find(requiredAttributes.at(EObjectTypes::kBusStop).begin(),
                              requiredAttributes.at(EObjectTypes::kBusStop).end(),
                              attribute.name()) == requiredAttributes.at(EObjectTypes::kBusStop).end()) {
                    errStream << "[XmlValidator] Attribute '" << attribute.name()
                              << "' is not known for object type 'busstop'."
                              << "\n";
                    error = true;
                    falseAttribute = true;
                } else if (std::find(attributes.begin(), attributes.end(), attribute.name()) != attributes.end()) {
                    errStream << "[XmlValidator] Attribute '" << attribute.name()
                              << "' is already set and cannot be set again for type 'busstop'."
                              << "\n";
                    error = true;
                    falseAttribute = true;
                } else {
                    attributes.emplace_back(attribute.name());
                }
            }

            if (falseAttribute) continue;

            // check missing attributes
            bool missingAttributes = false;
            for (std::vector<std::string>::const_iterator it = requiredAttributes.at(EObjectTypes::kBusStop).begin();
                 it != requiredAttributes.at(EObjectTypes::kBusStop).end(); ++it) {
                if (std::find(attributes.begin(), attributes.end(), *it) == attributes.end()) {
                    errStream << "[XmlValidator] Not all required attributes for type 'busstop' are set (required: ";
                    for (std::vector<std::string>::const_iterator itTwo =
                           requiredAttributes.at(EObjectTypes::kBusStop).begin();
                         itTwo != requiredAttributes.at(EObjectTypes::kBusStop).end(); ++itTwo) {
                        if (std::next(itTwo) == requiredAttributes.at(EObjectTypes::kBusStop).end())
                            errStream << *itTwo << ")."
                                      << "\n";
                        else
                            errStream << *itTwo << ", ";
                    }
                    error = true;
                    missingAttributes = true;
                    break;
                }
            }

            if (missingAttributes) continue;

            // extract required attributes
            const std::string road = object.child("baan").text().as_string();
            const std::string pos = object.child("positie").text().as_string();
            const std::string haltTime = object.child("wachttijd").text().as_string();

            // place attributes in map for export
            config[EObjectTypes::kBusStop].push_back({{EAttributes(EBusStopAttributes::kRoadName), road},
                                                      {EAttributes(EBusStopAttributes::kPosition), pos},
                                                      {EAttributes(EBusStopAttributes::kHaltTime), haltTime}});

        } else if (std::string(object.name()) == "KRUISPUNT") {
            bool falseAttribute = false;
            // check if lights need to be added
            bool lights = false;
            std::string lightCycle;
            std::string posRoadOne;
            std::string posRoadTwo;
            std::string roadOne;
            std::string roadTwo;

            // check only valid attributes are set
            unsigned int count = 0;
            for (const pugi::xml_node &attribute : object.children()) {
                if (attribute.name() == std::string("baan")) {
                    // check that a position is set
                    if (attribute.attributes_begin()->name() != std::string("positie")) {
                        errStream
                          << "[XmlValidator] CrossRoad objects requires two roads with a position as attribute, not: '"
                          << attribute.attributes_begin()->name() << "' as attribute." << '\n';
                        error = true;
                        falseAttribute = true;
                    } else {
                        if (count == 0) {
                            posRoadOne = attribute.attributes_begin()->value();
                            roadOne = attribute.text().as_string();
                        } else if (count == 1) {
                            posRoadTwo = attribute.attributes_begin()->value();
                            roadTwo = attribute.text().as_string();
                        }
                    }
                    ++count;
                } else if (attribute.name() == std::string("lichten")) {
                    lights = true;
                    lightCycle = object.child("lichten").text().as_string();
                } else {
                    errStream << "[XmlValidator] CrossRoad object requires two roads, not: '" << attribute.name()
                              << "'.\n";
                    error = true;
                    falseAttribute = true;
                }
            }

            if (count != 2) {
                errStream << "[XmlValidator] CrossRoad object requires two roads, currently has '" << count
                          << "' roads.\n";
                error = true;
                falseAttribute = true;
            }

            if (falseAttribute) continue;

            // place attributes in map for export

            config[EObjectTypes::kCrossRoad].push_back(
              {{EAttributes(ECrossRoadAttributes::kFirstRoad), roadOne},
               {EAttributes(ECrossRoadAttributes::kSecondRoad), roadTwo},
               {EAttributes(ECrossRoadAttributes::kFirstRoadPosition), posRoadOne},
               {EAttributes(ECrossRoadAttributes::kSecondRoadPosition), posRoadTwo},
               {EAttributes(ECrossRoadAttributes::kLights), (lights) ? lightCycle : "-1"}});

        } else {
            errStream << "[XmlValidator] The object '" << object.name() << "' cannot be parsed."
                      << "\n";
        }
    }

    // return if attributes are not valid
    if (error) return {};

    // Validate map
    std::vector<std::string> validatedRoads;
    /* To validate the map the following order is maintained: roads > lights > vehicles */

    // ==== go over all roads ====
    std::vector<Object> roads;
    bool found = true;
    try {
        roads = config.at(EObjectTypes::kRoad);
    } catch (const std::exception &e) { found = false; }
    if (found) {
        for (const Object &road : roads) {
            /* For the roads only the types need to validated */
            try {
                // check if road length is a valid integer
                const int length = Utils::stoi(road.at(EAttributes(ERoadAttributes::kLength)));
                if (length <= 0) throw std::exception();
                validatedRoads.push_back(road.at(EAttributes(ERoadAttributes::kName)));
            } catch (std::exception &e) {
                errStream << "[XmlValidator] road length is not a valid unsigned integer (length: "
                          << road.at(EAttributes(ERoadAttributes::kLength)) << ")."
                          << "\n";
                error = true;
            }
        }
    }

    // ==== validate the lights ====
    std::vector<Object> lights;
    std::unordered_map<std::string, std::vector<double>> lightPositions;
    found = true;
    try {
        lights = config.at(EObjectTypes::kLight);
    } catch (const std::exception &e) { found = false; }
    if (found) {
        for (const Object &light : lights) {
            /* Lights can only be spawned on an existing road and within the roads length. The types also need
             * validating */
            const std::string roadName = light.at(EAttributes(ELightAttributes::kRoadName));
            const std::string positionString = light.at(EAttributes(ELightAttributes::kPosition));
            const std::string cycleTimeString = light.at(EAttributes(ELightAttributes::kCycleTime));

            // check types before checking semantic validity
            bool incorrectType = false;

            // check that the position type is valid
            int position;
            try {
                position = Utils::stoi(positionString);
                if (position < 0) throw std::exception();
            } catch (const std::exception &e) {
                errStream << "[XmlValidator] Cannot create light at a position that is not an integer or that is "
                             "signed (position: "
                          << positionString << ")."
                          << "\n";
                error = true;
                incorrectType = true;
            }

            // check that the cycleTime type is valid
            int cycleTime;
            try {
                cycleTime = Utils::stoi(cycleTimeString);
                if (cycleTime < 0) throw std::exception();
            } catch (const std::exception &e) {
                errStream << "[XmlValidator] Cannot create light with a cycle time that is not an integer or that is "
                             "signed (cycleTime: "
                          << cycleTimeString << ")."
                          << "\n";
                error = true;
                incorrectType = true;
            }

            if (incorrectType) continue;

            // check that the road exists
            if (std::find(validatedRoads.begin(), validatedRoads.end(), roadName) != validatedRoads.end()) {
                // check that the spawn happens within road length
                const Object road = findRoadWithName(roadName, config);
                if (Utils::stoi(road.at(EAttributes(ERoadAttributes::kLength))) < position) {
                    errStream
                      << "[XmlValidator] Cannot create light at a position that is outside the road (road length: "
                      << road.at(EAttributes(ERoadAttributes::kLength)) << "; position: " << positionString << ")."
                      << "\n";
                    error = true;
                    // check that lights are not spawned in eachothers stop distance
                } else {
                    bool validPosition = true;
                    for (std::vector<double>::const_iterator it = lightPositions[roadName].begin();
                         it != lightPositions[roadName].end(); ++it) {
                        if (!(position <= *it - 50 || position >= *it)) {
                            errStream << "[XmlValidator] Cannot create light at a position that is inside the brake "
                                         "distance of another light (newPosition: "
                                      << position << "; otherPosition: " << *it << "; MinDistance: 50"
                                      << ").\n";
                            error = true;
                            validPosition = false;
                        }
                    }

                    if (validPosition) lightPositions[roadName].push_back(position);
                }
            } else {
                errStream << "[XmlValidator] Cannot create light on road that does not exist or that is not valid: on '"
                          << roadName << "' at position " << positionString << '.' << "\n";
                error = true;
            }
        }
    }

    std::vector<Object> vehicles;
    found = true;
    try {
        vehicles = config.at(EObjectTypes::kVehicle);
    } catch (const std::exception &e) { found = false; }

    if (found) {
        // ==== validate the vehicles ====
        for (const Object &vehicle : vehicles) {
            /* Vehicles can only be spawned on an existing road and within the roads length. The types also need
             * validating */
            const std::string roadName = vehicle.at(EAttributes(EVehicleAttributes::kRoadName));
            const std::string positionString = vehicle.at(EAttributes(EVehicleAttributes::kPosition));
            const std::string type = vehicle.at(EAttributes(EVehicleAttributes::kType));

            // check types before checking semantic validity
            bool incorrectType = false;

            // check that the types are valid
            int position;
            try {
                position = Utils::stoi(positionString);
                if (position < 0) throw std::exception();

            } catch (const std::exception &e) {
                errStream << "[XmlValidator] Cannot create vehicle at a position that is not an integer or that is "
                             "signed (position: "
                          << positionString << ")."
                          << "\n";
                incorrectType = true;
                error = true;
            }

            // check that the type of vehicle exists
            if (std::find(vehicleTypes.begin(), vehicleTypes.end(), type) == vehicleTypes.end()) {
                errStream << "[XmlValidator] Cannot create vehicle of type: '" << type << "'. Allowed types are: ";
                for (std::vector<std::string>::const_iterator it = vehicleTypes.begin(); it != vehicleTypes.end();
                     ++it) {
                    if (std::next(it) == vehicleTypes.end()) {
                        errStream << '\'' << *it << "'."
                                  << "\n";
                    } else {
                        errStream << '\'' << *it << "', ";
                    }
                }
                incorrectType = true;
                error = true;
            }

            if (incorrectType) continue;

            // check that the road exists
            if (std::find(validatedRoads.begin(), validatedRoads.end(), roadName) != validatedRoads.end()) {
                // check that the spawn happens within road length
                const Object road = findRoadWithName(roadName, config);
                if (Utils::stoi(road.at(EAttributes(ERoadAttributes::kLength))) < position) {
                    errStream
                      << "[XmlValidator] Cannot create vehicle at a position that is outside the road (road length: "
                      << road.at(EAttributes(ERoadAttributes::kLength)) << "; position: " << positionString << ")."
                      << "\n";
                    error = true;
                }
            } else {
                errStream
                  << "[XmlValidator] Cannot create vehicle on road that does not exist or that is not valid: on '"
                  << roadName << "' at " << positionString << '.' << "\n";
                error = true;
            }
        }
    }

    // ==== validate the vehicleGenerators ====
    std::vector<Object> vehicleGenerators;
    found = true;
    try {
        vehicleGenerators = config.at(EObjectTypes::kVehicleGenerator);
    } catch (const std::exception &e) { found = false; }
    if (found) {
        for (const Object &vehicleGenerator : vehicleGenerators) {
            /* Vehicles can only be spawned on an existing road and within the roads length. The types also need
             * validating */
            const std::string roadName = vehicleGenerator.at(EAttributes(EVehicleGeneratorAttributes::kRoadName));
            const std::string frequencyString =
              vehicleGenerator.at(EAttributes(EVehicleGeneratorAttributes::kFrequency));
            const std::string type = vehicleGenerator.at(EAttributes(EVehicleGeneratorAttributes::kType));

            // check types before checking semantic validity
            bool incorrectType = false;

            // check that the types are valid
            int frequency;
            try {
                frequency = Utils::stoi(frequencyString);
                if (frequency <= 0) throw std::exception();

            } catch (const std::exception &e) {
                errStream << "[XmlValidator] Cannot create vehicleGenerator with a frequency that is not an integer or "
                             "that is less or equal to zero (frequency: "
                          << frequencyString << ")."
                          << "\n";
                incorrectType = true;
                error = true;
            }

            // check that the type of vehicle exists
            if (std::find(vehicleTypes.begin(), vehicleTypes.end(), type) == vehicleTypes.end()) {
                errStream << "[XmlValidator] Cannot create vehicle of type: '" << type << "'. Allowed types are: ";
                for (std::vector<std::string>::const_iterator it = vehicleTypes.begin(); it != vehicleTypes.end();
                     ++it) {
                    if (std::next(it) == vehicleTypes.end()) {
                        errStream << '\'' << *it << "'."
                                  << "\n";
                    } else {
                        errStream << '\'' << *it << "', ";
                    }
                }
                incorrectType = true;
                error = true;
            }

            if (incorrectType) continue;

            // check that the road exists
            if (std::find(validatedRoads.begin(), validatedRoads.end(), roadName) == validatedRoads.end()) {
                errStream << "[XmlValidator] Cannot create vehicleGenerator on road that does not exist or that is not "
                             "valid: on '"
                          << roadName << "'."
                          << "\n";
                error = true;
            }
        }
    }

    // ==== validate the busstops ====
    std::vector<Object> busstops;
    found = true;
    try {
        busstops = config.at(EObjectTypes::kBusStop);
    } catch (const std::exception &e) { found = false; }
    if (found) {
        for (const Object &busstop : busstops) {
            /* Lights can only be spawned on an existing road and within the roads length. The types also need
             * validating */
            const std::string roadName = busstop.at(EAttributes(EBusStopAttributes::kRoadName));
            const std::string positionString = busstop.at(EAttributes(EBusStopAttributes::kPosition));
            const std::string haltTimeString = busstop.at(EAttributes(EBusStopAttributes::kHaltTime));

            // check types before checking semantic validity
            bool incorrectType = false;

            // check that the position type is valid
            int position;
            try {
                position = Utils::stoi(positionString);
                if (position < 0) throw std::exception();
            } catch (const std::exception &e) {
                errStream << "[XmlValidator] Cannot create busstop at a position that is not an integer or that is "
                             "signed (position: "
                          << positionString << ")."
                          << "\n";
                error = true;
                incorrectType = true;
            }

            // check that the haltTime type is valid
            int haltTime;
            try {
                haltTime = Utils::stoi(haltTimeString);
                if (haltTime < 0) throw std::exception();
            } catch (const std::exception &e) {
                errStream << "[XmlValidator] Cannot create busstop with a halt time that is not an integer or that is "
                             "signed (haltTime: "
                          << haltTimeString << ")."
                          << "\n";
                error = true;
                incorrectType = true;
            }

            if (incorrectType) continue;

            // check that the road exists
            if (std::find(validatedRoads.begin(), validatedRoads.end(), roadName) != validatedRoads.end()) {
                // check that the spawn happens within road length
                const Object road = findRoadWithName(roadName, config);
                if (Utils::stoi(road.at(EAttributes(ERoadAttributes::kLength))) < position) {
                    errStream
                      << "[XmlValidator] Cannot create busstop at a position that is outside the road (road length: "
                      << road.at(EAttributes(ERoadAttributes::kLength)) << "; position: " << positionString << ")."
                      << "\n";
                    error = true;
                }
            } else {
                errStream
                  << "[XmlValidator] Cannot create busstop on road that does not exist or that is not valid: on '"
                  << roadName << "' at position " << positionString << '.' << "\n";
                error = true;
            }
        }
    }

    // ==== Validate the crossroads ====
    std::vector<Object> crossRoads;
    found = true;
    try {
        crossRoads = config.at(EObjectTypes::kCrossRoad);
    } catch (const std::exception &e) { found = false; }
    if (found) {
        for (const Object &crossRoad : crossRoads) {
            /* Lights can only be spawned on an existing road and within the roads length. The types also need
             * validating */
            const std::string roadOne = crossRoad.at(EAttributes(ECrossRoadAttributes::kFirstRoad));
            const std::string roadTwo = crossRoad.at(EAttributes(ECrossRoadAttributes::kSecondRoad));
            const std::string posRoadOneString = crossRoad.at(EAttributes(ECrossRoadAttributes::kFirstRoadPosition));
            const std::string posRoadTwoString = crossRoad.at(EAttributes(ECrossRoadAttributes::kSecondRoadPosition));
            const std::string cycleTimeString = crossRoad.at(EAttributes(ECrossRoadAttributes::kLights));

            // check types before checking semantic validity
            bool incorrectType = false;

            // check that the position type is valid
            int firstRoadPosition;
            try {
                firstRoadPosition = Utils::stoi(posRoadOneString);
                if (firstRoadPosition < 0) throw std::exception();
            } catch (const std::exception &e) {
                errStream << "[XmlValidator] Cannot create CrossRoad on road '" << roadOne
                          << "' at a position that is not an integer or that is "
                             "signed (position: "
                          << posRoadOneString << ")."
                          << "\n";
                error = true;
                incorrectType = true;
            }

            // check that the position type is valid
            int secondRoadPosition;
            try {
                secondRoadPosition = Utils::stoi(posRoadTwoString);
                if (secondRoadPosition < 0) throw std::exception();
            } catch (const std::exception &e) {
                errStream << "[XmlValidator] Cannot create CrossRoad on road '" << roadTwo
                          << "' at a position that is not an integer or that is "
                             "signed (position: "
                          << posRoadTwoString << ")."
                          << "\n";
                error = true;
                incorrectType = true;
            }

            if (incorrectType) continue;

            if (roadOne == roadTwo) {
                errStream << "[XmlValidator] Cannot create CrossRoad with duplicate roads. (roadName: " << roadOne
                          << ")\n";
                continue;
            }

            if (cycleTimeString != "-1") {
                // check that the lights are at valid positions
                //// road one
                for (std::vector<double>::const_iterator it = lightPositions[roadOne].begin();
                     it != lightPositions[roadOne].end(); ++it) {
                    if (!(firstRoadPosition - 1 <= *it - 50 || firstRoadPosition - 1 >= *it)) {
                        errStream << "[XmlValidator] Cannot create light attached to crossroad at a position that is "
                                     "inside the brake "
                                     "distance of another light (newPosition: "
                                  << firstRoadPosition - 1 << "; otherPosition: " << *it << "; MinDistance: 50"
                                  << ").\n";
                        error = true;
                    }
                }

                //// road two
                for (std::vector<double>::const_iterator it = lightPositions[roadTwo].begin();
                     it != lightPositions[roadTwo].end(); ++it) {
                    if (!(secondRoadPosition - 1 <= *it - 50 || secondRoadPosition - 1 >= *it)) {
                        errStream << "[XmlValidator] Cannot create light attached to crossroad at a position that is "
                                     "inside the brake "
                                     "distance of another light (newPosition: "
                                  << secondRoadPosition - 1 << "; otherPosition: " << *it << "; MinDistance: 50"
                                  << ").\n";
                        error = true;
                    }
                }

                int cycleTime;
                try {
                    cycleTime = Utils::stoi(cycleTimeString);
                    if (cycleTime < 0) throw std::exception();
                } catch (const std::exception &e) {
                    errStream << "[XmlValidator] Cannot create light on a crossroad with a cycle time that is not an "
                                 "integer or that is "
                                 "signed (cycleTime: "
                              << cycleTimeString << ")."
                              << "\n";
                    error = true;
                }
            }

            // check that the road exists
            if (std::find(validatedRoads.begin(), validatedRoads.end(), roadOne) != validatedRoads.end()) {
                // check that the spawn happens within road length
                const Object road = findRoadWithName(roadOne, config);
                if (Utils::stoi(road.at(EAttributes(ERoadAttributes::kLength))) < firstRoadPosition) {
                    errStream
                      << "[XmlValidator] Cannot create CrossRoad at a position that is outside the road (road length: "
                      << road.at(EAttributes(ERoadAttributes::kLength)) << "; roadName: " << roadOne
                      << "; position: " << posRoadOneString << ")."
                      << "\n";
                    error = true;
                }
            } else {
                errStream
                  << "[XmlValidator] Cannot create CrossRoad on road that does not exist or that is not valid: on '"
                  << roadOne << "' at position " << posRoadOneString << '.' << "\n";
                error = true;
            }

            // check that the road exists
            if (std::find(validatedRoads.begin(), validatedRoads.end(), roadTwo) != validatedRoads.end()) {
                // check that the spawn happens within road length
                const Object road = findRoadWithName(roadTwo, config);
                if (Utils::stoi(road.at(EAttributes(ERoadAttributes::kLength))) < secondRoadPosition) {
                    errStream
                      << "[XmlValidator] Cannot create CrossRoad at a position that is outside the road (road length: "
                      << road.at(EAttributes(ERoadAttributes::kLength)) << "; roadName: " << roadTwo
                      << "; position: " << posRoadTwoString << ")."
                      << "\n";
                    error = true;
                }
            } else {
                errStream
                  << "[XmlValidator] Cannot create CrossRoad on road that does not exist or that is not valid: on '"
                  << roadTwo << "' at position " << posRoadTwoString << '.' << "\n";
                error = true;
            }
        }
    }

    if (!error && config.empty()) {
        errStream << "[XmlValidator] Input stream must contain at least one road."
                  << "\n";
    }

    if (!error) { return config; }

    return {};
}

Validator::Object Validator::findRoadWithName(const std::string &name, const ValMap &config) {
    const std::vector<Object> roads = config.at(EObjectTypes::kRoad);
    for (const Object &road : roads) {
        if (road.at(EAttributes(ERoadAttributes::kName)) == name) return road;
    }
    return {};
}
