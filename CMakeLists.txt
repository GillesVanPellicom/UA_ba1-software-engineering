cmake_minimum_required(VERSION 3.6)
project(se-project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Include gtest directory
include_directories(src/lib/gtest/include)
link_directories(src/lib/gtest/lib)

# ======== Load test files for Test target ======
file(GLOB_RECURSE TEST_SOURCE_FILES src/tests/**/*.cpp)

# ======== Set source files of libraries and classes ========
## Include Main Simulation class
set(SIMULATION_SOURCE_FILES src/Simulation.h src/Simulation.cpp)

## Include PugiXml source files
AUX_SOURCE_DIRECTORY(src/lib/pugixml PUGIXML_SOURCE_FILES)

## Include XmlValidator source files
AUX_SOURCE_DIRECTORY(src/lib/xml-validator XMLVALIDATOR_SOURCE_FILES)

## Include Utils source files
AUX_SOURCE_DIRECTORY(src/lib/utils UTILS_SOURCE_FILES)

## Include mini source files
AUX_SOURCE_DIRECTORY(src/lib/mini MINI_SOURCE_FILES)

## Include path source files
AUX_SOURCE_DIRECTORY(src/lib/path PATH_SOURCE_FILES)

# Include logger source files
AUX_SOURCE_DIRECTORY(src/lib/logger LOGGER_SOURCE_FILES)

## Include objects source files
set(OBJECT_SOURCE_FILES
        src/objects/Object.cpp
        src/objects/Object.h
        src/objects/road/RoadObject.h
        src/objects/road/RoadObject.cpp
        src/objects/entities/Entity.cpp
        src/objects/entities/Entity.h
        src/objects/entities/light/LightEntity.cpp
        src/objects/entities/light/LightEntity.h
        src/objects/entities/vehicle/VehicleEntity.cpp
        src/objects/entities/vehicle/VehicleEntity.h
        src/objects/entities/vehicleGenerator/VehicleGeneratorEntity.cpp
        src/objects/entities/vehicleGenerator/VehicleGeneratorEntity.h
        src/objects/entities/busStop/BusStopEntity.cpp
        src/objects/entities/busStop/BusStopEntity.h
        src/objects/crossroad/CrossRoadObject.cpp
        src/objects/crossroad/CrossRoadObject.h
        )

# ======= Create targets ========
## Set source files for RELEASE target
set(
        RELEASE_SOURCE_FILES
        src/SimulationMain.cpp
        ${SIMULATION_SOURCE_FILES}
        ${OBJECT_SOURCE_FILES}
        ${UTILS_SOURCE_FILES}
        ${PUGIXML_SOURCE_FILES}
        ${XMLVALIDATOR_SOURCE_FILES}
        ${MINI_SOURCE_FILES}
        ${PATH_SOURCE_FILES}
        ${LOGGER_SOURCE_FILES}
)

## Set source files for TEST target
set(
        TEST_SOURCE_FILES
        src/SimulationTest.cpp
        ${SIMULATION_SOURCE_FILES}
        ${OBJECT_SOURCE_FILES}
        ${TEST_SOURCE_FILES}
        ${UTILS_SOURCE_FILES}
        ${PUGIXML_SOURCE_FILES}
        ${XMLVALIDATOR_SOURCE_FILES}
        ${PATH_SOURCE_FILES}
        ${LOGGER_SOURCE_FILES}
)

# Create RELEASE target
add_executable(sim ${RELEASE_SOURCE_FILES})

# Create TEST target
add_executable(sim_test ${TEST_SOURCE_FILES})


# ======= Link gtest library ========
target_link_libraries(sim_test gtest)
