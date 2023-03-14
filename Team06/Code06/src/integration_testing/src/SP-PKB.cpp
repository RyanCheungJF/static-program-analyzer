#include <filesystem>

#include "../../spa/src/SP/SP.h"
#include "../../spa/src/utils/AppConstants.h"
#include "../../unit_testing/src/utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;

TEST_CASE("SP-PKB Integration: Valid Source Program 1") {
    SP testSP;
    PKB testPKB;
    WritePKB writePKB;
    ReadPKB readPKB;
    testPKB.initializePkb();
    writePKB.setInstancePKB(testPKB);
    readPKB.setInstancePKB(testPKB);

    auto testDirectory = std::filesystem::path(INTEGRATION_TESTING_DIR);
    for (int i = 0; i < 3; i++) {
        testDirectory = testDirectory.parent_path();
    }
    testDirectory /= "Tests06/sp/sp-pkb/";
    auto filePath = testDirectory.string() + "valid1.txt";
    testSP.processFile(filePath, &writePKB, &readPKB);

    SECTION("SP-PKB Integration: ModifiesP") {
        auto procedureNames = readPKB.getAllProcedureNames();
        auto expectedProcedureNames = std::unordered_set<Ent>({"A", "B", "C"});
        REQUIRE(procedureNames == expectedProcedureNames);

        auto variablesAModifies = readPKB.getModifiesP("A");
        auto expectedAModifies = std::unordered_set<Ent>({"x"});
        REQUIRE(variablesAModifies == expectedAModifies);

        auto variablesBModifies = readPKB.getModifiesP("B");
        auto expectedBModifies = std::unordered_set<Ent>({"y"});
        REQUIRE(variablesBModifies == expectedBModifies);

        auto variablesCModifies = readPKB.getModifiesP("C");
        auto expectedCModifies = std::unordered_set<Ent>({"z", "y"});
        REQUIRE(variablesCModifies == expectedCModifies);
    }

    SECTION("SP-PKB Integration: ModifiesS") {
        auto variables1Modifies = readPKB.getModifiesS(1);
        auto expected1Modifies = std::unordered_set<Ent>({"x"});
        REQUIRE(variables1Modifies == expected1Modifies);

        auto variables2Modifies = readPKB.getModifiesS(2);
        auto expected2Modifies = std::unordered_set<Ent>({"y"});
        REQUIRE(variables2Modifies == expected2Modifies);

        auto variables3Modifies = readPKB.getModifiesS(3);
        auto expected3Modifies = std::unordered_set<Ent>({"y"});
        REQUIRE(variables3Modifies == expected3Modifies);

        auto variables4Modifies = readPKB.getModifiesS(4);
        REQUIRE(variables4Modifies.empty());

        auto variables5Modifies = readPKB.getModifiesS(5);
        auto expected5Modifies = std::unordered_set<Ent>({"y"});
        REQUIRE(variables5Modifies == expected5Modifies);

        auto variables6Modifies = readPKB.getModifiesS(6);
        auto expected6Modifies = std::unordered_set<Ent>({"z"});
        REQUIRE(variables6Modifies == expected6Modifies);
    }

    SECTION("SP-PKB Integration: UsesP") {
        auto variablesAUses = readPKB.getUsesP("A");
        auto expectedAUses = std::unordered_set<Ent>({"x"});
        REQUIRE(variablesAUses == expectedAUses);

        auto variablesBUses = readPKB.getUsesP("B");
        auto expectedBUses = std::unordered_set<Ent>({"y"});
        REQUIRE(variablesBUses == expectedBUses);

        auto variablesCUses = readPKB.getUsesP("C");
        auto expectedCUses = std::unordered_set<Ent>({"z", "y"});
        REQUIRE(variablesCUses == expectedCUses);
    }

    SECTION("SP-PKB Integration: UsesS") {
        auto variables1Uses = readPKB.getUsesS(1);
        auto expected1Uses = std::unordered_set<Ent>({"x"});
        REQUIRE(variables1Uses == expected1Uses);

        auto variables2Uses = readPKB.getUsesS(2);
        auto expected2Uses = std::unordered_set<Ent>({"y"});
        REQUIRE(variables2Uses == expected2Uses);

        auto variables3Uses = readPKB.getUsesS(3);
        REQUIRE(variables3Uses.empty());

        auto variables4Uses = readPKB.getUsesS(4);
        auto expected4Uses = std::unordered_set<Ent>({"z"});
        REQUIRE(variables4Uses == expected4Uses);

        auto variables5Uses = readPKB.getUsesS(5);
        auto expected5Uses = std::unordered_set<Ent>({"y"});
        REQUIRE(variables5Uses == expected5Uses);

        auto variables6Uses = readPKB.getUsesS(6);
        auto expected6Uses = std::unordered_set<Ent>({"z"});
        REQUIRE(variables6Uses == expected6Uses);
    }

    SECTION("SP-PKB Integration: Follows") {
        std::vector<Parameter> params1 = {Parameter("1", AppConstants::FIXED_INT),
                                          Parameter("2", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> followsTest1 = Relationship::makeRelationship(AppConstants::FOLLOWS, params1);
        std::vector<std::vector<std::string>> follows1actual = readPKB.findRelationship(followsTest1);
        REQUIRE(follows1actual.empty());

        shared_ptr<Relationship> followsTest2 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("2", AppConstants::FIXED_INT), Parameter("3", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows2actual = readPKB.findRelationship(followsTest2);
        std::vector<std::vector<std::string>> follows2expected = {{"2", "3"}};
        REQUIRE(follows2expected == follows2actual);

        shared_ptr<Relationship> followsTest3 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("4", AppConstants::FIXED_INT), Parameter("5", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows3actual = readPKB.findRelationship(followsTest3);
        std::vector<std::vector<std::string>> follows3expected = {{"4", "5"}};
        REQUIRE(follows3expected == follows3actual);

        shared_ptr<Relationship> followsTest4 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("5", AppConstants::FIXED_INT), Parameter("6", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows4actual = readPKB.findRelationship(followsTest4);
        std::vector<std::vector<std::string>> follows4expected = {{"5", "6"}};
        REQUIRE(follows4expected == follows4actual);

        shared_ptr<Relationship> followsTest5 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("4", AppConstants::FIXED_INT), Parameter("6", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows5actual = readPKB.findRelationship(followsTest5);
        REQUIRE(follows5actual.empty());

        shared_ptr<Relationship> followsTest7 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("4", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> follows7actual = readPKB.findRelationship(followsTest7);
        std::vector<std::vector<std::string>> follows7expected = {{"4", "5"}};
        REQUIRE(follows7expected == follows7actual);

        shared_ptr<Relationship> followsTest8 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("_", AppConstants::WILDCARD), Parameter("6", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows8actual = readPKB.findRelationship(followsTest8);
        std::vector<std::vector<std::string>> follows8expected = {{"5", "6"}};
        REQUIRE(follows8expected == follows8actual);

        shared_ptr<Relationship> followsTest9 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("_", AppConstants::WILDCARD), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> follows9actual = readPKB.findRelationship(followsTest9);
        std::vector<std::vector<std::string>> follows9expected = {{"2", "3"}, {"4", "5"}, {"5", "6"}};
        std::sort(follows9actual.begin(), follows9actual.end());
        std::sort(follows9expected.begin(), follows9expected.end());
        REQUIRE(follows9expected == follows9actual);

        shared_ptr<Relationship> followsTest10 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("re", AppConstants::READ), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> follows10actual = readPKB.findRelationship(followsTest10);
        REQUIRE(follows10actual.empty());

        shared_ptr<Relationship> followsTest11 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("_", AppConstants::WILDCARD), Parameter("re", AppConstants::READ)});
        std::vector<std::vector<std::string>> follows11actual = readPKB.findRelationship(followsTest11);
        std::vector<std::vector<std::string>> follows11expected = {{"2", "3"}};
        REQUIRE(follows11expected == follows11actual);

        shared_ptr<Relationship> followsTest12 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("pn", AppConstants::PRINT), Parameter("ca", AppConstants::CALL)});
        std::vector<std::vector<std::string>> follows12actual = readPKB.findRelationship(followsTest12);
        std::vector<std::vector<std::string>> follows12expected = {{"4", "5"}};
        REQUIRE(follows12expected == follows12actual);

        shared_ptr<Relationship> followsTest13 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("s", AppConstants::STMT), Parameter("ca", AppConstants::CALL)});
        std::vector<std::vector<std::string>> follows13actual = readPKB.findRelationship(followsTest13);
        std::vector<std::vector<std::string>> follows13expected = {{"4", "5"}};
        REQUIRE(follows13expected == follows13actual);

        shared_ptr<Relationship> followsTest14 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("as", AppConstants::ASSIGN), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> follows14actual = readPKB.findRelationship(followsTest14);
        std::vector<std::vector<std::string>> follows14expected = {{"2", "3"}};
        REQUIRE(follows14expected == follows14actual);
    }

    SECTION("SP-PKB Integration: FollowsT") {
        shared_ptr<Relationship> followsTTest1 = Relationship::makeRelationship(
            AppConstants::FOLLOWST, {Parameter("4", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> followsT1actual = readPKB.findRelationship(followsTTest1);
        std::vector<std::vector<std::string>> followsT1expected = {{"4", "5"}, {"4", "6"}};
        std::sort(followsT1actual.begin(), followsT1actual.end());
        std::sort(followsT1expected.begin(), followsT1expected.end());
        REQUIRE(followsT1expected == followsT1actual);

        shared_ptr<Relationship> followsTTest2 = Relationship::makeRelationship(
            AppConstants::FOLLOWST, {Parameter("as", AppConstants::ASSIGN), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> followsT2actual = readPKB.findRelationship(followsTTest2);
        std::vector<std::vector<std::string>> followsT2expected = {{"2", "3"}};
        REQUIRE(followsT2expected == followsT2actual);

        shared_ptr<Relationship> followsTTest3 = Relationship::makeRelationship(
            AppConstants::FOLLOWST, {Parameter("s", AppConstants::STMT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> followsT3actual = readPKB.findRelationship(followsTTest3);
        std::vector<std::vector<std::string>> followsT3expected = {{"2", "3"}, {"4", "5"}, {"4", "6"}, {"5", "6"}};
        std::sort(followsT3actual.begin(), followsT3actual.end());
        std::sort(followsT3expected.begin(), followsT3expected.end());
        REQUIRE(followsT3expected == followsT3actual);

        shared_ptr<Relationship> followsTTest4 = Relationship::makeRelationship(
            AppConstants::FOLLOWST, {Parameter("pn", AppConstants::PRINT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> followsT4actual = readPKB.findRelationship(followsTTest4);
        std::vector<std::vector<std::string>> followsT4expected = {{"4", "5"}, {"4", "6"}};
        std::sort(followsT4actual.begin(), followsT4actual.end());
        std::sort(followsT4expected.begin(), followsT4expected.end());
        REQUIRE(followsT4expected == followsT4actual);
    }

    SECTION("SP-PKB Integration: Calls") {
        shared_ptr<Relationship> callsTest1 = Relationship::makeRelationship(
            AppConstants::CALLS, {Parameter("C", AppConstants::PROCEDURE), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> callsT1actual = readPKB.findRelationship(callsTest1);
        std::vector<std::vector<std::string>> callsT1expected = {{"C", "B"}};
        REQUIRE(callsT1actual == callsT1expected);
    }

    SECTION("SP-PKB Integration: CallsT") {
        shared_ptr<Relationship> callsTTest1 = Relationship::makeRelationship(
            AppConstants::CALLST, {Parameter("C", AppConstants::PROCEDURE), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> callsTT1actual = readPKB.findRelationship(callsTTest1);
        std::vector<std::vector<std::string>> callsTT1expected = {{"C", "B"}};
        REQUIRE(callsTT1actual == callsTT1expected);
    }

    SECTION("SP-PKB Integration: CFG") {
        auto cfgA = readPKB.getCFG("A");
        REQUIRE(cfgA[1]["parents"].empty());
        REQUIRE(cfgA[1]["children"].empty());

        auto cfgB = readPKB.getCFG("B");
        REQUIRE(cfgB[2]["parents"].empty());
        REQUIRE(cfgB[2]["children"] == std::unordered_set({3}));
        REQUIRE(cfgB[3]["parents"] == std::unordered_set({2}));
        REQUIRE(cfgB[3]["children"].empty());

        auto cfgC = readPKB.getCFG("C");
        REQUIRE(cfgC[4]["parents"].empty());
        REQUIRE(cfgC[4]["children"] == std::unordered_set({5}));
        REQUIRE(cfgC[5]["parents"] == std::unordered_set({4}));
        REQUIRE(cfgC[5]["children"] == std::unordered_set({6}));
        REQUIRE(cfgC[6]["parents"] == std::unordered_set({5}));
        REQUIRE(cfgC[6]["children"].empty());
    }

    SECTION("SP-PKB Integration: Next") {
        shared_ptr<Relationship> nextTest1 = Relationship::makeRelationship(
            AppConstants::NEXT, {Parameter("1", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> nextT1actual = readPKB.findRelationship(nextTest1);
        REQUIRE(nextT1actual.empty());

        shared_ptr<Relationship> nextTest2 = Relationship::makeRelationship(
            AppConstants::NEXT, {Parameter("2", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> nextT2actual = readPKB.findRelationship(nextTest2);
        std::vector<std::vector<std::string>> nextT2expected = {{"2", "3"}};
        REQUIRE(nextT2actual == nextT2expected);

        shared_ptr<Relationship> nextTest3 = Relationship::makeRelationship(
            AppConstants::NEXT, {Parameter("_", AppConstants::WILDCARD), Parameter("6", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> nextT3actual = readPKB.findRelationship(nextTest3);
        std::vector<std::vector<std::string>> nextT3expected = {{"5", "6"}};
        REQUIRE(nextT3actual == nextT3expected);
    }
}

TEST_CASE("SP-PKB Integration: Valid Source Program 2") {
    SP testSP;
    PKB testPKB;
    WritePKB writePKB;
    ReadPKB readPKB;
    testPKB.initializePkb();
    writePKB.setInstancePKB(testPKB);
    readPKB.setInstancePKB(testPKB);

    auto testDirectory = std::filesystem::path(INTEGRATION_TESTING_DIR);
    for (int i = 0; i < 3; i++) {
        testDirectory = testDirectory.parent_path();
    }
    testDirectory /= "Tests06/sp/sp-pkb/";
    auto filePath = testDirectory.string() + "valid2.txt";
    testSP.processFile(filePath, &writePKB, &readPKB);

    SECTION("SP-PKB Integration: ModifiesP") {
        auto procedureNames = readPKB.getAllProcedureNames();
        auto expectedProcedureNames = std::unordered_set<Ent>({"A", "B"});
        REQUIRE(procedureNames == expectedProcedureNames);

        auto variablesAModifies = readPKB.getModifiesP("A");
        auto expectedAModifies = std::unordered_set<Ent>({"x", "y", "z"});
        REQUIRE(variablesAModifies == expectedAModifies);

        auto variablesBModifies = readPKB.getModifiesP("B");
        auto expectedBModifies = std::unordered_set<Ent>({"z"});
        REQUIRE(variablesBModifies == expectedBModifies);
    }

    SECTION("SP-PKB Integration: ModifiesS") {
        auto variables1Modifies = readPKB.getModifiesS(1);
        auto expected1Modifies = std::unordered_set<Ent>({"x"});
        REQUIRE(variables1Modifies == expected1Modifies);

        auto variables2Modifies = readPKB.getModifiesS(2);
        auto expected2Modifies = std::unordered_set<Ent>({"y"});
        REQUIRE(variables2Modifies == expected2Modifies);

        auto variables3Modifies = readPKB.getModifiesS(3);
        auto expected3Modifies = std::unordered_set<Ent>({"x", "y", "z"});
        REQUIRE(variables3Modifies == expected3Modifies);

        auto variables4Modifies = readPKB.getModifiesS(4);
        auto expected4Modifies = std::unordered_set<Ent>({"x", "y"});
        REQUIRE(variables4Modifies == expected4Modifies);

        auto variables5Modifies = readPKB.getModifiesS(5);
        auto expected5Modifies = std::unordered_set<Ent>({"y"});
        REQUIRE(variables5Modifies == expected5Modifies);

        auto variables6Modifies = readPKB.getModifiesS(6);
        auto expected6Modifies = std::unordered_set<Ent>({"x"});
        REQUIRE(variables6Modifies == expected6Modifies);

        auto variables7Modifies = readPKB.getModifiesS(7);
        auto expected7Modifies = std::unordered_set<Ent>({"x", "y", "z"});
        REQUIRE(variables7Modifies == expected7Modifies);

        auto variables8Modifies = readPKB.getModifiesS(8);
        auto expected8Modifies = std::unordered_set<Ent>({"y"});
        REQUIRE(variables8Modifies == expected8Modifies);

        auto variables9Modifies = readPKB.getModifiesS(9);
        auto expected9Modifies = std::unordered_set<Ent>({"x"});
        REQUIRE(variables9Modifies == expected9Modifies);

        auto variables10Modifies = readPKB.getModifiesS(10);
        auto expected10Modifies = std::unordered_set<Ent>({"z"});
        REQUIRE(variables10Modifies == expected10Modifies);

        auto variables11Modifies = readPKB.getModifiesS(11);
        auto expected11Modifies = std::unordered_set<Ent>({"z"});
        REQUIRE(variables11Modifies == expected11Modifies);
    }

    SECTION("SP-PKB Integration: UsesP") {
        auto variablesAUses = readPKB.getUsesP("A");
        auto expectedAUses = std::unordered_set<Ent>({"x", "y"});
        REQUIRE(variablesAUses == expectedAUses);

        auto variablesBUses = readPKB.getUsesP("B");
        REQUIRE(variablesBUses.empty());
    }

    SECTION("SP-PKB Integration: UsesS") {
        auto variables1Uses = readPKB.getUsesS(1);
        REQUIRE(variables1Uses.empty());

        auto variables2Uses = readPKB.getUsesS(2);
        REQUIRE(variables2Uses.empty());

        auto variables3Uses = readPKB.getUsesS(3);
        auto expected3Uses = std::unordered_set<Ent>({"x", "y"});
        REQUIRE(variables3Uses == expected3Uses);

        auto variables4Uses = readPKB.getUsesS(4);
        auto expected4Uses = std::unordered_set<Ent>({"x", "y"});
        REQUIRE(variables4Uses == expected4Uses);

        auto variables5Uses = readPKB.getUsesS(5);
        auto expected5Uses = std::unordered_set<Ent>({"y"});
        REQUIRE(variables5Uses == expected5Uses);

        auto variables6Uses = readPKB.getUsesS(6);
        auto expected6Uses = std::unordered_set<Ent>({"x"});
        REQUIRE(variables6Uses == expected6Uses);

        auto variables7Uses = readPKB.getUsesS(7);
        auto expected7Uses = std::unordered_set<Ent>({"x", "y"});
        REQUIRE(variables7Uses == expected7Uses);

        auto variables8Uses = readPKB.getUsesS(8);
        auto expected8Uses = std::unordered_set<Ent>({"y"});
        REQUIRE(variables8Uses == expected8Uses);

        auto variables9Uses = readPKB.getUsesS(9);
        auto expected9Uses = std::unordered_set<Ent>({"x", "y"});
        REQUIRE(variables9Uses == expected9Uses);

        auto variables10Uses = readPKB.getUsesS(10);
        REQUIRE(variables10Uses.empty());

        auto variables11Uses = readPKB.getUsesS(11);
        REQUIRE(variables11Uses.empty());
    }

    SECTION("SP-PKB Integration: Follows") {
        shared_ptr<Relationship> followsTest1 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("1", AppConstants::FIXED_INT), Parameter("2", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows1actual = readPKB.findRelationship(followsTest1);
        std::vector<std::vector<std::string>> follows1expected = {{"1", "2"}};
        REQUIRE(follows1actual == follows1expected);

        shared_ptr<Relationship> followsTest2 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("2", AppConstants::FIXED_INT), Parameter("3", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows2actual = readPKB.findRelationship(followsTest2);
        std::vector<std::vector<std::string>> follows2expected = {{"2", "3"}};
        REQUIRE(follows2expected == follows2actual);

        shared_ptr<Relationship> followsTest3 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("3", AppConstants::FIXED_INT), Parameter("4", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows3actual = readPKB.findRelationship(followsTest3);
        REQUIRE(follows3actual.empty());

        shared_ptr<Relationship> followsTest4 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("4", AppConstants::FIXED_INT), Parameter("7", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows4actual = readPKB.findRelationship(followsTest4);
        std::vector<std::vector<std::string>> follows4expected = {{"4", "7"}};
        REQUIRE(follows4expected == follows4actual);

        shared_ptr<Relationship> followsTest5 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("4", AppConstants::FIXED_INT), Parameter("5", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows5actual = readPKB.findRelationship(followsTest5);
        REQUIRE(follows5actual.empty());

        shared_ptr<Relationship> followsTest6 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("5", AppConstants::FIXED_INT), Parameter("6", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows6actual = readPKB.findRelationship(followsTest5);
        REQUIRE(follows6actual.empty());

        shared_ptr<Relationship> followsTest7 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("4", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> follows7actual = readPKB.findRelationship(followsTest7);
        std::vector<std::vector<std::string>> follows7expected = {{"4", "7"}};
        REQUIRE(follows7expected == follows7actual);

        shared_ptr<Relationship> followsTest8 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("_", AppConstants::WILDCARD), Parameter("9", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows8actual = readPKB.findRelationship(followsTest8);
        std::vector<std::vector<std::string>> follows8expected = {{"8", "9"}};
        REQUIRE(follows8expected == follows8actual);

        shared_ptr<Relationship> followsTest9 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("_", AppConstants::WILDCARD), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> follows9actual = readPKB.findRelationship(followsTest9);
        std::vector<std::vector<std::string>> follows9expected = {
            {"1", "2"}, {"2", "3"}, {"4", "7"}, {"8", "9"}, {"9", "10"}};
        std::sort(follows9actual.begin(), follows9actual.end());
        std::sort(follows9expected.begin(), follows9expected.end());
        REQUIRE(follows9expected == follows9actual);

        shared_ptr<Relationship> followsTest10 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("re", AppConstants::READ), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> follows10actual = readPKB.findRelationship(followsTest10);
        std::vector<std::vector<std::string>> follows10expected = {{"1", "2"}, {"2", "3"}};
        std::sort(follows10actual.begin(), follows10actual.end());
        std::sort(follows10expected.begin(), follows10expected.end());
        REQUIRE(follows10actual == follows10expected);

        shared_ptr<Relationship> followsTest11 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("_", AppConstants::WILDCARD), Parameter("re", AppConstants::READ)});
        std::vector<std::vector<std::string>> follows11actual = readPKB.findRelationship(followsTest11);
        std::vector<std::vector<std::string>> follows11expected = {{"1", "2"}};
        REQUIRE(follows11expected == follows11actual);

        shared_ptr<Relationship> followsTest12 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("_", AppConstants::WILDCARD), Parameter("ca", AppConstants::CALL)});
        std::vector<std::vector<std::string>> follows12actual = readPKB.findRelationship(followsTest12);
        std::vector<std::vector<std::string>> follows12expected = {{"9", "10"}};
        REQUIRE(follows12expected == follows12actual);

        shared_ptr<Relationship> followsTest13 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("as", AppConstants::ASSIGN), Parameter("s", AppConstants::STMT)});
        std::vector<std::vector<std::string>> follows13actual = readPKB.findRelationship(followsTest13);
        std::vector<std::vector<std::string>> follows13expected = {{"8", "9"}, {"9", "10"}};
        REQUIRE(follows13expected == follows13actual);
    }

    SECTION("SP-PKB Integration: FollowsT") {
        shared_ptr<Relationship> followsTTest1 = Relationship::makeRelationship(
            AppConstants::FOLLOWST, {Parameter("4", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> followsT1actual = readPKB.findRelationship(followsTTest1);
        std::vector<std::vector<std::string>> followsT1expected = {{"4", "7"}};
        std::sort(followsT1actual.begin(), followsT1actual.end());
        std::sort(followsT1expected.begin(), followsT1expected.end());
        REQUIRE(followsT1expected == followsT1actual);

        shared_ptr<Relationship> followsTTest2 = Relationship::makeRelationship(
            AppConstants::FOLLOWST, {Parameter("1", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> followsT2actual = readPKB.findRelationship(followsTTest2);
        std::vector<std::vector<std::string>> followsT2expected = {{"1", "2"}, {"1", "3"}};
        std::sort(followsT2actual.begin(), followsT2actual.end());
        std::sort(followsT2expected.begin(), followsT2expected.end());
        REQUIRE(followsT2expected == followsT2actual);

        shared_ptr<Relationship> followsTTest3 = Relationship::makeRelationship(
            AppConstants::FOLLOWST, {Parameter("s", AppConstants::STMT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> followsT3actual = readPKB.findRelationship(followsTTest3);
        std::vector<std::vector<std::string>> followsT3expected = {{"1", "2"},  {"1", "3"}, {"2", "3"}, {"4", "7"},
                                                                   {"8", "10"}, {"8", "9"}, {"9", "10"}};
        std::sort(followsT3actual.begin(), followsT3actual.end());
        std::sort(followsT3expected.begin(), followsT3expected.end());
        REQUIRE(followsT3expected == followsT3actual);
    }

    SECTION("SP-PKB Integration: Parent") {
        shared_ptr<Relationship> parentTest1 = Relationship::makeRelationship(
            AppConstants::PARENT, {Parameter("_", AppConstants::WILDCARD), Parameter("ass", AppConstants::ASSIGN)});
        std::vector<std::vector<std::string>> parent1actual = readPKB.findRelationship(parentTest1);
        std::vector<std::vector<std::string>> parent1expected = {{"4", "5"}, {"4", "6"}, {"7", "8"}, {"7", "9"}};
        std::sort(parent1actual.begin(), parent1actual.end());
        std::sort(parent1expected.begin(), parent1expected.end());
        REQUIRE(parent1expected == parent1actual);

        shared_ptr<Relationship> parentTest2 = Relationship::makeRelationship(
            AppConstants::PARENT, {Parameter("_", AppConstants::WILDCARD), Parameter("if", AppConstants::IF)});
        std::vector<std::vector<std::string>> parent2actual = readPKB.findRelationship(parentTest2);
        std::vector<std::vector<std::string>> parent2expected = {{"3", "4"}};
        REQUIRE(parent2expected == parent2actual);

        shared_ptr<Relationship> parentTest3 = Relationship::makeRelationship(
            AppConstants::PARENT, {Parameter("_", AppConstants::WILDCARD), Parameter("w", AppConstants::WHILE)});
        std::vector<std::vector<std::string>> parent3actual = readPKB.findRelationship(parentTest3);
        std::vector<std::vector<std::string>> parent3expected = {{"3", "7"}};
        REQUIRE(parent3expected == parent3actual);

        shared_ptr<Relationship> parentTest4 = Relationship::makeRelationship(
            AppConstants::PARENT, {Parameter("ass", AppConstants::ASSIGN), Parameter("w", AppConstants::WHILE)});
        std::vector<std::vector<std::string>> parent4actual = readPKB.findRelationship(parentTest4);
        REQUIRE(parent4actual.empty());

        shared_ptr<Relationship> parentTest5 = Relationship::makeRelationship(
            AppConstants::PARENT, {Parameter("w", AppConstants::WHILE), Parameter("ca", AppConstants::CALL)});
        std::vector<std::vector<std::string>> parent5actual = readPKB.findRelationship(parentTest5);
        std::vector<std::vector<std::string>> parent5expected = {{"7", "10"}};
        REQUIRE(parent5expected == parent5actual);
    }

    SECTION("SP-PKB Integration: ParentT") {
        shared_ptr<Relationship> parentTTest1 = Relationship::makeRelationship(
            AppConstants::PARENTT, {Parameter("_", AppConstants::WILDCARD), Parameter("ass", AppConstants::ASSIGN)});
        std::vector<std::vector<std::string>> parentT1actual = readPKB.findRelationship(parentTTest1);
        std::vector<std::vector<std::string>> parentT1expected = {{"3", "5"}, {"3", "6"}, {"3", "8"}, {"3", "9"},
                                                                  {"4", "5"}, {"4", "6"}, {"7", "8"}, {"7", "9"}};
        std::sort(parentT1actual.begin(), parentT1actual.end());
        std::sort(parentT1expected.begin(), parentT1expected.end());
        REQUIRE(parentT1expected == parentT1actual);

        shared_ptr<Relationship> parentTTest2 = Relationship::makeRelationship(
            AppConstants::PARENTT, {Parameter("_", AppConstants::WILDCARD), Parameter("if", AppConstants::IF)});
        std::vector<std::vector<std::string>> parentT2actual = readPKB.findRelationship(parentTTest2);
        std::vector<std::vector<std::string>> parentT2expected = {{"3", "4"}};
        REQUIRE(parentT2expected == parentT2actual);

        shared_ptr<Relationship> parentTTest3 = Relationship::makeRelationship(
            AppConstants::PARENTT, {Parameter("_", AppConstants::WILDCARD), Parameter("w", AppConstants::WHILE)});
        std::vector<std::vector<std::string>> parentT3actual = readPKB.findRelationship(parentTTest3);
        std::vector<std::vector<std::string>> parentT3expected = {{"3", "7"}};
        REQUIRE(parentT3expected == parentT3actual);

        shared_ptr<Relationship> parentTTest4 = Relationship::makeRelationship(
            AppConstants::PARENTT, {Parameter("ass", AppConstants::ASSIGN), Parameter("w", AppConstants::WHILE)});
        std::vector<std::vector<std::string>> parent4actual = readPKB.findRelationship(parentTTest4);
        REQUIRE(parent4actual.empty());

        shared_ptr<Relationship> parentTTest5 = Relationship::makeRelationship(
            AppConstants::PARENTT, {Parameter("w", AppConstants::WHILE), Parameter("ca", AppConstants::CALL)});
        std::vector<std::vector<std::string>> parentT5actual = readPKB.findRelationship(parentTTest5);
        std::vector<std::vector<std::string>> parentT5expected = {{"3", "10"}, {"7", "10"}};
        std::sort(parentT5actual.begin(), parentT5actual.end());
        std::sort(parentT5expected.begin(), parentT5expected.end());
        REQUIRE(parentT5expected == parentT5actual);
    }

    SECTION("SP-PKB Integration: Calls") {
        shared_ptr<Relationship> callsTest1 = Relationship::makeRelationship(
            AppConstants::CALLS, {Parameter("p", AppConstants::PROCEDURE), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> callsT1actual = readPKB.findRelationship(callsTest1);
        std::vector<std::vector<std::string>> callsT1expected = {{"A", "B"}};
        REQUIRE(callsT1actual == callsT1expected);
    }

    SECTION("SP-PKB Integration: CallsT") {
        shared_ptr<Relationship> callsTTest1 = Relationship::makeRelationship(
            AppConstants::CALLST, {Parameter("p", AppConstants::PROCEDURE), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> callsTT1actual = readPKB.findRelationship(callsTTest1);
        std::vector<std::vector<std::string>> callsTT1expected = {{"A", "B"}};
        REQUIRE(callsTT1actual == callsTT1expected);
    }

    SECTION("SP-PKB Integration: CFG") {
        auto cfgA = readPKB.getCFG("A");
        REQUIRE(cfgA[1]["parents"].empty());
        REQUIRE(cfgA[1]["children"] == std::unordered_set({2}));
        REQUIRE(cfgA[2]["parents"] == std::unordered_set({1}));
        REQUIRE(cfgA[2]["children"] == std::unordered_set({3}));
        REQUIRE(cfgA[3]["parents"] == std::unordered_set({2, 7}));
        REQUIRE(cfgA[3]["children"] == std::unordered_set({4}));
        REQUIRE(cfgA[4]["parents"] == std::unordered_set({3}));
        REQUIRE(cfgA[4]["children"] == std::unordered_set({5, 6}));
        REQUIRE(cfgA[5]["parents"] == std::unordered_set({4}));
        REQUIRE(cfgA[5]["children"] == std::unordered_set({7}));
        REQUIRE(cfgA[6]["parents"] == std::unordered_set({4}));
        REQUIRE(cfgA[6]["children"] == std::unordered_set({7}));
        REQUIRE(cfgA[7]["parents"] == std::unordered_set({5, 6, 10}));
        REQUIRE(cfgA[7]["children"] == std::unordered_set({3, 8}));
        REQUIRE(cfgA[8]["parents"] == std::unordered_set({7}));
        REQUIRE(cfgA[8]["children"] == std::unordered_set({9}));
        REQUIRE(cfgA[9]["parents"] == std::unordered_set({8}));
        REQUIRE(cfgA[9]["children"] == std::unordered_set({10}));
        REQUIRE(cfgA[10]["parents"] == std::unordered_set({9}));
        REQUIRE(cfgA[10]["children"] == std::unordered_set({7}));

        auto cfgB = readPKB.getCFG("B");
        REQUIRE(cfgA[11]["parents"].empty());
        REQUIRE(cfgA[11]["children"].empty());
    }

    SECTION("SP-PKB Integration: Next") {
        shared_ptr<Relationship> nextTest1 = Relationship::makeRelationship(
            AppConstants::NEXT, {Parameter("7", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> nextT1actual = readPKB.findRelationship(nextTest1);
        std::vector<std::vector<std::string>> nextT1expected = {{"7", "3"}, {"7", "8"}};
        std::sort(nextT1actual.begin(), nextT1actual.end());
        std::sort(nextT1expected.begin(), nextT1expected.end());
        REQUIRE(nextT1actual == nextT1expected);

        shared_ptr<Relationship> nextTest2 = Relationship::makeRelationship(
            AppConstants::NEXT, {Parameter("3", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> nextT2actual = readPKB.findRelationship(nextTest2);
        std::vector<std::vector<std::string>> nextT2expected = {{"3", "4"}};
        REQUIRE(nextT2actual == nextT2expected);

        shared_ptr<Relationship> nextTest3 = Relationship::makeRelationship(
            AppConstants::NEXT, {Parameter("_", AppConstants::WILDCARD), Parameter("7", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> nextT3actual = readPKB.findRelationship(nextTest3);
        std::vector<std::vector<std::string>> nextT3expected = {{"10", "7"}, {"6", "7"}, {"5", "7"}};
        std::sort(nextT3actual.begin(), nextT3actual.end());
        std::sort(nextT3expected.begin(), nextT3expected.end());
        REQUIRE(nextT3actual == nextT3expected);
    }
}

TEST_CASE("SP-PKB Integration: Valid Source Program 3") {
    SP testSP;
    PKB testPKB;
    WritePKB writePKB;
    ReadPKB readPKB;
    testPKB.initializePkb();
    writePKB.setInstancePKB(testPKB);
    readPKB.setInstancePKB(testPKB);

    auto testDirectory = std::filesystem::path(INTEGRATION_TESTING_DIR);
    for (int i = 0; i < 3; i++) {
        testDirectory = testDirectory.parent_path();
    }
    testDirectory /= "Tests06/sp/sp-pkb/";
    auto filePath = testDirectory.string() + "valid3.txt";
    testSP.processFile(filePath, &writePKB, &readPKB);

    SECTION("SP-PKB Integration: ModifiesP") {
        auto procedureNames = readPKB.getAllProcedureNames();
        auto expectedProcedureNames = std::unordered_set<Ent>({"A", "B", "C", "D", "E"});
        REQUIRE(procedureNames == expectedProcedureNames);

        auto variablesAModifies = readPKB.getModifiesP("A");
        auto expectedAModifies = std::unordered_set<Ent>({"x", "a", "r", "o", "e"});
        REQUIRE(variablesAModifies == expectedAModifies);

        auto variablesBModifies = readPKB.getModifiesP("B");
        auto expectedBModifies = std::unordered_set<Ent>({"a", "r", "o", "e"});
        REQUIRE(variablesBModifies == expectedBModifies);

        auto variablesCModifies = readPKB.getModifiesP("C");
        auto expectedCModifies = std::unordered_set<Ent>({"a", "r", "o", "e"});
        REQUIRE(variablesCModifies == expectedCModifies);

        auto variablesDModifies = readPKB.getModifiesP("D");
        auto expectedDModifies = std::unordered_set<Ent>({"a", "r", "o"});
        REQUIRE(variablesDModifies == expectedDModifies);

        auto variablesEModifies = readPKB.getModifiesP("E");
        auto expectedEModifies = std::unordered_set<Ent>({"y", "h", "x", "a", "r", "o", "e"});
        REQUIRE(variablesEModifies == expectedEModifies);
    }

    SECTION("SP-PKB Integration: ModifiesS") {
        auto variables1Modifies = readPKB.getModifiesS(1);
        auto expected1Modifies = std::unordered_set<Ent>({"a", "r", "o", "e"});
        REQUIRE(variables1Modifies == expected1Modifies);

        auto variables2Modifies = readPKB.getModifiesS(2);
        auto expected2Modifies = std::unordered_set<Ent>({"x"});
        REQUIRE(variables2Modifies == expected2Modifies);

        auto variables3Modifies = readPKB.getModifiesS(3);
        auto expected3Modifies = std::unordered_set<Ent>({"a", "r", "o", "e"});
        REQUIRE(variables3Modifies == expected3Modifies);

        auto variables4Modifies = readPKB.getModifiesS(4);
        auto expected4Modifies = std::unordered_set<Ent>({"a", "r", "o", "e"});
        REQUIRE(variables4Modifies == expected4Modifies);

        auto variables5Modifies = readPKB.getModifiesS(5);
        REQUIRE(variables5Modifies.empty());

        auto variables6Modifies = readPKB.getModifiesS(6);
        auto expected6Modifies = std::unordered_set<Ent>({"e"});
        REQUIRE(variables6Modifies == expected6Modifies);

        auto variables7Modifies = readPKB.getModifiesS(7);
        auto expected7Modifies = std::unordered_set<Ent>({"a", "r", "o"});
        REQUIRE(variables7Modifies == expected7Modifies);

        auto variables8Modifies = readPKB.getModifiesS(8);
        auto expected8Modifies = std::unordered_set<Ent>({"a", "r"});
        REQUIRE(variables8Modifies == expected8Modifies);

        auto variables9Modifies = readPKB.getModifiesS(9);
        auto expected9Modifies = std::unordered_set<Ent>({"a"});
        REQUIRE(variables9Modifies == expected9Modifies);

        auto variables10Modifies = readPKB.getModifiesS(10);
        auto expected10Modifies = std::unordered_set<Ent>({"r"});
        REQUIRE(variables10Modifies == expected10Modifies);

        auto variables11Modifies = readPKB.getModifiesS(11);
        auto expected11Modifies = std::unordered_set<Ent>({"o"});
        REQUIRE(variables11Modifies == expected11Modifies);

        auto variables12Modifies = readPKB.getModifiesS(12);
        auto expected12Modifies = std::unordered_set<Ent>({"y", "h"});
        REQUIRE(variables12Modifies == expected12Modifies);

        auto variables13Modifies = readPKB.getModifiesS(13);
        auto expected13Modifies = std::unordered_set<Ent>({"y"});
        REQUIRE(variables13Modifies == expected13Modifies);

        auto variables14Modifies = readPKB.getModifiesS(14);
        auto expected14Modifies = std::unordered_set<Ent>({"h"});
        REQUIRE(variables14Modifies == expected14Modifies);

        auto variables15Modifies = readPKB.getModifiesS(15);
        auto expected15Modifies = std::unordered_set<Ent>({"x", "a", "r", "o", "e"});
        REQUIRE(variables15Modifies == expected15Modifies);
    }

    SECTION("SP-PKB Integration: UsesP") {
        auto variablesAUses = readPKB.getUsesP("A");
        auto expectedAUses = std::unordered_set<Ent>({"k", "c", "t", "d", "b", "p"});
        REQUIRE(variablesAUses == expectedAUses);

        auto variablesBUses = readPKB.getUsesP("B");
        auto expectedBUses = std::unordered_set<Ent>({"k", "c", "t", "d", "b", "p"});
        REQUIRE(variablesBUses == expectedBUses);

        auto variablesCUses = readPKB.getUsesP("C");
        auto expectedCUses = std::unordered_set<Ent>({"k", "c", "t", "d", "b"});
        REQUIRE(variablesCUses == expectedCUses);

        auto variablesDUses = readPKB.getUsesP("D");
        auto expectedDUses = std::unordered_set<Ent>({"c", "t", "d", "b"});
        REQUIRE(variablesDUses == expectedDUses);

        auto variablesEUses = readPKB.getUsesP("E");
        auto expectedEUses = std::unordered_set<Ent>({"k", "c", "t", "d", "b", "a", "n", "v", "p"});
        REQUIRE(variablesEUses == expectedEUses);
    }

    SECTION("SP-PKB Integration: UsesS") {
        auto variables1Uses = readPKB.getUsesS(1);
        auto expected1Uses = std::unordered_set<Ent>({"k", "c", "t", "d", "b", "p"});
        REQUIRE(variables1Uses == expected1Uses);

        auto variables2Uses = readPKB.getUsesS(2);
        REQUIRE(variables2Uses.empty());

        auto variables3Uses = readPKB.getUsesS(3);
        auto expected3Uses = std::unordered_set<Ent>({"k", "c", "t", "d", "b"});
        REQUIRE(variables3Uses == expected3Uses);

        auto variables4Uses = readPKB.getUsesS(4);
        auto expected4Uses = std::unordered_set<Ent>({"k", "c", "t", "d", "b"});
        REQUIRE(variables4Uses == expected4Uses);

        auto variables5Uses = readPKB.getUsesS(5);
        auto expected5Uses = std::unordered_set<Ent>({"p"});
        REQUIRE(variables5Uses == expected5Uses);

        auto variables6Uses = readPKB.getUsesS(6);
        auto expected6Uses = std::unordered_set<Ent>({"k"});
        REQUIRE(variables6Uses == expected6Uses);

        auto variables7Uses = readPKB.getUsesS(7);
        auto expected7Uses = std::unordered_set<Ent>({"c", "t", "d", "b"});
        REQUIRE(variables7Uses == expected7Uses);

        auto variables8Uses = readPKB.getUsesS(8);
        auto expected8Uses = std::unordered_set<Ent>({"c", "t", "d", "b"});
        REQUIRE(variables8Uses == expected8Uses);

        auto variables9Uses = readPKB.getUsesS(9);
        auto expected9Uses = std::unordered_set<Ent>({"c", "d", "b"});
        REQUIRE(variables9Uses == expected9Uses);

        auto variables10Uses = readPKB.getUsesS(10);
        REQUIRE(variables10Uses.empty());

        auto variables11Uses = readPKB.getUsesS(11);
        REQUIRE(variables11Uses.empty());

        auto variables12Uses = readPKB.getUsesS(12);
        auto expected12Uses = std::unordered_set<Ent>({"a", "n", "v"});
        REQUIRE(variables12Uses == expected12Uses);

        auto variables13Uses = readPKB.getUsesS(13);
        REQUIRE(variables13Uses.empty());

        auto variables14Uses = readPKB.getUsesS(14);
        auto expected14Uses = std::unordered_set<Ent>({"n", "v"});
        REQUIRE(variables14Uses == expected14Uses);

        auto variables15Uses = readPKB.getUsesS(15);
        auto expected15Uses = std::unordered_set<Ent>({"k", "c", "t", "d", "b", "p"});
        REQUIRE(variables15Uses == expected15Uses);
    }

    SECTION("SP-PKB Integration: Follows") {
        std::vector<Parameter> params1 = {Parameter("1", AppConstants::FIXED_INT),
                                          Parameter("2", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> followsTest1 = Relationship::makeRelationship(AppConstants::FOLLOWS, params1);
        std::vector<std::vector<std::string>> follows1actual = readPKB.findRelationship(followsTest1);
        std::vector<std::vector<std::string>> follows1expected = {{"1", "2"}};
        REQUIRE(follows1actual == follows1expected);

        shared_ptr<Relationship> followsTest2 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("3", AppConstants::FIXED_INT), Parameter("4", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows2actual = readPKB.findRelationship(followsTest2);
        REQUIRE(follows2actual.empty());

        shared_ptr<Relationship> followsTest3 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("8", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> follows3actual = readPKB.findRelationship(followsTest3);
        std::vector<std::vector<std::string>> follows3expected = {{"8", "11"}};
        REQUIRE(follows3expected == follows3actual);

        shared_ptr<Relationship> followsTest4 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("_", AppConstants::WILDCARD), Parameter("15", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> follows4actual = readPKB.findRelationship(followsTest4);
        std::vector<std::vector<std::string>> follows4expected = {{"12", "15"}};
        REQUIRE(follows4expected == follows4actual);

        shared_ptr<Relationship> followsTest5 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("_", AppConstants::WILDCARD), Parameter("re", AppConstants::READ)});
        std::vector<std::vector<std::string>> follows5actual = readPKB.findRelationship(followsTest5);
        std::vector<std::vector<std::string>> follows5expected = {{"1", "2"}};
        REQUIRE(follows5expected == follows5actual);

        shared_ptr<Relationship> followsTest6 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("_", AppConstants::WILDCARD), Parameter("ca", AppConstants::CALL)});
        std::vector<std::vector<std::string>> follows6actual = readPKB.findRelationship(followsTest6);
        std::vector<std::vector<std::string>> follows6expected = {{"2", "3"}, {"6", "7"}, {"12", "15"}};
        std::sort(follows6actual.begin(), follows6actual.end());
        std::sort(follows6expected.begin(), follows6expected.end());
        REQUIRE(follows6expected == follows6actual);

        shared_ptr<Relationship> followsTest7 = Relationship::makeRelationship(
            AppConstants::FOLLOWS, {Parameter("_", AppConstants::WILDCARD), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> follows7actual = readPKB.findRelationship(followsTest7);
        std::vector<std::vector<std::string>> follows7expected = {{"1", "2"},  {"2", "3"},  {"4", "5"},  {"6", "7"},
                                                                  {"8", "11"}, {"9", "10"}, {"12", "15"}};
        std::sort(follows7actual.begin(), follows7actual.end());
        std::sort(follows7expected.begin(), follows7expected.end());
        REQUIRE(follows7expected == follows7actual);
    }

    SECTION("SP-PKB Integration: FollowsT") {
        shared_ptr<Relationship> followsTTest1 = Relationship::makeRelationship(
            AppConstants::FOLLOWST, {Parameter("1", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> followsT1actual = readPKB.findRelationship(followsTTest1);
        std::vector<std::vector<std::string>> followsT1expected = {{"1", "2"}, {"1", "3"}};
        std::sort(followsT1actual.begin(), followsT1actual.end());
        std::sort(followsT1expected.begin(), followsT1expected.end());
        REQUIRE(followsT1expected == followsT1actual);

        shared_ptr<Relationship> followsTTest2 = Relationship::makeRelationship(
            AppConstants::FOLLOWST, {Parameter("as", AppConstants::ASSIGN), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> followsT2actual = readPKB.findRelationship(followsTTest2);
        std::vector<std::vector<std::string>> followsT2expected = {{"6", "7"}, {"9", "10"}};
        std::sort(followsT2actual.begin(), followsT2actual.end());
        std::sort(followsT2expected.begin(), followsT2expected.end());
        REQUIRE(followsT2expected == followsT2actual);

        shared_ptr<Relationship> followsTTest3 = Relationship::makeRelationship(
            AppConstants::FOLLOWST, {Parameter("s", AppConstants::STMT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> followsT3actual = readPKB.findRelationship(followsTTest3);
        std::vector<std::vector<std::string>> followsT3expected = {{"1", "2"}, {"1", "3"},  {"2", "3"},  {"4", "5"},
                                                                   {"6", "7"}, {"8", "11"}, {"9", "10"}, {"12", "15"}};
        std::sort(followsT3actual.begin(), followsT3actual.end());
        std::sort(followsT3expected.begin(), followsT3expected.end());
        REQUIRE(followsT3expected == followsT3actual);
    }

    SECTION("SP-PKB Integration: Parent") {
        shared_ptr<Relationship> parentTest1 = Relationship::makeRelationship(
            AppConstants::PARENT, {Parameter("_", AppConstants::WILDCARD), Parameter("ass", AppConstants::ASSIGN)});
        std::vector<std::vector<std::string>> parent1actual = readPKB.findRelationship(parentTest1);
        std::vector<std::vector<std::string>> parent1expected = {{"12", "13"}, {"12", "14"}, {"8", "10"}, {"8", "9"}};
        std::sort(parent1actual.begin(), parent1actual.end());
        std::sort(parent1expected.begin(), parent1expected.end());
        REQUIRE(parent1expected == parent1actual);

        shared_ptr<Relationship> parentTest2 = Relationship::makeRelationship(
            AppConstants::PARENT, {Parameter("_", AppConstants::WILDCARD), Parameter("if", AppConstants::IF)});
        std::vector<std::vector<std::string>> parent2actual = readPKB.findRelationship(parentTest2);
        REQUIRE(parent2actual.empty());

        shared_ptr<Relationship> parentTest3 = Relationship::makeRelationship(
            AppConstants::PARENT, {Parameter("w", AppConstants::WHILE), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> parent3actual = readPKB.findRelationship(parentTest3);
        std::vector<std::vector<std::string>> parent3expected = {{"8", "10"}, {"8", "9"}};
        std::sort(parent3actual.begin(), parent3actual.end());
        std::sort(parent3expected.begin(), parent3expected.end());
        REQUIRE(parent3expected == parent3actual);
    }

    SECTION("SP-PKB Integration: ParentT") {
        shared_ptr<Relationship> parentTTest1 = Relationship::makeRelationship(
            AppConstants::PARENTT, {Parameter("_", AppConstants::WILDCARD), Parameter("ass", AppConstants::ASSIGN)});
        std::vector<std::vector<std::string>> parentT1actual = readPKB.findRelationship(parentTTest1);
        std::vector<std::vector<std::string>> parentT1expected = {{"12", "13"}, {"12", "14"}, {"8", "10"}, {"8", "9"}};
        std::sort(parentT1actual.begin(), parentT1actual.end());
        std::sort(parentT1expected.begin(), parentT1expected.end());
        REQUIRE(parentT1expected == parentT1actual);
    }

    SECTION("SP-PKB Integration: Calls") {
        shared_ptr<Relationship> callsTest1 = Relationship::makeRelationship(
            AppConstants::CALLS, {Parameter("_", AppConstants::WILDCARD), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> callsT1actual = readPKB.findRelationship(callsTest1);
        std::vector<std::vector<std::string>> callsT1expected = {
            {"A", "B"}, {"A", "C"}, {"B", "C"}, {"C", "D"}, {"E", "A"}};
        std::sort(callsT1actual.begin(), callsT1actual.end());
        std::sort(callsT1expected.begin(), callsT1expected.end());
        REQUIRE(callsT1actual == callsT1expected);

        shared_ptr<Relationship> callsTest2 = Relationship::makeRelationship(
            AppConstants::CALLS, {Parameter("A", AppConstants::FIXED_STRING), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> callsT2actual = readPKB.findRelationship(callsTest2);
        std::vector<std::vector<std::string>> callsT2expected = {{"A", "B"}, {"A", "C"}};
        std::sort(callsT2actual.begin(), callsT2actual.end());
        std::sort(callsT2expected.begin(), callsT2expected.end());
        REQUIRE(callsT2actual == callsT2expected);

        shared_ptr<Relationship> callsTest3 = Relationship::makeRelationship(
            AppConstants::CALLS, {Parameter("E", AppConstants::FIXED_STRING), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> callsT3actual = readPKB.findRelationship(callsTest3);
        std::vector<std::vector<std::string>> callsT3expected = {{"E", "A"}};
        REQUIRE(callsT3actual == callsT3expected);

        shared_ptr<Relationship> callsTest4 = Relationship::makeRelationship(
            AppConstants::CALLS, {Parameter("_", AppConstants::WILDCARD), Parameter("C", AppConstants::FIXED_STRING)});
        std::vector<std::vector<std::string>> callsT4actual = readPKB.findRelationship(callsTest4);
        std::vector<std::vector<std::string>> callsT4expected = {{"A", "C"}, {"B", "C"}};
        std::sort(callsT4actual.begin(), callsT4actual.end());
        std::sort(callsT4expected.begin(), callsT4expected.end());
        REQUIRE(callsT4actual == callsT4expected);
    }

    SECTION("SP-PKB Integration: CallsT") {
        shared_ptr<Relationship> callsTTest1 = Relationship::makeRelationship(
            AppConstants::CALLST, {Parameter("p", AppConstants::PROCEDURE), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> callsTT1actual = readPKB.findRelationship(callsTTest1);
        std::vector<std::vector<std::string>> callsTT1expected = {{"A", "B"}, {"A", "C"}, {"A", "D"}, {"E", "A"},
                                                                  {"E", "B"}, {"E", "C"}, {"E", "D"}, {"B", "C"},
                                                                  {"B", "D"}, {"C", "D"}};
        std::sort(callsTT1actual.begin(), callsTT1actual.end());
        std::sort(callsTT1expected.begin(), callsTT1expected.end());
        REQUIRE(callsTT1actual == callsTT1expected);

        shared_ptr<Relationship> callsTTest2 = Relationship::makeRelationship(
            AppConstants::CALLST, {Parameter("E", AppConstants::FIXED_STRING), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> callsTT2actual = readPKB.findRelationship(callsTTest2);
        std::vector<std::vector<std::string>> callsTT2expected = {{"E", "A"}, {"E", "B"}, {"E", "C"}, {"E", "D"}};
        std::sort(callsTT2actual.begin(), callsTT2actual.end());
        std::sort(callsTT2expected.begin(), callsTT2expected.end());
        REQUIRE(callsTT2actual == callsTT2expected);

        shared_ptr<Relationship> callsTTest3 = Relationship::makeRelationship(
            AppConstants::CALLST, {Parameter("_", AppConstants::WILDCARD), Parameter("C", AppConstants::FIXED_STRING)});
        std::vector<std::vector<std::string>> callsTT3actual = readPKB.findRelationship(callsTTest3);
        std::vector<std::vector<std::string>> callsTT3expected = {{"A", "C"}, {"B", "C"}, {"E", "C"}};
        std::sort(callsTT3actual.begin(), callsTT3actual.end());
        std::sort(callsTT3expected.begin(), callsTT3expected.end());
        REQUIRE(callsTT3actual == callsTT3expected);
    }

    SECTION("SP-PKB Integration: CFG") {
        auto cfgA = readPKB.getCFG("A");
        REQUIRE(cfgA[1]["parents"].empty());
        REQUIRE(cfgA[1]["children"] == std::unordered_set({2}));
        REQUIRE(cfgA[2]["parents"] == std::unordered_set({1}));
        REQUIRE(cfgA[2]["children"] == std::unordered_set({3}));
        REQUIRE(cfgA[3]["parents"] == std::unordered_set({2}));
        REQUIRE(cfgA[3]["children"].empty());

        auto cfgB = readPKB.getCFG("B");
        REQUIRE(cfgB[4]["parents"].empty());
        REQUIRE(cfgB[4]["children"] == std::unordered_set({5}));
        REQUIRE(cfgB[5]["parents"] == std::unordered_set({4}));
        REQUIRE(cfgB[5]["children"].empty());

        auto cfgC = readPKB.getCFG("C");
        REQUIRE(cfgC[6]["parents"].empty());
        REQUIRE(cfgC[6]["children"] == std::unordered_set({7}));
        REQUIRE(cfgC[7]["parents"] == std::unordered_set({6}));
        REQUIRE(cfgC[7]["children"].empty());

        auto cfgD = readPKB.getCFG("D");
        REQUIRE(cfgD[8]["parents"] == std::unordered_set({10}));
        REQUIRE(cfgD[8]["children"] == std::unordered_set({9, 11}));
        REQUIRE(cfgD[9]["parents"] == std::unordered_set({8}));
        REQUIRE(cfgD[9]["children"] == std::unordered_set({10}));
        REQUIRE(cfgD[10]["parents"] == std::unordered_set({9}));
        REQUIRE(cfgD[10]["children"] == std::unordered_set({8}));
        REQUIRE(cfgD[11]["parents"] == std::unordered_set({8}));
        REQUIRE(cfgD[11]["children"].empty());

        auto cfgE = readPKB.getCFG("E");
        REQUIRE(cfgE[12]["parents"].empty());
        REQUIRE(cfgE[12]["children"] == std::unordered_set({13, 14}));
        REQUIRE(cfgE[13]["parents"] == std::unordered_set({12}));
        REQUIRE(cfgE[13]["children"] == std::unordered_set({15}));
        REQUIRE(cfgE[14]["parents"] == std::unordered_set({12}));
        REQUIRE(cfgE[14]["children"] == std::unordered_set({15}));
        REQUIRE(cfgE[15]["parents"] == std::unordered_set({13, 14}));
        REQUIRE(cfgE[15]["children"].empty());
    }

    SECTION("SP-PKB Integration: Next") {
        shared_ptr<Relationship> nextTest1 = Relationship::makeRelationship(
            AppConstants::NEXT, {Parameter("8", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> nextT1actual = readPKB.findRelationship(nextTest1);
        std::vector<std::vector<std::string>> nextT1expected = {{"8", "9"}, {"8", "11"}};
        std::sort(nextT1actual.begin(), nextT1actual.end());
        std::sort(nextT1expected.begin(), nextT1expected.end());
        REQUIRE(nextT1actual == nextT1expected);

        shared_ptr<Relationship> nextTest2 = Relationship::makeRelationship(
            AppConstants::NEXT, {Parameter("4", AppConstants::FIXED_INT), Parameter("_", AppConstants::WILDCARD)});
        std::vector<std::vector<std::string>> nextT2actual = readPKB.findRelationship(nextTest2);
        std::vector<std::vector<std::string>> nextT2expected = {{"4", "5"}};
        REQUIRE(nextT2actual == nextT2expected);

        shared_ptr<Relationship> nextTest3 = Relationship::makeRelationship(
            AppConstants::NEXT, {Parameter("_", AppConstants::WILDCARD), Parameter("15", AppConstants::FIXED_INT)});
        std::vector<std::vector<std::string>> nextT3actual = readPKB.findRelationship(nextTest3);
        std::vector<std::vector<std::string>> nextT3expected = {{"13", "15"}, {"14", "15"}};
        std::sort(nextT3actual.begin(), nextT3actual.end());
        std::sort(nextT3expected.begin(), nextT3expected.end());
        REQUIRE(nextT3actual == nextT3expected);
    }
}

TEST_CASE("Invalid Source Program") {
    SP testSP;
    PKB testPKB;
    WritePKB writePKB;
    ReadPKB readPKB;
    testPKB.initializePkb();
    writePKB.setInstancePKB(testPKB);
    readPKB.setInstancePKB(testPKB);

    auto testDirectory = std::filesystem::path(INTEGRATION_TESTING_DIR);
    for (int i = 0; i < 3; i++) {
        testDirectory = testDirectory.parent_path();
    }
    testDirectory /= "Tests06/sp/sp-pkb/";

    SECTION("additional ;") {
        std::string errorMessage = "";
        try {
            auto filePath = testDirectory.string() + "invalid1.txt";
            testSP.processFile(filePath, &writePKB, &readPKB); // execution should stop here.
            REQUIRE(false);
        } catch (std::exception e) {
            REQUIRE(true);
        }
    }
}