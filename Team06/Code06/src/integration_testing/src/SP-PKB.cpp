#include "catch.hpp"
#include <filesystem>
#include "../../spa/src/utils/AppConstants.h"
#include "../../spa/src/SP/SP.h"
#include "../../spa/src/qps/entities/Relationship.h"
#include "../../unit_testing/src/utils/utils.h"

using namespace unit_testing_utils;

TEST_CASE("Valid Source Program") {
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

    SECTION("Simple multi-procedure SIMPLE program") {
        auto filePath = testDirectory.string() + "valid1.txt";
        testSP.processFile(filePath, &writePKB, &readPKB);

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

        // Follows
        std::vector<Parameter> params1 = {Parameter("1", AppConstants::FIXED_INT), Parameter("2", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::FOLLOWS, params1);
        std::vector<std::vector<std::string>> follows1actual = readPKB.findRelationship(rs1);
        REQUIRE(follows1actual.empty());

        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                      {Parameter("2", AppConstants::FIXED_INT),
                                                                       Parameter("3", AppConstants::FIXED_INT)}
                                                                       );
        std::vector<std::vector<std::string>> follows2actual = readPKB.findRelationship(rs2);
        std::vector<std::vector<std::string>> follows2expected = {{"2", "3"}};
        REQUIRE(follows2expected == follows2actual);

        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                      {Parameter("4", AppConstants::FIXED_INT),
                                                                       Parameter("5", AppConstants::FIXED_INT)}
        );
        std::vector<std::vector<std::string>> follows3actual = readPKB.findRelationship(rs3);
        std::vector<std::vector<std::string>> follows3expected = {{"4", "5"}};
        REQUIRE(follows3expected == follows3actual);

        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                      {Parameter("5", AppConstants::FIXED_INT),
                                                                       Parameter("6", AppConstants::FIXED_INT)}
        );
        std::vector<std::vector<std::string>> follows4actual = readPKB.findRelationship(rs4);
        std::vector<std::vector<std::string>> follows4expected = {{"5", "6"}};
        REQUIRE(follows4expected == follows4actual);

        shared_ptr<Relationship> rs5 = Relationship::makeRelationship(AppConstants::FOLLOWS,
                                                                      {Parameter("4", AppConstants::FIXED_INT),
                                                                       Parameter("6", AppConstants::FIXED_INT)}
        );
        std::vector<std::vector<std::string>> follows5actual = readPKB.findRelationship(rs5);
        REQUIRE(follows5actual.empty());


//        std::cout << "follows2actual results:\n";
//        for (auto i :follows2actual) {
//            std::cout << i[0] << " " << i[1] << "\n";
//        }


        // FollowsT

        // ParentT

        // ParentT

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