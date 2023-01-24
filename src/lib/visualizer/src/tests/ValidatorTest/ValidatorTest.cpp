//============================================================================
// Name        : ValidatorTest.cpp
// Description : Test file of the validator class
// Author      : "Jonas Caluwé" <Jonas.Caluwe@student.uantwerpen.be> &&
//               "Gilles Van pellicom" <Gilles.Vanpellicom@student.uantwerpen.be>
// Date        : 2022/03/04
// Version     : 1.0
//============================================================================

#include <gtest/gtest.h>

#include <fstream>

#include "../../lib/xml-validator/Validator.h"

TEST(ValidatorTest, OutputValmapNotEmpty0) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream file(kResPath + "test0.xml");

    Validator::ValMap result = Validator::validate((std::istream &) file, (std::ostream &) dummyStream);
    EXPECT_NE(Validator::ValMap(), result);
}

TEST(ValidatorTest, OutputValmapNotEmpty1) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream file(kResPath + "test9.xml");

    Validator::ValMap result = Validator::validate((std::istream &) file, (std::ostream &) dummyStream);
    EXPECT_NE(Validator::ValMap(), result);
}

TEST(ValidatorTest, OutputValmapNotEmpty2) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream file(kResPath + "test12.xml");

    Validator::ValMap result = Validator::validate((std::istream &) file, (std::ostream &) dummyStream);
    EXPECT_NE(Validator::ValMap(), result);
}

TEST(ValidatorTest, OutputValmapNotEmpty3) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream file(kResPath + "test13.xml");

    Validator::ValMap result = Validator::validate((std::istream &) file, (std::ostream &) dummyStream);
    EXPECT_NE(Validator::ValMap(), result);
}

TEST(ValidatorTest, OutputValmapNotEmpty4) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream file(kResPath + "test19.xml");

    Validator::ValMap result = Validator::validate((std::istream &) file, (std::ostream &) dummyStream);
    EXPECT_NE(Validator::ValMap(), result);
}

TEST(ValidatorTest, OutputValmapNotEmpty5) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream file(kResPath + "test24.xml");

    Validator::ValMap result = Validator::validate((std::istream &) file, (std::ostream &) dummyStream);
    EXPECT_NE(Validator::ValMap(), result);
}

TEST(ValidatorTest, ExpectedOutputCompare0) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream file(kResPath + "test0.xml");

    Validator::ValMap result = Validator::validate((std::istream &) file, (std::ostream &) dummyStream);
    Validator::ValMap expected(
      {{Validator::EObjectTypes::kRoad,
        {{{Validator::ERoadAttributes::kName, "Middelheimlaan"}, {Validator::ERoadAttributes::kLength, "500"}}}},
       {Validator::EObjectTypes::kLight,
        {{{Validator::ELightAttributes::kRoadName, "Middelheimlaan"},
          {Validator::ELightAttributes::kPosition, "400"},
          {Validator::ELightAttributes::kCycleTime, "20"}}}},
       {Validator::EObjectTypes::kVehicle,
        {{{Validator::EVehicleAttributes::kRoadName, "Middelheimlaan"},
          {Validator::EVehicleAttributes::kPosition, "20"},
          {Validator::EVehicleAttributes::kType, "auto"}},
         {{Validator::EVehicleAttributes::kRoadName, "Middelheimlaan"},
          {Validator::EVehicleAttributes::kPosition, "0"},
          {Validator::EVehicleAttributes::kType, "auto"}}}}});

    EXPECT_TRUE(expected == result);
}

TEST(ValidatorTest, ExpectedOutputCompare1) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream file(kResPath + "test1.xml");

    Validator::ValMap result = Validator::validate((std::istream &) file, (std::ostream &) dummyStream);
    Validator::ValMap expected(
      {{Validator::EObjectTypes::kRoad,
        {{{Validator::ERoadAttributes::kName, "Korte langneuslaan"}, {Validator::ERoadAttributes::kLength, "700"}},
         {{Validator::ERoadAttributes::kName, "Lange kortneuslaan"}, {Validator::ERoadAttributes::kLength, "200"}}}},
       {Validator::EObjectTypes::kLight,
        {
          {{Validator::ELightAttributes::kRoadName, "Korte langneuslaan"},
           {Validator::ELightAttributes::kPosition, "320"},
           {Validator::ELightAttributes::kCycleTime, "14"}},
          {{Validator::ELightAttributes::kRoadName, "Lange kortneuslaan"},
           {Validator::ELightAttributes::kPosition, "170"},
           {Validator::ELightAttributes::kCycleTime, "28"}},
          {{Validator::ELightAttributes::kRoadName, "Korte langneuslaan"},
           {Validator::ELightAttributes::kPosition, "200"},
           {Validator::ELightAttributes::kCycleTime, "7"}},
        }},
       {Validator::EObjectTypes::kVehicle,
        {{{Validator::EVehicleAttributes::kRoadName, "Korte langneuslaan"},
          {Validator::EVehicleAttributes::kPosition, "292"},
          {Validator::EVehicleAttributes::kType, "auto"}},
         {{Validator::EVehicleAttributes::kRoadName, "Lange kortneuslaan"},
          {Validator::EVehicleAttributes::kPosition, "192"},
          {Validator::EVehicleAttributes::kType, "auto"}}}}});

    EXPECT_TRUE(expected == result);
}

TEST(ValidatorTest, ExpectedEmptyValmapOnFail) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // stream to send error messages to when we are not interested in them
    std::ostream dummyStream(nullptr);

    const std::ifstream file(kResPath + "test2.xml");

    Validator::ValMap result = Validator::validate((std::istream &) file, dummyStream);

    EXPECT_EQ(Validator::ValMap(), result);
}

TEST(ValidatorTest, ExpectedErrorMessageCompare0) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test2.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare0.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare1) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test3.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare1.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare2) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test4.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare2.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare3) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test5.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare3.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare4) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test6.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare4.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare5) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test7.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare5.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare6) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test8.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare6.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare7) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test10.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare7.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare8) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test11.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare8.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare9) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test14.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare9.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare10) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test15.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare10.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare11) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test16.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare11.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare12) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test17.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare12.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare13) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test18.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare13.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare14) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test20.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare14.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare15) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test21.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare15.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare16) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test22.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare16.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare17) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test23.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare17.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    // std::ofstream tmp(kBasePath + "expected/ExpectedErrorMessageCompare17.txt");

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}

TEST(ValidatorTest, ExpectedErrorMessageCompare18) {
    const std::string kBasePath = std::string(__FILE__).substr(0, std::string(__FILE__).find_last_of('/')) + '/';
    const std::string kResPath = kBasePath + "res/";

    // load the xml file
    const std::ifstream xmlFile(kResPath + "test25.xml");

    // create stream to write the error messages to
    std::stringstream errOut("");

    // convert expectedErrFile ifstream to stringstream
    const std::ifstream expectedErrFile(kBasePath + "expected/ExpectedErrorMessageCompare18.txt");
    std::stringstream expectedErr;
    expectedErr << expectedErrFile.rdbuf();

    // std::ofstream tmp(kBasePath + "expected/ExpectedErrorMessageCompare18.txt");

    Validator::validate((std::istream &) xmlFile, (std::ostream &) errOut);

    EXPECT_EQ(expectedErr.str(), errOut.str());
}