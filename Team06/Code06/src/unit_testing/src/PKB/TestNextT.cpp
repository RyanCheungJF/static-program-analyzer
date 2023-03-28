#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

TEST_CASE("findRelationship(shared_ptr<Relationship> rs): Next*") {
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

    std::unordered_set<StmtNum> vals = {1, 2, 3, 4, 5, 6, 7, 8};
    writePkb.setProcedure(proc1, vals);

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
    std::unordered_set<StmtNum> vals2 = {9, 10, 11, 12};
    writePkb.setProcedure(proc2, vals2);

    SECTION("Next*(int, int)") {
        std::vector<Parameter> params = {Parameter("1", ParameterType::FIXED_INT),
                                         Parameter("2", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"1", "2"}}, res));
    }

    SECTION("Next*(int, _)") {
        std::vector<Parameter> params = {Parameter("1", ParameterType::FIXED_INT),
                                         Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected = {{"1", "2"}, {"1", "3"}, {"1", "4"}, {"1", "5"},
                                                          {"1", "6"}, {"1", "7"}, {"1", "8"}};
        REQUIRE(unit_testing_utils::equals(expected, res));
    }

    SECTION("Next*(int, stmt)") {
        std::vector<Parameter> params = {Parameter("10", ParameterType::FIXED_INT),
                                         Parameter("s2", ParameterType::STMT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected = {{"10", "11"}, {"10", "12"}, {"10", "9"}, {"10", "10"}};

        REQUIRE(unit_testing_utils::equals(expected, res));
    }

    SECTION("Next*(_, int)") {
        std::vector<Parameter> params = {Parameter("_", ParameterType::WILDCARD),
                                         Parameter("5", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected = {{"1", "5"}, {"2", "5"}, {"3", "5"}, {"4", "5"}};
        REQUIRE(unit_testing_utils::equals(expected, res));
    }

    SECTION("Next*(stmt, int)") {
        std::vector<Parameter> params = {Parameter("asTeeAmTii", ParameterType::STMT),
                                         Parameter("1", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({}, res));

        std::vector<Parameter> params2 = {Parameter("asTeeAmTii", ParameterType::STMT),
                                          Parameter("8", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXTT, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected = {{"7", "8"}, {"6", "8"}, {"5", "8"}, {"2", "8"},
                                                          {"4", "8"}, {"1", "8"}, {"3", "8"}};
        REQUIRE(unit_testing_utils::equals(expected, res2));
    }

    SECTION("Next*(stmttype, int)") {
        std::vector<Parameter> params1 = {Parameter("a1", ParameterType::ASSIGN),
                                          Parameter("8", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"7", "8"}, {"2", "8"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("i1", ParameterType::IF), Parameter("7", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXTT, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"1", "7"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("re", ParameterType::READ),
                                          Parameter("8", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::NEXTT, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {{"3", "8"}};
        REQUIRE(unit_testing_utils::equals(expected3, res3));
    }

    SECTION("Next*(int, stmttype)") {
        std::vector<Parameter> params1 = {Parameter("11", ParameterType::FIXED_INT),
                                          Parameter("w", ParameterType::WHILE)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals({{"11", "9"}}, res1));

        std::vector<Parameter> params2 = {Parameter("11", ParameterType::FIXED_INT),
                                          Parameter("asgn", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXTT, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals({{"11", "10"}, {"11", "11"}}, res2));

        std::vector<Parameter> params3 = {Parameter("4", ParameterType::FIXED_INT),
                                          Parameter("ca", ParameterType::CALL)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::NEXTT, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals({{"4", "5"}}, res3));

        std::vector<Parameter> params4 = {Parameter("5", ParameterType::FIXED_INT),
                                          Parameter("ca", ParameterType::CALL)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::NEXT, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        REQUIRE(unit_testing_utils::equals({}, res4));
    }

    SECTION("Next*(stmttype, _)") {
        std::vector<Parameter> params = {Parameter("a2", ParameterType::ASSIGN),
                                         Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected = {
            {"2", "5"},   {"2", "6"},  {"2", "7"},   {"2", "8"},  {"7", "8"},   {"7", "6"},   {"7", "7"},  {"10", "11"},
            {"10", "12"}, {"10", "9"}, {"10", "10"}, {"11", "9"}, {"11", "12"}, {"11", "10"}, {"11", "11"}};
        REQUIRE(unit_testing_utils::equals(expected, res));
    }

    SECTION("Next*(stmttype, stmt)") {
        std::vector<Parameter> params = {Parameter("i2", ParameterType::IF), Parameter("s2", ParameterType::STMT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected = {{"1", "3"}, {"1", "2"}, {"1", "4"}, {"1", "5"},
                                                          {"1", "6"}, {"1", "7"}, {"1", "8"}};
        REQUIRE(unit_testing_utils::equals(expected, res));

        std::vector<Parameter> params2 = {Parameter("ww", ParameterType::WHILE), Parameter("ss", ParameterType::STMT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXTT, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"6", "7"},  {"6", "8"},  {"6", "6"}, {"9", "10"},
                                                           {"9", "11"}, {"9", "12"}, {"9", "9"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));
    }

    SECTION("Next*(_, stmttype)") {
        std::vector<Parameter> params = {Parameter("_", ParameterType::WILDCARD),
                                         Parameter("prr", ParameterType::PRINT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"3", "4"}, {"1", "4"},   {"7", "8"},  {"6", "8"},
                                                           {"5", "8"}, {"2", "8"},   {"4", "8"},  {"1", "8"},
                                                           {"3", "8"}, {"10", "12"}, {"9", "12"}, {"11", "12"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("as", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXTT, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {
            {"1", "2"}, {"6", "7"},  {"5", "7"},   {"7", "7"},   {"2", "7"},   {"4", "7"},  {"1", "7"},
            {"3", "7"}, {"9", "10"}, {"11", "10"}, {"10", "10"}, {"10", "11"}, {"9", "11"}, {"11", "11"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));
    }

    SECTION("Next*(stmt, stmttype)") {
        std::vector<Parameter> params = {Parameter("st3", ParameterType::STMT), Parameter("re2", ParameterType::WHILE)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"7", "6"},  {"5", "6"}, {"6", "6"}, {"2", "6"},
                                                           {"4", "6"},  {"1", "6"}, {"3", "6"}, {"11", "9"},
                                                           {"10", "9"}, {"9", "9"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }

    SECTION("Next*(stmttype, stmttype)") {
        std::vector<Parameter> params = {Parameter("w3", ParameterType::WHILE),
                                         Parameter("as3", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"6", "7"}, {"9", "10"}, {"9", "11"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("as4", ParameterType::ASSIGN),
                                          Parameter("w4", ParameterType::WHILE)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXTT, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"2", "6"}, {"7", "6"}, {"10", "9"}, {"11", "9"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("as5", ParameterType::ASSIGN),
                                          Parameter("as5", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::NEXTT, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals({{"7", "7"}, {"11", "11"}, {"10", "10"}}, res3));
    }

    SECTION("Next*(_, _)") {
        std::vector<std::vector<std::string>> expected = {
            {"1", "3"},  {"1", "2"},   {"1", "4"},  {"1", "5"},   {"1", "6"},   {"1", "7"},   {"1", "8"},
            {"2", "5"},  {"2", "6"},   {"2", "7"},  {"2", "8"},   {"3", "4"},   {"3", "5"},   {"3", "6"},
            {"3", "7"},  {"3", "8"},   {"4", "5"},  {"4", "6"},   {"4", "7"},   {"4", "8"},   {"5", "6"},
            {"5", "7"},  {"5", "8"},   {"6", "7"},  {"6", "6"},   {"6", "8"},   {"7", "8"},   {"7", "6"},
            {"7", "7"},  {"9", "10"},  {"9", "11"}, {"9", "12"},  {"9", "9"},   {"10", "11"}, {"10", "12"},
            {"10", "9"}, {"10", "10"}, {"11", "9"}, {"11", "12"}, {"11", "10"}, {"11", "11"}};

        std::vector<Parameter> params = {Parameter("_", ParameterType::WILDCARD),
                                         Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals(expected, res1));
    }
}

TEST_CASE("findRelationship(shared_ptr<Relationship> rs): Next* - complex") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    //  : are self pointing nodes allowed?
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph3 = {
        {13, {{AppConstants::PARENTS, {15, 16}}, {AppConstants::CHILDREN, {14}}}},
        {14, {{AppConstants::PARENTS, {13}}, {AppConstants::CHILDREN, {15, 17}}}},
        {15, {{AppConstants::PARENTS, {14}}, {AppConstants::CHILDREN, {16, 13}}}},
        {16, {{AppConstants::PARENTS, {15}}, {AppConstants::CHILDREN, {15, 13, 18}}}},
        {17, {{AppConstants::PARENTS, {14, 17}}, {AppConstants::CHILDREN, {18, 17, 13}}}},
        {18, {{AppConstants::PARENTS, {16, 17}}, {AppConstants::CHILDREN, {19, 20}}}},
        {19, {{AppConstants::PARENTS, {18}}, {AppConstants::CHILDREN, {}}}},
        {20, {{AppConstants::PARENTS, {19}}, {AppConstants::CHILDREN, {21, 22}}}},
        {21, {{AppConstants::PARENTS, {20}}, {AppConstants::CHILDREN, {}}}},
        {22, {{AppConstants::PARENTS, {20}}, {AppConstants::CHILDREN, {22}}}}};

    ProcName proc3 = "proc3";
    writePkb.writeCFG(proc3, graph3);
    writePkb.setStatement(AppConstants::WHILE, 13);
    writePkb.setStatement(AppConstants::IF, 14);
    writePkb.setStatement(AppConstants::WHILE, 15);
    writePkb.setStatement(AppConstants::PRINT, 16);
    writePkb.setStatement(AppConstants::WHILE, 17);
    writePkb.setStatement(AppConstants::IF, 18);
    writePkb.setStatement(AppConstants::CALL, 19);
    writePkb.setStatement(AppConstants::IF, 20);
    writePkb.setStatement(AppConstants::CALL, 21);
    writePkb.setStatement(AppConstants::WHILE, 22);
    std::unordered_set<StmtNum> vals1 = {13, 14, 15, 16, 17, 18, 19, 20, 21, 22};
    writePkb.setProcedure(proc3, vals1);

    SECTION("Next*(_, _)") {
        std::vector<std::vector<std::string>> expected = {
            {"13", "14"}, {"13", "15"}, {"13", "17"}, {"13", "16"}, {"13", "13"}, {"13", "18"}, {"13", "19"},
            {"13", "20"}, {"13", "21"}, {"13", "22"}, {"14", "17"}, {"14", "15"}, {"14", "18"}, {"14", "13"},
            {"14", "16"}, {"14", "19"}, {"14", "20"}, {"14", "14"}, {"14", "21"}, {"14", "22"}, {"15", "13"},
            {"15", "16"}, {"15", "14"}, {"15", "15"}, {"15", "18"}, {"15", "17"}, {"15", "19"}, {"15", "20"},
            {"15", "21"}, {"15", "22"}, {"16", "18"}, {"16", "13"}, {"16", "15"}, {"16", "19"}, {"16", "20"},
            {"16", "14"}, {"16", "16"}, {"16", "21"}, {"16", "22"}, {"16", "17"}, {"17", "17"}, {"17", "13"},
            {"17", "18"}, {"17", "14"}, {"17", "19"}, {"17", "20"}, {"17", "15"}, {"17", "21"}, {"17", "22"},
            {"17", "16"}, {"18", "20"}, {"18", "19"}, {"18", "21"}, {"18", "22"}, {"20", "22"}, {"20", "21"},
            {"22", "22"}};

        std::vector<Parameter> params = {Parameter("_", ParameterType::WILDCARD),
                                         Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        REQUIRE(unit_testing_utils::equals(expected, res1));

        std::vector<Parameter> params2 = {Parameter("st1", ParameterType::STMT), Parameter("st2", ParameterType::STMT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXTT, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected, res2));
    }
}
