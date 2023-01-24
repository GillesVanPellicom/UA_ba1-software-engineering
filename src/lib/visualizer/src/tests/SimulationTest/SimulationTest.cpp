//============================================================================
// Name        : SimulationTest.cpp
// Description : Test file of the simulation class
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/22
// Version     : 1.0
//============================================================================

#include <gtest/gtest.h>

#include <fstream>

#include "../../Simulation.h"
#include "../../lib/logger/Logger.h"

TEST(SimulationTest, HappyDay) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::stringstream errOut("");

    const std::ifstream file(kResPath + "test0.xml");

    Simulation sim((std::istream &) file, 1.0 / 60.0, errOut);

    for (unsigned int i = 0; i < 100; ++i) { sim.godTick(); }

    EXPECT_EQ("", errOut.str());
}

TEST(SimulationTest, ExpectedOutputCompare0) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test0.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare0.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 100; ++i) {
        sim.godTick();
        Logger::logSimpleOutput(sim, outputStream);
    }

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare1) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test1.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare1.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    Simulation sim((std::istream &) xmlFile, 60.0 / 1.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) {
        sim.godTick();
        Logger::logSimpleOutput(sim, outputStream);
    }

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare2) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test2.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare2.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) {
        sim.godTick();
        Logger::logSimpleOutput(sim, outputStream);
    }

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare3) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test3.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare3.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) {
        sim.godTick();
        Logger::logSimpleOutput(sim, outputStream);
    }

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare4) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test4.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare4.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    // std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare4.txt");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) {
        sim.godTick();
        Logger::logSimpleOutput(sim, outputStream);
    }

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare5) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test5.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare5.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    // std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare5.txt");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) {
        sim.godTick();
        Logger::logSimpleOutput(sim, outputStream);
    }

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare6) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test7.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare6-1.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    const std::ifstream expectedOutputFile2(kBasePath + "expected/ExpectedOutputCompare6-2.txt");
    std::stringstream expectedOutput2;
    expectedOutput2 << expectedOutputFile2.rdbuf();

    std::stringstream outputStream("");

    //  std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare6-2.txt");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 1500; ++i) {
        sim.godTick();
        Logger::logSimpleOutput(sim, outputStream);
    }

    EXPECT_TRUE(expectedOutput.str() == outputStream.str() || outputStream.str() == expectedOutput2.str());
}

TEST(SimulationTest, vehicleOnRoads0) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test6.xml");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 5; ++i) { sim.godTick(); }

    EXPECT_TRUE(sim.getVehicles().find(5) != sim.getVehicles().end());
    EXPECT_TRUE(sim.getWorld().find(5) != sim.getWorld().end());
}

TEST(SimulationTest, ExpectedOutputCompare7) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test8.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare7.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    // std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare7.txt");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) {
        sim.godTick();
        Logger::logSimpleOutput(sim, outputStream);
    }

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare8) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test0.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare8.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    // std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare8.txt");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 100; ++i) { sim.godTick(); }

    Logger::logAdvancedOutput(sim, outputStream);

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare9) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test1.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare9.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    // std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare9.txt");

    Simulation sim((std::istream &) xmlFile, 60.0 / 1.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) { sim.godTick(); }

    Logger::logAdvancedOutput(sim, outputStream);

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare10) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test2.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare10.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    // std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare10.txt");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) { sim.godTick(); }

    Logger::logAdvancedOutput(sim, outputStream);

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare11) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test3.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare11.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    // std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare11.txt");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) { sim.godTick(); }

    Logger::logAdvancedOutput(sim, outputStream);

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare12) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test4.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare12.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    // std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare12.txt");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) { sim.godTick(); }

    Logger::logAdvancedOutput(sim, outputStream);

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare13) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test5.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare13.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    // std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare13.txt");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) { sim.godTick(); }

    Logger::logAdvancedOutput(sim, outputStream);

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare14) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test8.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare14.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    // std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare14.txt");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) { sim.godTick(); }

    Logger::logAdvancedOutput(sim, outputStream);

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}

TEST(SimulationTest, ExpectedOutputCompare15) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream xmlFile(kResPath + "test8.xml");

    const std::ifstream expectedOutputFile(kBasePath + "expected/ExpectedOutputCompare15.txt");
    std::stringstream expectedOutput;
    expectedOutput << expectedOutputFile.rdbuf();

    std::stringstream outputStream("");

    // std::ofstream tmp(kBasePath + "expected/ExpectedOutputCompare15.txt");

    Simulation sim((std::istream &) xmlFile, 1.0 / 60.0, dummyStream);

    for (unsigned int i = 0; i < 4000; ++i) { sim.godTick(); }

    Logger::logAdvancedOutput(sim, outputStream);

    EXPECT_EQ(expectedOutput.str(), outputStream.str());
}