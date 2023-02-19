#include "catch.hpp"
#include <filesystem>
#include "../../spa/src/utils/AppConstants.h"
#include "../../spa/src/SP/SP.h"
#include "../../unit_testing/src/utils/utils.h"

using namespace unit_testing_utils;

TEST_CASE("SP-PKB Integration: Valid Source Program, Uses, Modifies") {
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

    SECTION("SP-PKB Integration: Valid Source Program, Uses, Modifies: ModifiesS + ModifiesP") {

        auto procedureNames = readPKB.getAllProcedureNames();
        auto expectedProcedureNames = std::unordered_set<Ent>({ "A", "B", "C" });
        REQUIRE(procedureNames == expectedProcedureNames);

        // Modifies
        auto variablesAModifies = readPKB.getModifiesP("A");
        auto expectedAModifies = std::unordered_set<Ent>({ "x" });
        REQUIRE(variablesAModifies == expectedAModifies);

        auto variablesBModifies = readPKB.getModifiesP("B");
        auto expectedBModifies = std::unordered_set<Ent>({ "y" });
        REQUIRE(variablesBModifies == expectedBModifies);

        auto variablesCModifies = readPKB.getModifiesP("C");
        auto expectedCModifies = std::unordered_set<Ent>({ "z", "y" });
        REQUIRE(variablesCModifies == expectedCModifies);

        auto variables1Modifies = readPKB.getModifiesS(1);
        auto expected1Modifies = std::unordered_set<Ent>({ "x" });
        REQUIRE(variables1Modifies == expected1Modifies);

        auto variables2Modifies = readPKB.getModifiesS(2);
        auto expected2Modifies = std::unordered_set<Ent>({ "y" });
        REQUIRE(variables2Modifies == expected2Modifies);

        auto variables3Modifies = readPKB.getModifiesS(3);
        auto expected3Modifies = std::unordered_set<Ent>({ "y" });
        REQUIRE(variables3Modifies == expected3Modifies);

        auto variables4Modifies = readPKB.getModifiesS(4);
        auto expected4Modifies = std::unordered_set<Ent>({});
        REQUIRE(variables4Modifies == expected4Modifies);

        auto variables5Modifies = readPKB.getModifiesS(5);
        auto expected5Modifies = std::unordered_set<Ent>({ "y" });
        REQUIRE(variables5Modifies == expected5Modifies);

        auto variables6Modifies = readPKB.getModifiesS(6);
        auto expected6Modifies = std::unordered_set<Ent>({ "z" });
        REQUIRE(variables6Modifies == expected6Modifies);
    }

    SECTION("SP-PKB Integration: Valid Source Program, Uses, Modifies: UsesS + UsesP") {

        // Uses
        auto variablesAUses = readPKB.getUsesP("A");
        auto expectedAUses = std::unordered_set<Ent>({ "x" });
        REQUIRE(variablesAUses == expectedAUses);

        auto variablesBUses = readPKB.getUsesP("B");
        auto expectedBUses = std::unordered_set<Ent>({ "y" });
        REQUIRE(variablesBUses == expectedBUses);

        auto variablesCUses = readPKB.getUsesP("C");
        auto expectedCUses = std::unordered_set<Ent>({ "z", "y" });
        REQUIRE(variablesCUses == expectedCUses);

        auto variables1Uses = readPKB.getUsesS(1);
        auto expected1Uses = std::unordered_set<Ent>({ "x" });
        REQUIRE(variables1Uses == expected1Uses);

        auto variables2Uses = readPKB.getUsesS(2);
        auto expected2Uses = std::unordered_set<Ent>({ "y" });
        REQUIRE(variables2Uses == expected2Uses);

        auto variables3Uses = readPKB.getUsesS(3);
        auto expected3Uses = std::unordered_set<Ent>({});
        REQUIRE(variables3Uses == expected3Uses);

        auto variables4Uses = readPKB.getUsesS(4);
        auto expected4Uses = std::unordered_set<Ent>({ "z" });
        REQUIRE(variables4Uses == expected4Uses);

        auto variables5Uses = readPKB.getUsesS(5);
        auto expected5Uses = std::unordered_set<Ent>({ "y" });
        REQUIRE(variables5Uses == expected5Uses);

        auto variables6Uses = readPKB.getUsesS(6);
        auto expected6Uses = std::unordered_set<Ent>({ "z" });
        REQUIRE(variables6Uses == expected6Uses);
    }
}

TEST_CASE("SP-PKB Integration: Valid Source Program, Follows, FollowsT") {
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

    SECTION("SP-PKB Integration: Valid Source Program, Follows, FollowsT: Follows") {
        // Follows
        std::vector<Parameter> params1 = {Parameter("1", AppConstants::FIXED_INT), Parameter("2", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> followsTest1 = Relationship::makeRelationship(AppConstants::FOLLOWS, params1);
        std::vector<std::vector<std::string>> follows1actual = readPKB.findRelationship(followsTest1);
        REQUIRE(follows1actual.empty());

        shared_ptr<Relationship> followsTest2 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                               {Parameter("2", AppConstants::FIXED_INT),
                                                                                Parameter("3", AppConstants::FIXED_INT)}
        );
        std::vector<std::vector<std::string>> follows2actual = readPKB.findRelationship(followsTest2);
        std::vector<std::vector<std::string>> follows2expected = {{"2", "3"}};
        REQUIRE(follows2expected == follows2actual);

        shared_ptr<Relationship> followsTest3 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                               {Parameter("4", AppConstants::FIXED_INT),
                                                                                Parameter("5", AppConstants::FIXED_INT)}
        );
        std::vector<std::vector<std::string>> follows3actual = readPKB.findRelationship(followsTest3);
        std::vector<std::vector<std::string>> follows3expected = {{"4", "5"}};
        REQUIRE(follows3expected == follows3actual);

        shared_ptr<Relationship> followsTest4 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                               {Parameter("5", AppConstants::FIXED_INT),
                                                                                Parameter("6", AppConstants::FIXED_INT)}
        );
        std::vector<std::vector<std::string>> follows4actual = readPKB.findRelationship(followsTest4);
        std::vector<std::vector<std::string>> follows4expected = {{"5", "6"}};
        REQUIRE(follows4expected == follows4actual);

        shared_ptr<Relationship> followsTest5 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                               {Parameter("4", AppConstants::FIXED_INT),
                                                                                Parameter("6", AppConstants::FIXED_INT)}
        );
        std::vector<std::vector<std::string>> follows5actual = readPKB.findRelationship(followsTest5);
        REQUIRE(follows5actual.empty());

        shared_ptr<Relationship> followsTest7 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                               {Parameter("4", AppConstants::FIXED_INT),
                                                                                Parameter("_", AppConstants::WILDCARD)}
        );
        std::vector<std::vector<std::string>> follows7actual = readPKB.findRelationship(followsTest7);
        std::vector<std::vector<std::string>> follows7expected = {{"4", "5"}};
        REQUIRE(follows7expected == follows7actual);

        shared_ptr<Relationship> followsTest8 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                               {Parameter("_", AppConstants::WILDCARD),
                                                                                Parameter("6", AppConstants::FIXED_INT)}
        );
        std::vector<std::vector<std::string>> follows8actual = readPKB.findRelationship(followsTest8);
        std::vector<std::vector<std::string>> follows8expected = {{"5", "6"}};
        REQUIRE(follows8expected == follows8actual);

        shared_ptr<Relationship> followsTest9 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                               {Parameter("_", AppConstants::WILDCARD),
                                                                                Parameter("_", AppConstants::WILDCARD)}
        );
        std::vector<std::vector<std::string>> follows9actual = readPKB.findRelationship(followsTest9);
        std::vector<std::vector<std::string>> follows9expected = {{"2", "3"}, {"4", "5"}, {"5", "6"}};
        std::sort(follows9actual.begin(), follows9actual.end());
        std::sort(follows9expected.begin(), follows9expected.end());
        REQUIRE(follows9expected == follows9actual);

        shared_ptr<Relationship> followsTest10 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                                {Parameter("re", AppConstants::READ),
                                                                                 Parameter("_", AppConstants::WILDCARD)}
        );
        std::vector<std::vector<std::string>> follows10actual = readPKB.findRelationship(followsTest10);
        REQUIRE(follows1actual.empty());

        shared_ptr<Relationship> followsTest11 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                                {Parameter("_", AppConstants::WILDCARD),
                                                                                 Parameter("re", AppConstants::READ)}
        );
        std::vector<std::vector<std::string>> follows11actual = readPKB.findRelationship(followsTest11);
        std::vector<std::vector<std::string>> follows11expected = {{"2", "3"}};
        std::sort(follows11actual.begin(), follows11actual.end());
        std::sort(follows11expected.begin(), follows11expected.end());
        REQUIRE(follows11expected == follows11actual);

        shared_ptr<Relationship> followsTest12 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                                {Parameter("pn", AppConstants::PRINT),
                                                                                 Parameter("ca", AppConstants::CALL)}
        );
        std::vector<std::vector<std::string>> follows12actual = readPKB.findRelationship(followsTest12);
        std::vector<std::vector<std::string>> follows12expected = {{"4", "5"}};
        REQUIRE(follows12expected == follows12actual);

        shared_ptr<Relationship> followsTest13 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                                {Parameter("s", AppConstants::STMT),
                                                                                 Parameter("ca", AppConstants::CALL)}
        );
        std::vector<std::vector<std::string>> follows13actual = readPKB.findRelationship(followsTest13);
        std::vector<std::vector<std::string>> follows13expected = {{"4", "5"}};
        REQUIRE(follows13expected == follows13actual);

        shared_ptr<Relationship> followsTest14 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                                {Parameter("as", AppConstants::ASSIGN),
                                                                                 Parameter("_", AppConstants::WILDCARD)}
        );
        std::vector<std::vector<std::string>> follows14actual = readPKB.findRelationship(followsTest14);
        std::vector<std::vector<std::string>> follows14expected = {{"2", "3"}};
        REQUIRE(follows14expected == follows14actual);
    }

    SECTION("SP-PKB Integration: Valid Source Program, Follows, FollowsT: FollowsT") {
        // FollowsT
        shared_ptr<Relationship> followsTTest1 = Relationship::makeRelationship(AppConstants::FOLLOWST,
                                                                                {Parameter("4", AppConstants::FIXED_INT),
                                                                                 Parameter("_", AppConstants::WILDCARD)}
        );
        std::vector<std::vector<std::string>> followsT1actual = readPKB.findRelationship(followsTTest1);
        std::vector<std::vector<std::string>> followsT1expected = {{"4", "5"}, {"4", "6"}};
        std::sort(followsT1actual.begin(), followsT1actual.end());
        std::sort(followsT1expected.begin(), followsT1expected.end());
        REQUIRE(followsT1expected == followsT1actual);

        shared_ptr<Relationship> followsTTest2 = Relationship::makeRelationship(AppConstants::FOLLOWST,
                                                                                {Parameter("as", AppConstants::ASSIGN),
                                                                                 Parameter("_", AppConstants::WILDCARD)}
        );
        std::vector<std::vector<std::string>> followsT2actual = readPKB.findRelationship(followsTTest2);
        std::vector<std::vector<std::string>> followsT2expected = {{"2", "3"}};
        REQUIRE(followsT2expected == followsT2actual);

        shared_ptr<Relationship> followsTTest3 = Relationship::makeRelationship(AppConstants::FOLLOWST,
                                                                                {Parameter("s", AppConstants::STMT),
                                                                                 Parameter("_", AppConstants::WILDCARD)}
        );
        std::vector<std::vector<std::string>> followsT3actual = readPKB.findRelationship(followsTTest3);
        std::vector<std::vector<std::string>> followsT3expected = {{"2", "3"}, {"4", "5"}, {"4", "6"}, {"5", "6"}};
        std::sort(followsT3actual.begin(), followsT3actual.end());
        std::sort(followsT3expected.begin(), followsT3expected.end());
        REQUIRE(followsT3expected == followsT3actual);

        shared_ptr<Relationship> followsTTest4 = Relationship::makeRelationship(AppConstants::FOLLOWST,
                                                                                {Parameter("pn", AppConstants::PRINT),
                                                                                 Parameter("_", AppConstants::WILDCARD)}
        );
        std::vector<std::vector<std::string>> followsT4actual = readPKB.findRelationship(followsTTest4);
        std::vector<std::vector<std::string>> followsT4expected = {{"4", "5"}, {"4", "6"}};
        std::sort(followsT4actual.begin(), followsT4actual.end());
        std::sort(followsT4expected.begin(), followsT4expected.end());
        REQUIRE(followsT4expected == followsT4actual);
    }


}

TEST_CASE("SP-PKB Integration: Valid Source Program, Parent, ParentT") {
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

    SECTION("SP-PKB Integration: Valid Source Program, Parent, ParentT: Parent") {

        shared_ptr<Relationship> parentTest1 = Relationship::makeRelationship(AppConstants::PARENT,
                                                                              {Parameter("_", AppConstants::WILDCARD),
                                                                               Parameter("ass", AppConstants::ASSIGN)}
        );
        std::vector<std::vector<std::string>> parent1actual = readPKB.findRelationship(parentTest1);
        std::vector<std::vector<std::string>> parent1expected = {{"4", "5"}, {"4", "6"}, {"7", "8"}, {"7", "9"}};
        std::sort(parent1actual.begin(), parent1actual.end());
        std::sort(parent1expected.begin(), parent1expected.end());
        REQUIRE(parent1expected == parent1actual);

        shared_ptr<Relationship> parentTest2 = Relationship::makeRelationship(AppConstants::PARENT,
                                                                              {Parameter("_", AppConstants::WILDCARD),
                                                                               Parameter("if", AppConstants::IF)}
        );
        std::vector<std::vector<std::string>> parent2actual = readPKB.findRelationship(parentTest2);
        std::vector<std::vector<std::string>> parent2expected = {{"3", "4"}};
        std::sort(parent2actual.begin(), parent2actual.end());
        std::sort(parent2expected.begin(), parent2expected.end());
        REQUIRE(parent2expected == parent2actual);

        shared_ptr<Relationship> parentTest3 = Relationship::makeRelationship(AppConstants::PARENT,
                                                                              {Parameter("_", AppConstants::WILDCARD),
                                                                               Parameter("w", AppConstants::WHILE)}
        );
        std::vector<std::vector<std::string>> parent3actual = readPKB.findRelationship(parentTest3);
        std::vector<std::vector<std::string>> parent3expected = {{"3", "7"}};
        std::sort(parent3actual.begin(), parent3actual.end());
        std::sort(parent3expected.begin(), parent3expected.end());
        REQUIRE(parent3expected == parent3actual);

        shared_ptr<Relationship> parentTest4 = Relationship::makeRelationship(AppConstants::PARENT,
                                                                              {Parameter("ass", AppConstants::ASSIGN),
                                                                               Parameter("w", AppConstants::WHILE)}
        );
        std::vector<std::vector<std::string>> parent4actual = readPKB.findRelationship(parentTest4);
        REQUIRE(parent4actual.empty());

        shared_ptr<Relationship> parentTest5 = Relationship::makeRelationship(AppConstants::PARENT,
                                                                              {Parameter("w", AppConstants::WHILE),
                                                                               Parameter("ca", AppConstants::CALL)}
        );
        std::vector<std::vector<std::string>> parent5actual = readPKB.findRelationship(parentTest5);
        std::vector<std::vector<std::string>> parent5expected = {{"7", "10"}};
        std::sort(parent5actual.begin(), parent5actual.end());
        std::sort(parent5expected.begin(), parent5expected.end());
        REQUIRE(parent5expected == parent5actual);
    }

    SECTION("SP-PKB Integration: Valid Source Program, Parent, ParentT: ParentT") {

        shared_ptr<Relationship> parentTTest1 = Relationship::makeRelationship(AppConstants::PARENTT,
                                                                              {Parameter("_", AppConstants::WILDCARD),
                                                                               Parameter("ass", AppConstants::ASSIGN)}
        );
        std::vector<std::vector<std::string>> parentT1actual = readPKB.findRelationship(parentTTest1);
        std::vector<std::vector<std::string>> parentT1expected = { { "3", "5" }, { "3", "6" }, { "3", "8" },
                                                                   { "3", "9" }, { "4", "5" }, { "4", "6" },
                                                                   { "7", "8" }, { "7", "9" } };
        std::sort(parentT1actual.begin(), parentT1actual.end());
        std::sort(parentT1expected.begin(), parentT1expected.end());
        REQUIRE(parentT1expected == parentT1actual);

        shared_ptr<Relationship> parentTTest2 = Relationship::makeRelationship(AppConstants::PARENTT,
                                                                              {Parameter("_", AppConstants::WILDCARD),
                                                                               Parameter("if", AppConstants::IF)}
        );
        std::vector<std::vector<std::string>> parentT2actual = readPKB.findRelationship(parentTTest2);
        std::vector<std::vector<std::string>> parentT2expected = {{"3", "4"}};
        std::sort(parentT2actual.begin(), parentT2actual.end());
        std::sort(parentT2expected.begin(), parentT2expected.end());
        REQUIRE(parentT2expected == parentT2actual);

        shared_ptr<Relationship> parentTTest3 = Relationship::makeRelationship(AppConstants::PARENTT,
                                                                              {Parameter("_", AppConstants::WILDCARD),
                                                                               Parameter("w", AppConstants::WHILE)}
        );
        std::vector<std::vector<std::string>> parentT3actual = readPKB.findRelationship(parentTTest3);
        std::vector<std::vector<std::string>> parentT3expected = {{"3", "7"}};
        std::sort(parentT3actual.begin(), parentT3actual.end());
        std::sort(parentT3expected.begin(), parentT3expected.end());
        REQUIRE(parentT3expected == parentT3actual);

        shared_ptr<Relationship> parentTTest4 = Relationship::makeRelationship(AppConstants::PARENTT,
                                                                              {Parameter("ass", AppConstants::ASSIGN),
                                                                               Parameter("w", AppConstants::WHILE)}
        );
        std::vector<std::vector<std::string>> parent4actual = readPKB.findRelationship(parentTTest4);
        REQUIRE(parent4actual.empty());

        shared_ptr<Relationship> parentTTest5 = Relationship::makeRelationship(AppConstants::PARENTT,
                                                                              {Parameter("w", AppConstants::WHILE),
                                                                               Parameter("ca", AppConstants::CALL)}
        );
        std::vector<std::vector<std::string>> parentT5actual = readPKB.findRelationship(parentTTest5);
        std::vector<std::vector<std::string>> parentT5expected = { { "3", "10" }, { "7", "10" } };
        std::sort(parentT5actual.begin(), parentT5actual.end());
        std::sort(parentT5expected.begin(), parentT5expected.end());
        REQUIRE(parentT5expected == parentT5actual);


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
            testSP.processFile(filePath, &writePKB, &readPKB); //execution should stop here.
            REQUIRE(false);
        } catch (std::exception e) {
            REQUIRE(true);
        }
    }
}