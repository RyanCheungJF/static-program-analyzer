#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

TEST_CASE("findRelationship(shared_ptr<Relationship> rs): Next") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph1 = {
        {1, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {2, 3}}}},
        {2, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {5}}}},
        {3, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {4}}}},
        {4, {{AppConstants::PARENTS, {3}}, {AppConstants::CHILDREN, {5}}}},
        {5, {{AppConstants::PARENTS, {2, 4}}, {AppConstants::CHILDREN, {6}}}},
        {6, {{AppConstants::PARENTS, {5, 7}}, {AppConstants::CHILDREN, {7}}}},
        {7, {{AppConstants::PARENTS, {6}}, {AppConstants::CHILDREN, {6, 8}}}},
        {8, {{AppConstants::PARENTS, {7}}, {AppConstants::CHILDREN, {}}}}};

    ProcName proc1 = "proc1";
    writePkb.writeCFG(proc1, graph1);
    writePkb.setStatement(AppConstants::IF, 1);
    writePkb.setStatement(AppConstants::ASSIGN, 2);
    writePkb.setStatement(AppConstants::READ, 3);
    writePkb.setStatement(AppConstants::PRINT, 4);
    writePkb.setStatement(AppConstants::CALL, 5);
    writePkb.setStatement(AppConstants::WHILE, 6);
    writePkb.setStatement(AppConstants::ASSIGN, 7);
    writePkb.setStatement(AppConstants::PRINT, 8);
    writePkb.setProcedure(proc1, {1, 2, 3, 4, 5, 6, 7, 8});

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph2 = {
        {9, {{AppConstants::PARENTS, {11}}, {AppConstants::CHILDREN, {10}}}},
        {10, {{AppConstants::PARENTS, {9}}, {AppConstants::CHILDREN, {11}}}},
        {11, {{AppConstants::PARENTS, {10}}, {AppConstants::CHILDREN, {12, 9}}}},
        {12, {{AppConstants::PARENTS, {10}}, {AppConstants::CHILDREN, {}}}}};

    ProcName proc2 = "proc2";
    writePkb.writeCFG(proc2, graph2);
    writePkb.setStatement(AppConstants::WHILE, 9);
    writePkb.setStatement(AppConstants::ASSIGN, 10);
    writePkb.setStatement(AppConstants::ASSIGN, 11);
    writePkb.setStatement(AppConstants::PRINT, 12);
    writePkb.setProcedure(proc2, {9, 10, 11, 12});

    SECTION("Next(int, int)") {
        std::vector<Parameter> params = {Parameter("1", ParameterType::FIXED_INT),
                                         Parameter("2", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"1", "2"}}, res));
    }

    SECTION("Next(int, _)") {
        std::vector<Parameter> params = {Parameter("1", ParameterType::FIXED_INT),
                                         Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"1", "2"}, {"1", "3"}}, res));
    }

    SECTION("Next(int, stmt)") {
        std::vector<Parameter> params = {Parameter("10", ParameterType::FIXED_INT),
                                         Parameter("s2", ParameterType::STMT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"10", "11"}}, res));
    }

    SECTION("Next(_, int)") {
        std::vector<Parameter> params = {Parameter("_", ParameterType::WILDCARD),
                                         Parameter("5", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"2", "5"}, {"4", "5"}}, res));
    }

    SECTION("Next(stmt, int)") {
        std::vector<Parameter> params = {Parameter("asTeeAmTii", ParameterType::STMT),
                                         Parameter("1", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({}, res));

        std::vector<Parameter> params2 = {Parameter("asTeeAmTii", ParameterType::STMT),
                                          Parameter("8", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXT, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals({{"7", "8"}}, res2));
    }

    SECTION("Next(stmttype, int)") {
        std::vector<Parameter> params1 = {Parameter("a1", ParameterType::ASSIGN),
                                          Parameter("8", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"7", "8"}}, res1));

        std::vector<Parameter> params2 = {Parameter("i1", ParameterType::IF), Parameter("7", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXT, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals({}, res2));

        std::vector<Parameter> params3 = {Parameter("re", ParameterType::READ),
                                          Parameter("4", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::NEXT, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals({{"3", "4"}}, res3));
    }

    SECTION("Next(int, stmttype)") {
        std::vector<Parameter> params1 = {Parameter("11", ParameterType::FIXED_INT),
                                          Parameter("w", ParameterType::WHILE)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"11", "9"}}, res1));

        std::vector<Parameter> params2 = {Parameter("11", ParameterType::FIXED_INT),
                                          Parameter("prr", ParameterType::PRINT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXT, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals({{"11", "12"}}, res2));

        std::vector<Parameter> params3 = {Parameter("4", ParameterType::FIXED_INT),
                                          Parameter("ca", ParameterType::CALL)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::NEXT, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals({{"4", "5"}}, res3));

        std::vector<Parameter> params4 = {Parameter("5", ParameterType::FIXED_INT),
                                          Parameter("ca", ParameterType::CALL)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::NEXT, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        REQUIRE(unit_testing_utils::equals({}, res4));
    }

    SECTION("Next(stmttype, _)") {
        std::vector<Parameter> params = {Parameter("a2", ParameterType::ASSIGN),
                                         Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals(
            {{"2", "5"}, {"7", "6"}, {"7", "8"}, {"11", "9"}, {"10", "11"}, {"11", "12"}}, res));
    }

    SECTION("Next(stmttype, stmt)") {
        std::vector<Parameter> params = {Parameter("i2", ParameterType::IF), Parameter("s2", ParameterType::STMT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"1", "2"}, {"1", "3"}}, res));
    }

    SECTION("Next(_, stmttype)") {
        std::vector<Parameter> params = {Parameter("_", ParameterType::WILDCARD),
                                         Parameter("re2", ParameterType::CALL)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"2", "5"}, {"4", "5"}}, res));
    }

    SECTION("Next(stmt, stmttype)") {
        std::vector<Parameter> params = {Parameter("st3", ParameterType::STMT), Parameter("re2", ParameterType::WHILE)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"5", "6"}, {"7", "6"}, {"11", "9"}}, res));
    }

    SECTION("Next(stmttype, stmttype)") {
        std::vector<Parameter> params = {Parameter("w3", ParameterType::WHILE),
                                         Parameter("as3", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"6", "7"}, {"9", "10"}}, res));

        std::vector<Parameter> params2 = {Parameter("as4", ParameterType::ASSIGN),
                                          Parameter("w4", ParameterType::WHILE)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXT, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals({{"7", "6"}, {"11", "9"}}, res2));

        std::vector<Parameter> params3 = {Parameter("as5", ParameterType::ASSIGN),
                                          Parameter("as5", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::NEXT, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals({}, res3));
    }

    SECTION("Next(_, _)") {
        std::vector<std::vector<std::string>> expected = {
            {"1", "3"}, {"1", "2"}, {"2", "5"},  {"3", "4"},   {"4", "5"},  {"5", "6"},  {"6", "7"},
            {"7", "8"}, {"7", "6"}, {"9", "10"}, {"10", "11"}, {"11", "9"}, {"11", "12"}};

        std::vector<Parameter> params = {Parameter("_", ParameterType::WILDCARD),
                                         Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals(expected, res));
    }
}