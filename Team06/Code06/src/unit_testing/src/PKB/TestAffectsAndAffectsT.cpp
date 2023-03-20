#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

TEST_CASE("findRelationship(shared_ptr<Relationship> rs): Affects & Affects* 1") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph1 = {
        {1, {{AppConstants::PARENTS, {19}}, {AppConstants::CHILDREN, {2, 20}}}},
        {2, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {3}}}},
        {3, {{AppConstants::PARENTS, {2}}, {AppConstants::CHILDREN, {4}}}},
        {4, {{AppConstants::PARENTS, {3}}, {AppConstants::CHILDREN, {5, 6}}}},
        {5, {{AppConstants::PARENTS, {4}}, {AppConstants::CHILDREN, {7}}}},
        {6, {{AppConstants::PARENTS, {4}}, {AppConstants::CHILDREN, {7}}}},
        {7, {{AppConstants::PARENTS, {5, 6}}, {AppConstants::CHILDREN, {8}}}},
        {8, {{AppConstants::PARENTS, {7}}, {AppConstants::CHILDREN, {9}}}},
        {9, {{AppConstants::PARENTS, {8}}, {AppConstants::CHILDREN, {10}}}},
        {10, {{AppConstants::PARENTS, {9}}, {AppConstants::CHILDREN, {11}}}},
        {11, {{AppConstants::PARENTS, {10}}, {AppConstants::CHILDREN, {12}}}},
        {12, {{AppConstants::PARENTS, {11}}, {AppConstants::CHILDREN, {13}}}},
        {13, {{AppConstants::PARENTS, {12}}, {AppConstants::CHILDREN, {14}}}},
        {14, {{AppConstants::PARENTS, {13}}, {AppConstants::CHILDREN, {15}}}},
        {15, {{AppConstants::PARENTS, {14}}, {AppConstants::CHILDREN, {16}}}},
        {16, {{AppConstants::PARENTS, {15}}, {AppConstants::CHILDREN, {17}}}},
        {17, {{AppConstants::PARENTS, {16}}, {AppConstants::CHILDREN, {18}}}},
        {18, {{AppConstants::PARENTS, {17}}, {AppConstants::CHILDREN, {19}}}},
        {18, {{AppConstants::PARENTS, {17}}, {AppConstants::CHILDREN, {19}}}},
        {19, {{AppConstants::PARENTS, {18}}, {AppConstants::CHILDREN, {1}}}},
        {20, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {21}}}},
        {21, {{AppConstants::PARENTS, {20}}, {AppConstants::CHILDREN, {22}}}},
        {22, {{AppConstants::PARENTS, {21}}, {AppConstants::CHILDREN, {23}}}},
        {23, {{AppConstants::PARENTS, {22}}, {AppConstants::CHILDREN, {24, 25}}}},
        {24, {{AppConstants::PARENTS, {23}}, {AppConstants::CHILDREN, {27}}}},
        {25, {{AppConstants::PARENTS, {23}}, {AppConstants::CHILDREN, {26}}}},
        {26, {{AppConstants::PARENTS, {25}}, {AppConstants::CHILDREN, {27}}}},
        {27, {{AppConstants::PARENTS, {24, 26}}, {AppConstants::CHILDREN, {28}}}},
        {28, {{AppConstants::PARENTS, {27}}, {AppConstants::CHILDREN, {}}}}};

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph2 = {
        {29, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {}}}}};
    /*
     * while (a != b) { //1
     *     y = v + 1; //2
     *     x = x + 3; //3
     *     if (x == y) then { //4
     *         v = y + 3; //5
     *     } else {
     *         y = x + 3; //6
     *     }
     *     v = 2 + 1; //7
     *     v = v + 1; //8
     *     x = y + 3; //9
     *     c = 4 + 1; //10
     *     d = c + 1; //11
     *     f = e + 1; //12
     *     e = g + 1; //13
     *     a = a + 1; //14
     *     read b; //15
     *     call proc2; //16, modifies j
     *     print d; //17
     *     d = d + 1; //18
     *     h = h + 1; // 19
     * }
     * i = h + 1 + i; //20
     * call proc2; //21
     * j = j + 1; //22
     * if (k != l) then { //23
     *     k = l; //24
     * } else {
     *     l = k; //25
     *     m = i; //26
     * }
     * print k; //27
     * print l; //28
     *
     *
     * proc2:
     * j = j + 1; //29
     *
     * Valid Affects relationships:
     * (2, 5), (2, 9), (3, 6), (6, 9),
     * (7, 8), (8, 2), (9, 3),
     * (10, 11), (11, 18) (13, 12),
     * (14, 14), (19, 19), (19, 20)
     * (20, 26)
     *
     * Valid Affects* relationships:
     * (2, 5), (2, 3), (2, 6), (2, 9)
     * (3, 6), (3, 9), (3, 3),
     * (6, 9), (6, 3), (6, 6)
     * (7, 8), (7, 2), (7, 5), (7, 9), (7, 6), (7, 3)
     * (8, 2), (8, 5, (8, 9), (8, 6), (8, 3)
     * (9, 3), (9, 6), (9, 9)
     * (10, 11), (10, 18)
     * (11, 18)
     * (13, 12)
     * (14, 14)
     * (19, 19), (19, 20), (19, 26)
     * (20, 26)
     */

    ProcName proc1 = "proc1";
    ProcName proc2 = "proc2";
    writePkb.writeCFG(proc1, graph1);
    writePkb.writeCFG(proc2, graph2);

    writePkb.setStatement(AppConstants::WHILE, 1);
    writePkb.setStatement(AppConstants::ASSIGN, 2);
    writePkb.setStatement(AppConstants::ASSIGN, 3);
    writePkb.setStatement(AppConstants::IF, 4);
    writePkb.setStatement(AppConstants::ASSIGN, 5);
    writePkb.setStatement(AppConstants::ASSIGN, 6);
    writePkb.setStatement(AppConstants::ASSIGN, 7);
    writePkb.setStatement(AppConstants::ASSIGN, 8);
    writePkb.setStatement(AppConstants::ASSIGN, 9);
    writePkb.setStatement(AppConstants::ASSIGN, 10);
    writePkb.setStatement(AppConstants::ASSIGN, 11);
    writePkb.setStatement(AppConstants::ASSIGN, 12);
    writePkb.setStatement(AppConstants::ASSIGN, 13);
    writePkb.setStatement(AppConstants::ASSIGN, 14);
    writePkb.setStatement(AppConstants::READ, 15);
    writePkb.setStatement(AppConstants::CALL, 16);
    writePkb.setCall(16, proc2);
    writePkb.setStatement(AppConstants::PRINT, 17);
    writePkb.setStatement(AppConstants::ASSIGN, 18);
    writePkb.setStatement(AppConstants::ASSIGN, 19);
    writePkb.setStatement(AppConstants::ASSIGN, 20);
    writePkb.setStatement(AppConstants::CALL, 21);
    writePkb.setCall(21, proc2);
    writePkb.setStatement(AppConstants::ASSIGN, 22);
    writePkb.setStatement(AppConstants::IF, 23);
    writePkb.setStatement(AppConstants::ASSIGN, 24);
    writePkb.setStatement(AppConstants::ASSIGN, 25);
    writePkb.setStatement(AppConstants::ASSIGN, 26);
    writePkb.setStatement(AppConstants::PRINT, 27);
    writePkb.setStatement(AppConstants::PRINT, 28);

    writePkb.setProcedure(
        proc1, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27});
    writePkb.setStatement(AppConstants::ASSIGN, 28);
    writePkb.setProcedure(proc2, {28});

    writePkb.setModifiesS(1, {"y", "x", "v"});
    writePkb.setModifiesS(2, {"y"});
    writePkb.setModifiesS(3, {"x"});
    writePkb.setModifiesS(4, {"v", "y"});
    writePkb.setModifiesS(5, {"v"});
    writePkb.setModifiesS(6, {"y"});
    writePkb.setModifiesS(7, {"v"});
    writePkb.setModifiesS(8, {"v"});
    writePkb.setModifiesS(9, {"x"});
    writePkb.setModifiesS(10, {"c"});
    writePkb.setModifiesS(11, {"d"});
    writePkb.setModifiesS(12, {"f"});
    writePkb.setModifiesS(13, {"e"});
    writePkb.setModifiesS(14, {"a"});
    writePkb.setModifiesS(15, {"b"});
    writePkb.setModifiesS(16, {"j"});
    //    writePkb.setModifiesS(17, {});
    writePkb.setModifiesS(18, {"d"});
    writePkb.setModifiesS(19, {"h"});
    writePkb.setModifiesS(20, {"i"});
    //    writePkb.setModifiesS(21, {});
    writePkb.setModifiesS(22, {"j"});
    writePkb.setModifiesS(23, {"k", "l"});
    writePkb.setModifiesS(24, {"k"});
    writePkb.setModifiesS(25, {"l"});
    writePkb.setModifiesS(26, {"m"});
    writePkb.setModifiesP(proc1, {"y", "x", "v", "c", "d", "f", "e", "a", "b", "h", "j", "k", "l", "m", "i"});

    writePkb.setModifiesS(29, {"j"});
    writePkb.setModifiesP(proc2, {"j"});

    writePkb.setUsesS(1, {"a", "b"});
    writePkb.setUsesS(2, {"v"});
    writePkb.setUsesS(3, {"x"});
    writePkb.setUsesS(4, {"x", "v", "y"});
    writePkb.setUsesS(5, {"y"});
    writePkb.setUsesS(6, {"x"});
    writePkb.setUsesS(7, {});
    writePkb.setUsesS(8, {"v"});
    writePkb.setUsesS(9, {"y"});
    //    writePkb.setUsesS(10, {});
    writePkb.setUsesS(11, {"c"});
    writePkb.setUsesS(12, {"e"});
    writePkb.setUsesS(13, {"g"});
    writePkb.setUsesS(14, {"a"});
    writePkb.setUsesS(17, {"d"});
    writePkb.setUsesS(18, {"d"});
    writePkb.setUsesS(19, {"h"});
    writePkb.setUsesS(20, {"h", "i"});
    //    writePkb.setUsesS(21, {});
    writePkb.setUsesS(22, {"j"});
    writePkb.setUsesS(23, {"k", "l"});
    writePkb.setUsesS(24, {"l"});
    writePkb.setUsesS(25, {"k"});
    writePkb.setUsesS(26, {"i"});
    writePkb.setUsesS(27, {"k"});
    writePkb.setUsesS(28, {"l"});
    writePkb.setUsesP(proc1, {"a", "b", "y", "x", "v", "c", "e", "g", "d", "h", "j", "k", "l", "i"});

    writePkb.setUsesS(29, {"j"});
    writePkb.setUsesP(proc2, {"j"});

    SECTION("Affects(int, int)") {
        std::vector<Parameter> params1 = {Parameter("6", ParameterType::FIXED_INT),
                                          Parameter("9", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"6", "9"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("7", ParameterType::FIXED_INT),
                                          Parameter("8", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"7", "8"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("8", ParameterType::FIXED_INT),
                                          Parameter("8", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(res3.empty());

        std::vector<Parameter> params4 = {Parameter("22", ParameterType::FIXED_INT),
                                          Parameter("22", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(expected4 == res4);

        std::vector<Parameter> params5 = {Parameter("24", ParameterType::FIXED_INT),
                                          Parameter("25", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs5 = Relationship::makeRelationship(AppConstants::AFFECTS, params5);
        std::vector<std::vector<std::string>> res5 = readPkb.findRelationship(rs5);
        std::vector<std::vector<std::string>> expected5 = {};
        REQUIRE(expected5 == res5);

        std::vector<Parameter> params6 = {Parameter("20", ParameterType::FIXED_INT),
                                          Parameter("26", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs6 = Relationship::makeRelationship(AppConstants::AFFECTS, params6);
        std::vector<std::vector<std::string>> res6 = readPkb.findRelationship(rs6);
        std::vector<std::vector<std::string>> expected6 = {{"20", "26"}};
        REQUIRE(expected6 == res6);
    }

    SECTION("Affects(int, _)") {
        std::vector<Parameter> params1 = {Parameter("19", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"19", "19"}, {"19", "20"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("1", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(expected2 == res2);

        std::vector<Parameter> params3 = {Parameter("15", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {};
        REQUIRE(expected3 == res3);

        std::vector<Parameter> params4 = {Parameter("4", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(expected4 == res4);

        std::vector<Parameter> params5 = {Parameter("21", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs5 = Relationship::makeRelationship(AppConstants::AFFECTS, params5);
        std::vector<std::vector<std::string>> res5 = readPkb.findRelationship(rs5);
        std::vector<std::vector<std::string>> expected5 = {};
        REQUIRE(expected5 == res5);

        std::vector<Parameter> params6 = {Parameter("20", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs6 = Relationship::makeRelationship(AppConstants::AFFECTS, params6);
        std::vector<std::vector<std::string>> res6 = readPkb.findRelationship(rs6);
        std::vector<std::vector<std::string>> expected6 = {{"20", "26"}};
        REQUIRE(expected6 == res6);
    }

    SECTION("Affects(_, int)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("3", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"9", "3"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("1", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("4", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {};
        REQUIRE(unit_testing_utils::equals(expected3, res3));

        std::vector<Parameter> params4 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("16", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(unit_testing_utils::equals(expected4, res4));

        std::vector<Parameter> params5 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("26", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs5 = Relationship::makeRelationship(AppConstants::AFFECTS, params5);
        std::vector<std::vector<std::string>> res5 = readPkb.findRelationship(rs5);
        std::vector<std::vector<std::string>> expected5 = {{"20", "26"}};
        REQUIRE(unit_testing_utils::equals(expected5, res5));
    }

    SECTION("Affects(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {
            {"2", "5"},   {"2", "9"},   {"3", "6"},   {"6", "9"},   {"7", "8"},   {"8", "2"},   {"9", "3"},
            {"10", "11"}, {"11", "18"}, {"13", "12"}, {"14", "14"}, {"19", "20"}, {"19", "19"}, {"20", "26"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("aa", ParameterType::ASSIGN),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected1, res2));

        std::vector<Parameter> params3 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("as", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals(expected1, res3));

        std::vector<Parameter> params4 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        REQUIRE(unit_testing_utils::equals(expected1, res4));
    }

    SECTION("Affects*(int, int)") {
        std::vector<Parameter> params1 = {Parameter("19", ParameterType::FIXED_INT),
                                          Parameter("20", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"19", "20"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("7", ParameterType::FIXED_INT),
                                          Parameter("5", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"7", "5"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("8", ParameterType::FIXED_INT),
                                          Parameter("8", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTST, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(res3.empty());
    }

    SECTION("Affects*(int, _)") {
        std::vector<Parameter> params1 = {Parameter("9", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"9", "3"}, {"9", "6"}, {"9", "9"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("1", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(expected2 == res2);

        std::vector<Parameter> params3 = {Parameter("15", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTST, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {};
        REQUIRE(expected3 == res3);

        std::vector<Parameter> params4 = {Parameter("4", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTST, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(expected4 == res4);
    }

    SECTION("Affects*(_, int)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("6", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"2", "6"}, {"3", "6"}, {"6", "6"},
                                                           {"7", "6"}, {"8", "6"}, {"9", "6"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("1", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("4", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTST, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {};
        REQUIRE(unit_testing_utils::equals(expected3, res3));

        std::vector<Parameter> params4 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("16", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTST, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(unit_testing_utils::equals(expected4, res4));
    }

    SECTION("Affects*(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {
            {"2", "5"},   {"2", "3"},   {"2", "6"},   {"2", "9"},   {"3", "6"},   {"3", "9"},   {"3", "3"},
            {"6", "9"},   {"6", "3"},   {"6", "6"},   {"7", "8"},   {"7", "2"},   {"7", "5"},   {"7", "9"},
            {"7", "6"},   {"7", "3"},   {"8", "2"},   {"8", "5"},   {"8", "9"},   {"8", "6"},   {"8", "3"},
            {"9", "3"},   {"9", "6"},   {"9", "9"},   {"10", "11"}, {"10", "18"}, {"11", "18"}, {"13", "12"},
            {"14", "14"}, {"19", "19"}, {"19", "20"}, {"19", "26"}, {"20", "26"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("aa", ParameterType::ASSIGN),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected1, res2));

        std::vector<Parameter> params3 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("as", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTST, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals(expected1, res3));

        std::vector<Parameter> params4 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTST, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        REQUIRE(unit_testing_utils::equals(expected1, res4));

        std::vector<Parameter> params5 = {Parameter("as", ParameterType::ASSIGN),
                                          Parameter("as", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs5 = Relationship::makeRelationship(AppConstants::AFFECTST, params5);
        std::vector<std::vector<std::string>> res5 = readPkb.findRelationship(rs5);
        std::vector<std::vector<std::string>> expected5 = {
            {"3", "3"}, {"6", "6"}, {"9", "9"}, {"14", "14"}, {"19", "19"}};
        REQUIRE(unit_testing_utils::equals(expected5, res5));
    }
}

TEST_CASE("findRelationship(shared_ptr<Relationship> rs): Affects & Affects* 2") {
    /*
     *
     * procedure One {
     *     x = x + 1; \\1
     *     y = y + x; \\2
     *     z = x + y + c; \\3
     *     c = 12345; \\4
     *     while (z == x + y) { \\5
     *         print c; \\6
     *         c = c + 0; \\7
     *     }
     *     call Two; \\8
     *     call Three; \\9
     *     call Four; \\10
     * }
     *
     * procedure Two {
     *     y = x + 2; \\11
     *     a = 999 + c; \\12
     *     b = a + y * x / z; \\13
     *     c = 1 + 2; \\14
     *     read b; \\15
     *     if (b != c) then { \\16
     *         call Three; \\17
     *         call Four; \\18
     *     } else {
     *         a = b; \\19
     *     }
     * }
     *
     * procedure Three {
     *     d = 100; \\20
     *     e = d; \\21
     *     call Six; \\22
     * }
     *
     * Valid Affects relationships:
     * (1, 2), (1, 3), (2, 3), (4, 7), (7, 7),
     * (11, 13), (12, 13), (20, 21)
     *
     * Valid Affects* relationships:
     * (1, 2), (1, 3)
     * (2, 3)
     * (4, 7)
     * (7, 7)
     * (11, 13)
     * (12, 13)
     * (20, 21)
     */

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graphOne = {
        {1, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {2}}}},
        {2, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {3}}}},
        {3, {{AppConstants::PARENTS, {2}}, {AppConstants::CHILDREN, {4}}}},
        {4, {{AppConstants::PARENTS, {3}}, {AppConstants::CHILDREN, {5}}}},
        {5, {{AppConstants::PARENTS, {4, 10}}, {AppConstants::CHILDREN, {6, 8}}}},
        {6, {{AppConstants::PARENTS, {4}}, {AppConstants::CHILDREN, {7}}}},
        {7, {{AppConstants::PARENTS, {6}}, {AppConstants::CHILDREN, {5}}}},
        {8, {{AppConstants::PARENTS, {5}}, {AppConstants::CHILDREN, {9}}}},
        {9, {{AppConstants::PARENTS, {8}}, {AppConstants::CHILDREN, {10}}}},
        {10, {{AppConstants::PARENTS, {9}}, {AppConstants::CHILDREN, {}}}}};

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graphTwo = {
        {11, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {12}}}},
        {12, {{AppConstants::PARENTS, {11}}, {AppConstants::CHILDREN, {13}}}},
        {13, {{AppConstants::PARENTS, {12}}, {AppConstants::CHILDREN, {14}}}},
        {14, {{AppConstants::PARENTS, {13}}, {AppConstants::CHILDREN, {15}}}},
        {15, {{AppConstants::PARENTS, {14}}, {AppConstants::CHILDREN, {16}}}},
        {16, {{AppConstants::PARENTS, {15}}, {AppConstants::CHILDREN, {17, 19}}}},
        {17, {{AppConstants::PARENTS, {16}}, {AppConstants::CHILDREN, {18}}}},
        {18, {{AppConstants::PARENTS, {17}}, {AppConstants::CHILDREN, {}}}},
        {19, {{AppConstants::PARENTS, {16}}, {AppConstants::CHILDREN, {}}}}};

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graphThree = {
        {20, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {21}}}},
        {21, {{AppConstants::PARENTS, {20}}, {AppConstants::CHILDREN, {22}}}},
        {22, {{AppConstants::PARENTS, {21}}, {AppConstants::CHILDREN, {}}}}};

    ProcName proc1 = "One";
    ProcName proc2 = "Two";
    ProcName proc3 = "Three";
    writePkb.writeCFG(proc1, graphOne);
    writePkb.writeCFG(proc2, graphTwo);
    writePkb.writeCFG(proc3, graphThree);

    writePkb.setStatement(AppConstants::ASSIGN, 1);
    writePkb.setStatement(AppConstants::ASSIGN, 2);
    writePkb.setStatement(AppConstants::ASSIGN, 3);
    writePkb.setStatement(AppConstants::ASSIGN, 4);
    writePkb.setStatement(AppConstants::WHILE, 5);
    writePkb.setStatement(AppConstants::PRINT, 6);
    writePkb.setStatement(AppConstants::ASSIGN, 7);
    writePkb.setStatement(AppConstants::CALL, 8);
    writePkb.setCall(8, proc2);
    writePkb.setStatement(AppConstants::CALL, 9);
    writePkb.setCall(9, proc3);
    writePkb.setStatement(AppConstants::CALL, 10);

    writePkb.setStatement(AppConstants::ASSIGN, 11);
    writePkb.setStatement(AppConstants::ASSIGN, 12);
    writePkb.setStatement(AppConstants::ASSIGN, 13);
    writePkb.setStatement(AppConstants::ASSIGN, 14);
    writePkb.setStatement(AppConstants::READ, 15);
    writePkb.setStatement(AppConstants::IF, 16);
    writePkb.setStatement(AppConstants::CALL, 17);
    writePkb.setCall(17, proc3);
    writePkb.setStatement(AppConstants::CALL, 18);
    writePkb.setStatement(AppConstants::ASSIGN, 19);

    writePkb.setStatement(AppConstants::ASSIGN, 20);
    writePkb.setStatement(AppConstants::ASSIGN, 21);
    writePkb.setStatement(AppConstants::CALL, 22);

    writePkb.setProcedure(proc1, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    writePkb.setProcedure(proc2, {11, 12, 13, 14, 15, 16, 17, 18, 19});
    writePkb.setProcedure(proc3, {20, 21, 22});

    writePkb.setModifiesS(1, {"x"});
    writePkb.setModifiesS(2, {"y"});
    writePkb.setModifiesS(3, {"z"});
    writePkb.setModifiesS(4, {"c"});
    writePkb.setModifiesS(5, {"c"});
    writePkb.setModifiesS(7, {"c"});
    writePkb.setModifiesS(8, {"y", "a", "b", "c", "d", "e"}); // call Two
    writePkb.setModifiesS(9, {"d", "e"});                     // call Three

    writePkb.setModifiesS(11, {"y"});
    writePkb.setModifiesS(12, {"a"});
    writePkb.setModifiesS(13, {"b"});
    writePkb.setModifiesS(14, {"c"});
    writePkb.setModifiesS(15, {"b"});
    writePkb.setModifiesS(16, {"d", "e", "a"});
    writePkb.setModifiesS(17, {"d", "e"}); // call Three
    writePkb.setModifiesS(19, {"a"});

    writePkb.setModifiesS(20, {"d"});
    writePkb.setModifiesS(21, {"e"});

    writePkb.setModifiesP(proc1, {"x", "y", "z", "a", "b", "c", "d", "e"});
    writePkb.setModifiesP(proc2, {"y", "a", "b", "c", "d", "e"});
    writePkb.setModifiesP(proc3, {"d", "e"});

    writePkb.setUsesS(1, {"x"});
    writePkb.setUsesS(2, {"y", "x"});
    writePkb.setUsesS(3, {"x", "y", "c"});
    writePkb.setUsesS(5, {"x", "c", "a", "y", "z", "b", "d"}); // additional stuff inside
    writePkb.setUsesS(6, {"c"});
    writePkb.setUsesS(7, {"c"});
    writePkb.setUsesS(8, {"x", "c", "a", "y", "z", "b", "c", "d"}); // call Two
    writePkb.setUsesS(9, {"d"});                                    // call Three

    writePkb.setUsesS(11, {"x"});
    writePkb.setUsesS(12, {"c"});
    writePkb.setUsesS(13, {"a", "y", "x", "z"});
    writePkb.setUsesS(16, {"b", "c", "d"}); // additional stuff inside
    writePkb.setUsesS(17, {"d"});           // call Three
    writePkb.setUsesS(19, {"b"});

    writePkb.setUsesS(21, {"d"});

    writePkb.setUsesP(proc1, {"x", "y", "c", "a", "y", "z", "b", "d"});
    writePkb.setUsesP(proc2, {"x", "c", "a", "y", "z", "b", "d"});
    writePkb.setUsesP(proc3, {"d"});

    SECTION("Affects(int, int)") {
        std::vector<Parameter> params1 = {Parameter("7", ParameterType::FIXED_INT),
                                          Parameter("7", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"7", "7"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }

    SECTION("Affects(int, _)") {
        std::vector<Parameter> params1 = {Parameter("11", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"11", "13"}};
        REQUIRE(expected1 == res1);
    }

    SECTION("Affects(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"1", "2"}, {"1", "3"},   {"2", "3"},   {"7", "7"},
                                                           {"4", "7"}, {"11", "13"}, {"12", "13"}, {"20", "21"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }

    SECTION("Affects*(a, int)") {
        std::vector<Parameter> params1 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("3", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"1", "3"}, {"2", "3"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }

    SECTION("Affects*(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"1", "2"}, {"1", "3"},   {"2", "3"},   {"7", "7"},
                                                           {"4", "7"}, {"11", "13"}, {"12", "13"}, {"20", "21"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }
}

// This test case's expected results are wrong
TEST_CASE("findRelationship(shared_ptr<Relationship> rs): Affects & Affects* 3") {
    /*
     *
     * while (v != 1) { //1
     *     d = 1; //2
     *     while (d != 1) { //3
     *         while (e != 1) { //4
     *              a = d + e; //5
     *         }
     *         b = a + c; //6
     *         d = b; //7
     *     }
     *     b = d; //8
     * }
     *
     * Valid Affects relationships:
     * (2, 5), (2, 8), (5, 6), (6, 7), (7, 5), (7, 8)
     *
     * Valid Affects* relationships:
     * (2, 5), (2, 8), (2, 6), (2, 7)
     * (5, 6), (5, 7), (5, 8), (5, 5)
     * (6, 7), (6, 8), (6, 5), (6, 6)
     * (7, 8), (7, 5), (7, 6), (7, 7)
     *
     */

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graphOne = {
        {1, {{AppConstants::PARENTS, {8}}, {AppConstants::CHILDREN, {2}}}},
        {2, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {3}}}},
        {3, {{AppConstants::PARENTS, {2, 7}}, {AppConstants::CHILDREN, {4, 8}}}},
        {4, {{AppConstants::PARENTS, {3, 5}}, {AppConstants::CHILDREN, {5, 6}}}},
        {5, {{AppConstants::PARENTS, {4}}, {AppConstants::CHILDREN, {4}}}},
        {6, {{AppConstants::PARENTS, {4}}, {AppConstants::CHILDREN, {7}}}},
        {7, {{AppConstants::PARENTS, {6}}, {AppConstants::CHILDREN, {3}}}},
        {8, {{AppConstants::PARENTS, {3}}, {AppConstants::CHILDREN, {1}}}}};

    ProcName proc1 = "One";
    writePkb.writeCFG(proc1, graphOne);

    writePkb.setStatement(AppConstants::WHILE, 1);
    writePkb.setStatement(AppConstants::ASSIGN, 2);
    writePkb.setStatement(AppConstants::WHILE, 3);
    writePkb.setStatement(AppConstants::WHILE, 4);
    writePkb.setStatement(AppConstants::ASSIGN, 5);
    writePkb.setStatement(AppConstants::ASSIGN, 6);
    writePkb.setStatement(AppConstants::ASSIGN, 7);
    writePkb.setStatement(AppConstants::ASSIGN, 8);

    writePkb.setProcedure(proc1, {1, 2, 3, 4, 5, 6, 7, 8});

    writePkb.setModifiesS(1, {"b", "d", "a"});
    writePkb.setModifiesS(2, {"d"});
    writePkb.setModifiesS(3, {"b", "d", "a"});
    writePkb.setModifiesS(4, {"a"});
    writePkb.setModifiesS(5, {"a"});
    writePkb.setModifiesS(6, {"b"});
    writePkb.setModifiesS(7, {"d"});
    writePkb.setModifiesS(8, {"b"});

    writePkb.setModifiesP(proc1, {"d", "a", "b"});

    writePkb.setUsesS(1, {"v", "d", "a", "c", "b", "d", "e"});
    writePkb.setUsesS(3, {"d", "a", "c", "b", "d", "e"});
    writePkb.setUsesS(4, {"e", "d"});
    writePkb.setUsesS(5, {"d", "e"});
    writePkb.setUsesS(6, {"a", "c"});
    writePkb.setUsesS(7, {"b"});
    writePkb.setUsesS(8, {"d"});

    writePkb.setUsesP(proc1, {"v", "d", "a", "c", "b", "d", "e"});

    SECTION("Affects(int, int)") {
        std::vector<Parameter> params1 = {Parameter("2", ParameterType::FIXED_INT),
                                          Parameter("5", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"2", "5"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("5", ParameterType::FIXED_INT),
                                          Parameter("2", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(unit_testing_utils::equals(expected2, res2));
    }

    SECTION("Affects(int, _)") {
        std::vector<Parameter> params1 = {Parameter("7", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"7", "8"}, {"7", "5"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("3", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(unit_testing_utils::equals(expected2, res2));
    }

    SECTION("Affects(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected = {{"2", "5"}, {"2", "8"}, {"5", "6"},
                                                          {"6", "7"}, {"7", "5"}, {"7", "8"}};
        REQUIRE(unit_testing_utils::equals(expected, res1));

        std::vector<Parameter> params2 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected, res2));

        std::vector<Parameter> params3 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals(expected, res3));

        std::vector<Parameter> params4 = {Parameter("a", ParameterType::ASSIGN), Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        REQUIRE(res4.empty());
    }

    SECTION("Affects*(a, int)") {
        std::vector<Parameter> params1 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("8", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"2", "8"}, {"7", "8"}, {"6", "8"}, {"5", "8"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("2", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(expected2 == res2);
    }

    SECTION("Affects*(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {
            {"2", "5"}, {"2", "6"}, {"2", "7"}, {"2", "8"}, {"5", "6"}, {"5", "7"}, {"5", "8"}, {"5", "5"},
            {"6", "7"}, {"6", "8"}, {"6", "5"}, {"6", "6"}, {"7", "8"}, {"7", "7"}, {"7", "5"}, {"7", "6"},

        };
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }
}

TEST_CASE("findRelationship(shared_ptr<Relationship> rs): Affects & Affects* Template 4") {
    /*
     *
     * a = 1; //1
     * if (a != b) then { //2
     *     c = 1; //3
     *     if (c != b) then { //4
     *         b = 1; //5
     *     } else {
     *         f = 1 + a + b; //6
     *         e = 2; //7
     *     }
     *     a = a + d + f; //8
     * } else {
     *     e = b + c + e; //9
     * }
     * e = b; //10
     * v = e; //11
     * g = a; //12
     *
     *
     *
     * Valid Affects relationships:
     * (1, 6), (1, 8) [issue], (1, 12) [issue?]
     * (5, 10)
     * (6, 8)
     * (8, 12)
     * (10, 11)
     *
     * Valid Affects* relationships:
     *
     *
     */

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graphOne = {
        {1, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {2}}}},
        {2, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {3, 9}}}},
        {3, {{AppConstants::PARENTS, {2}}, {AppConstants::CHILDREN, {4}}}},
        {4, {{AppConstants::PARENTS, {3}}, {AppConstants::CHILDREN, {5, 6}}}},
        {5, {{AppConstants::PARENTS, {4}}, {AppConstants::CHILDREN, {8}}}},
        {6, {{AppConstants::PARENTS, {4}}, {AppConstants::CHILDREN, {7}}}},
        {7, {{AppConstants::PARENTS, {6}}, {AppConstants::CHILDREN, {8}}}},
        {8, {{AppConstants::PARENTS, {5, 7}}, {AppConstants::CHILDREN, {10}}}},
        {9, {{AppConstants::PARENTS, {2}}, {AppConstants::CHILDREN, {10}}}},
        {10, {{AppConstants::PARENTS, {8, 9}}, {AppConstants::CHILDREN, {11}}}},
        {11, {{AppConstants::PARENTS, {10}}, {AppConstants::CHILDREN, {12}}}},
        {12, {{AppConstants::PARENTS, {11}}, {AppConstants::CHILDREN, {}}}}};

    ProcName proc1 = "One";
    writePkb.writeCFG(proc1, graphOne);

    writePkb.setStatement(AppConstants::ASSIGN, 1);
    writePkb.setStatement(AppConstants::IF, 2);
    writePkb.setStatement(AppConstants::ASSIGN, 3);
    writePkb.setStatement(AppConstants::IF, 4);
    writePkb.setStatement(AppConstants::ASSIGN, 5);
    writePkb.setStatement(AppConstants::ASSIGN, 6);
    writePkb.setStatement(AppConstants::ASSIGN, 7);
    writePkb.setStatement(AppConstants::ASSIGN, 8);
    writePkb.setStatement(AppConstants::ASSIGN, 9);
    writePkb.setStatement(AppConstants::ASSIGN, 10);
    writePkb.setStatement(AppConstants::ASSIGN, 11);
    writePkb.setStatement(AppConstants::ASSIGN, 12);

    writePkb.setProcedure(proc1, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});

    writePkb.setModifiesS(1, {"a"});
    writePkb.setModifiesS(3, {"c"});
    writePkb.setModifiesS(5, {"b"});
    writePkb.setModifiesS(6, {"f"});
    writePkb.setModifiesS(7, {"e"});
    writePkb.setModifiesS(8, {"a"});
    writePkb.setModifiesS(9, {"e"});
    writePkb.setModifiesS(10, {"e"});
    writePkb.setModifiesS(11, {"v"});
    writePkb.setModifiesS(12, {"g"});
    writePkb.setModifiesP(proc1, {"a", "c", "b", "f", "e", "v", "g"});

    writePkb.setUsesS(6, {"a", "b"});
    writePkb.setUsesS(8, {"a", "d", "f"});
    writePkb.setUsesS(9, {"b", "c", "e"});
    writePkb.setUsesS(10, {"b"});
    writePkb.setUsesS(11, {"e"});
    writePkb.setUsesS(12, {"a"});
    writePkb.setUsesP(proc1, {"a", "b", "c", "d", "e", "f"});

    SECTION("Affects(int, int)") {
        std::vector<Parameter> params1 = {Parameter("1", ParameterType::FIXED_INT),
                                          Parameter("6", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"1", "6"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }

    SECTION("Affects(int, _)") {
        std::vector<Parameter> params1 = {Parameter("8", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"8", "12"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("3", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(unit_testing_utils::equals(expected2, res2));
    }

    SECTION("Affects(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected = {{"1", "6"}, {"1", "8"},  {"1", "12"}, {"5", "10"},
                                                          {"6", "8"}, {"8", "12"}, {"10", "11"}};
        REQUIRE(unit_testing_utils::equals(expected, res1));

        std::vector<Parameter> params2 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected, res2));

        std::vector<Parameter> params3 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals(expected, res3));

        std::vector<Parameter> params4 = {Parameter("a", ParameterType::ASSIGN), Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        REQUIRE(res4.empty());
    }

    SECTION("Affects*(a, int)") {
        std::vector<Parameter> params1 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("8", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"1", "8"}, {"6", "8"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("3", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(expected2 == res2);
    }

    SECTION("Affects*(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"1", "6"},  {"1", "8"}, {"1", "12"}, {"5", "10"},
                                                           {"5", "11"}, {"6", "8"}, {"6", "12"}, {"8", "12"},
                                                           {"10", "11"}

        };
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }
}

TEST_CASE("findRelationship(shared_ptr<Relationship> rs): Affects & Affects* 5") {
    /*
     *
     * proc One {
     *     while (a b c) { //1
     *         if (b u n v m n x h f) then { //2
     *             c = a + b; //3
     *             call D;  //4 [modifies and uses c is true]
     *         } else {
     *             if (c) then { //5
     *                 call D;  //6 [modifies and uses c is true]
     *             } else {
     *                 c = b + c; //7
     *             }
     *         }
     *         d = d + e; //8
     *         print g; //9
     *         call G; //10, [modifies and uses c is true]
     *         read f; //11
     *     }
     * }
     *
     *
     * Valid Affects relationships:
     * (8, 8)
     *
     * Valid Affects* relationships:
     * (8, 8)
     *
     */

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graphOne = {
        {1, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {2}}}},
        {2, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {3, 5}}}},
        {3, {{AppConstants::PARENTS, {2}}, {AppConstants::CHILDREN, {4}}}},
        {4, {{AppConstants::PARENTS, {3}}, {AppConstants::CHILDREN, {8}}}},
        {5, {{AppConstants::PARENTS, {2}}, {AppConstants::CHILDREN, {6, 7}}}},
        {6, {{AppConstants::PARENTS, {5}}, {AppConstants::CHILDREN, {8}}}},
        {7, {{AppConstants::PARENTS, {5}}, {AppConstants::CHILDREN, {8}}}},
        {8, {{AppConstants::PARENTS, {4, 5, 7}}, {AppConstants::CHILDREN, {9}}}},
        {9, {{AppConstants::PARENTS, {8}}, {AppConstants::CHILDREN, {10}}}},
        {10, {{AppConstants::PARENTS, {9}}, {AppConstants::CHILDREN, {11}}}},
        {11, {{AppConstants::PARENTS, {10}}, {AppConstants::CHILDREN, {1}}}}};

    ProcName proc1 = "One";
    writePkb.writeCFG(proc1, graphOne);

    writePkb.setStatement(AppConstants::WHILE, 1);
    writePkb.setStatement(AppConstants::IF, 2);
    writePkb.setStatement(AppConstants::ASSIGN, 3);
    writePkb.setStatement(AppConstants::CALL, 4);
    writePkb.setStatement(AppConstants::IF, 5);
    writePkb.setStatement(AppConstants::CALL, 6);
    writePkb.setStatement(AppConstants::ASSIGN, 7);
    writePkb.setStatement(AppConstants::ASSIGN, 8);
    writePkb.setStatement(AppConstants::PRINT, 9);
    writePkb.setStatement(AppConstants::CALL, 10);
    writePkb.setStatement(AppConstants::READ, 11);

    writePkb.setProcedure(proc1, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});

    writePkb.setModifiesS(1, {"c", "d", "f"});
    writePkb.setModifiesS(2, {"c"});
    writePkb.setModifiesS(3, {"c"});
    writePkb.setModifiesS(4, {"c"});
    writePkb.setModifiesS(5, {"c"});
    writePkb.setModifiesS(6, {"c"});
    writePkb.setModifiesS(7, {"c"});
    writePkb.setModifiesS(8, {"d"});
    writePkb.setModifiesS(9, {});
    writePkb.setModifiesS(10, {"c"});
    writePkb.setModifiesS(11, {"f"});
    writePkb.setModifiesP(proc1, {"c", "d", "f"});

    writePkb.setUsesS(1, {"a", "b", "c"});
    writePkb.setUsesS(2, {"a", "b", "c"});
    writePkb.setUsesS(3, {"a", "b"});
    writePkb.setUsesS(4, {"c"});
    writePkb.setUsesS(5, {"b", "c"});
    writePkb.setUsesS(6, {"c"});
    writePkb.setUsesS(7, {""});
    writePkb.setUsesS(8, {"d", "e"});
    writePkb.setUsesS(9, {"g"});
    writePkb.setUsesS(10, {"c"});
    writePkb.setUsesP(proc1, {"a", "b", "c", "d", "e", "g"});

    writePkb.setParentT(1, {2, 3, 4, 5, 6, 7, 8, 9, 10, 11});

    SECTION("Affects(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected = {{"8", "8"}};
        REQUIRE(unit_testing_utils::equals(expected, res1));

        std::vector<Parameter> params2 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected, res2));

        std::vector<Parameter> params3 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals(expected, res3));

        std::vector<Parameter> params4 = {Parameter("a", ParameterType::ASSIGN), Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        REQUIRE(unit_testing_utils::equals(expected, res4));
    }

    SECTION("Affects*(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"8", "8"}

        };
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }
}

TEST_CASE("findRelationship(shared_ptr<Relationship> rs): Affects & Affects* systest2 model") {
    /*
     *
     * procedure wEiRdNAmeS123 {
     * 	read p0licemAN;	// 1
     * 	read p0liceWahMan;	// 2
     * 	read pOl3icEwOM5N;	// 3
     * 	if (p0licemAN == p0liceWahMan) then {	// 4
     * 		read read;	// 5
     * 		read = read + 1;	// 6
     * 		read rEaD;	// 7
     * 		rEEE = rEaD + read;	// 8
     * 	} else {
     * 		call noindent;	// 9
     * 		print print;	// 10
     * 	}
     * 	call call;	// 11
     * 	m0ArWeirdNaMe5AnDtH7sIsVer9Lon6AnDILov3Ch3rr1E2=8;	// 12
     * }
     *
     * procedure noindent {
     * 	call call;	// 13
     * 	call call;	// 14
     * 	call call;	// 15
     * 	i = 5;	// 16
     * 	N = 50;	// 17
     * 	while (i <= N) {	// 18
     * 		call call;	// 19
     * 		i = i + 1;	// 20
     * 		print i;	// 21
     * 	}
     * 	x = 1+2;	// 22
     * 	y = 2+3;	// 23
     * 	if (x != y) then {	// 24
     * 		x = y;	// 25
     * 	} else {
     * 		y = x;	// 26
     * 	}
     * 	print x;	// 27
     * 	print y;	// 28
     * }
     *
     * procedure call {
     * 	a = 2 + 3;	// 29
     * }
     *
     * Valid Affects relationships:
     * (16, 20), (20, 20), (22, 26), (23, 25)
     *
     * Valid Affects* relationships:
     * (16, 20), (20, 20), (22, 26), (23, 25)
     *
     */

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graphOne = {
        {1, {{AppConstants::PARENTS, {8}}, {AppConstants::CHILDREN, {2}}}},
        {2, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {3}}}},
        {3, {{AppConstants::PARENTS, {2, 7}}, {AppConstants::CHILDREN, {4}}}},
        {4, {{AppConstants::PARENTS, {3, 5}}, {AppConstants::CHILDREN, {5, 9}}}},
        {5, {{AppConstants::PARENTS, {4}}, {AppConstants::CHILDREN, {6}}}},
        {6, {{AppConstants::PARENTS, {5}}, {AppConstants::CHILDREN, {7}}}},
        {7, {{AppConstants::PARENTS, {6}}, {AppConstants::CHILDREN, {8}}}},
        {8, {{AppConstants::PARENTS, {7}}, {AppConstants::CHILDREN, {11}}}},
        {9, {{AppConstants::PARENTS, {4}}, {AppConstants::CHILDREN, {10}}}},
        {10, {{AppConstants::PARENTS, {9}}, {AppConstants::CHILDREN, {11}}}},
        {11, {{AppConstants::PARENTS, {8, 10}}, {AppConstants::CHILDREN, {12}}}},
        {12, {{AppConstants::PARENTS, {11}}, {AppConstants::CHILDREN, {}}}}};

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graphTwo = {
        {13, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {14}}}},
        {14, {{AppConstants::PARENTS, {13}}, {AppConstants::CHILDREN, {15}}}},
        {15, {{AppConstants::PARENTS, {14}}, {AppConstants::CHILDREN, {16}}}},
        {16, {{AppConstants::PARENTS, {15}}, {AppConstants::CHILDREN, {17}}}},
        {17, {{AppConstants::PARENTS, {16}}, {AppConstants::CHILDREN, {18}}}},
        {18, {{AppConstants::PARENTS, {17, 21}}, {AppConstants::CHILDREN, {19}}}},
        {19, {{AppConstants::PARENTS, {18}}, {AppConstants::CHILDREN, {20}}}},
        {20, {{AppConstants::PARENTS, {19}}, {AppConstants::CHILDREN, {21}}}},
        {21, {{AppConstants::PARENTS, {20}}, {AppConstants::CHILDREN, {18}}}},
        {22, {{AppConstants::PARENTS, {18}}, {AppConstants::CHILDREN, {23}}}},
        {23, {{AppConstants::PARENTS, {22}}, {AppConstants::CHILDREN, {24}}}},
        {24, {{AppConstants::PARENTS, {23}}, {AppConstants::CHILDREN, {25, 26}}}},
        {25, {{AppConstants::PARENTS, {24}}, {AppConstants::CHILDREN, {27}}}},
        {26, {{AppConstants::PARENTS, {24}}, {AppConstants::CHILDREN, {27}}}},
        {27, {{AppConstants::PARENTS, {25, 26}}, {AppConstants::CHILDREN, {28}}}},
        {28, {{AppConstants::PARENTS, {27}}, {AppConstants::CHILDREN, {}}}}};

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graphThree = {
        {29, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {}}}}};

    ProcName proc1 = "wEiRdNAmeS123";
    writePkb.writeCFG(proc1, graphOne);
    ProcName proc2 = "noindent";
    writePkb.writeCFG(proc2, graphTwo);
    ProcName proc3 = AppConstants::CALL;
    writePkb.writeCFG(proc3, graphThree);

    writePkb.setStatement(AppConstants::READ, 1);
    writePkb.setStatement(AppConstants::READ, 2);
    writePkb.setStatement(AppConstants::READ, 3);
    writePkb.setStatement(AppConstants::IF, 4);
    writePkb.setStatement(AppConstants::READ, 5);
    writePkb.setStatement(AppConstants::READ, 6);
    writePkb.setStatement(AppConstants::READ, 7);
    writePkb.setStatement(AppConstants::ASSIGN, 8);
    writePkb.setStatement(AppConstants::CALL, 9);
    writePkb.setStatement(AppConstants::PRINT, 10);
    writePkb.setStatement(AppConstants::CALL, 11);
    writePkb.setStatement(AppConstants::ASSIGN, 12);

    writePkb.setStatement(AppConstants::CALL, 13);
    writePkb.setStatement(AppConstants::CALL, 14);
    writePkb.setStatement(AppConstants::CALL, 15);
    writePkb.setStatement(AppConstants::ASSIGN, 16);
    writePkb.setStatement(AppConstants::ASSIGN, 17);
    writePkb.setStatement(AppConstants::WHILE, 18);
    writePkb.setStatement(AppConstants::CALL, 19);
    writePkb.setStatement(AppConstants::ASSIGN, 20);
    writePkb.setStatement(AppConstants::PRINT, 21);
    writePkb.setStatement(AppConstants::ASSIGN, 22);
    writePkb.setStatement(AppConstants::ASSIGN, 23);
    writePkb.setStatement(AppConstants::IF, 24);
    writePkb.setStatement(AppConstants::ASSIGN, 25);
    writePkb.setStatement(AppConstants::ASSIGN, 26);
    writePkb.setStatement(AppConstants::PRINT, 27);
    writePkb.setStatement(AppConstants::PRINT, 28);

    writePkb.setStatement(AppConstants::ASSIGN, 29);

    writePkb.setProcedure(proc1, {1, 2, 3, 4, 5, 6, 7, 8, 8, 10, 11, 12});
    writePkb.setProcedure(proc2, {113, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28});
    writePkb.setProcedure(proc3, {29});

    writePkb.setModifiesS(1, {"p0licemAN"});
    writePkb.setModifiesS(2, {"p0liceWahMan"});
    writePkb.setModifiesS(3, {"pOl3icEwOM5N"});
    writePkb.setModifiesS(4, {"a", "i", "N", "x", "y", "read", "rEaD", "rEEE"});
    writePkb.setModifiesS(5, {"read"});
    writePkb.setModifiesS(6, {"read"});
    writePkb.setModifiesS(7, {"rEaD"});
    writePkb.setModifiesS(8, {"rEEE"});
    writePkb.setModifiesS(9, {"a", "i", "N", "x", "y"});
    //    writePkb.setModifiesS(10, {});
    writePkb.setModifiesS(11, {"a"});
    writePkb.setModifiesS(12, {"m0ArWeirdNaMe5AnDtH7sIsVer9Lon6AnDILov3Ch3rr1E2"});
    writePkb.setModifiesP(
        proc1, {"p0licemAN", "p0liceWahMan", "pOl3icEwOM5N", "a", "i", "N", "x", "y", "read", "rEaD", "rEEE"});

    writePkb.setModifiesS(13, {"a"});
    writePkb.setModifiesS(14, {"a"});
    writePkb.setModifiesS(15, {"a"});
    writePkb.setModifiesS(16, {"i"});
    writePkb.setModifiesS(17, {"N"});
    writePkb.setModifiesS(18, {"a", "i"});
    writePkb.setModifiesS(19, {"a"});
    writePkb.setModifiesS(20, {"i"});
    writePkb.setModifiesS(21, {"i"});
    writePkb.setModifiesS(22, {"x"});
    writePkb.setModifiesS(23, {"y"});
    writePkb.setModifiesS(24, {"x", "y"});
    writePkb.setModifiesS(25, {"x"});
    writePkb.setModifiesS(26, {"y"});
    //    writePkb.setModifiesS(27, {});
    //    writePkb.setModifiesS(28, {});
    writePkb.setModifiesP(proc2, {"a", "i", "N", "x", "y"});

    writePkb.setModifiesS(29, {"a"});
    writePkb.setModifiesP(proc3, {"a"});

    //    writePkb.setUsesS(1, {});
    //    writePkb.setUsesS(2, {});
    //    writePkb.setUsesS(3, {});
    writePkb.setUsesS(4, {"p0licemAN", "p0liceWahMan", "rEaD", "read", "i", "N", "x", "y", AppConstants::PRINT});
    //    writePkb.setUsesS(5, {});
    //    writePkb.setUsesS(6, {});
    //    writePkb.setUsesS(7, {});
    writePkb.setUsesS(8, {"rEaD", "read"}); // rEeeeaD
    writePkb.setUsesS(9, {"i", "N", "x", "y"});
    writePkb.setUsesS(10, {AppConstants::PRINT});
    //    writePkb.setUsesS(11, {});
    //    writePkb.setUsesS(12, {});
    writePkb.setUsesP(proc1, {"i", "N", "x", "y", "p0licemAN", "p0liceWahMan", "rEaD", "read", "i", "N", "x", "y",
                              AppConstants::PRINT});

    //    writePkb.setUsesS(13, {});
    //    writePkb.setUsesS(14, {});
    //    writePkb.setUsesS(15, {});
    //    writePkb.setUsesS(16, {});
    //    writePkb.setUsesS(17, {});
    writePkb.setUsesS(18, {"i", "N"});
    //    writePkb.setUsesS(19, {});
    writePkb.setUsesS(20, {"i"});
    writePkb.setUsesS(21, {"i"});
    //    writePkb.setUsesS(22, {});
    //    writePkb.setUsesS(23, {});
    writePkb.setUsesS(24, {"x", "y"});
    writePkb.setUsesS(25, {"y"});
    writePkb.setUsesS(26, {"x"});
    writePkb.setUsesS(27, {"x"});
    writePkb.setUsesS(28, {"y"});
    writePkb.setUsesP(proc2, {"i", "N", "x", "y"});

    //    writePkb.setUsesS(29, {});
    //    writePkb.setUsesP(proc3, {});

    writePkb.setParentT(4, {5, 6, 7, 8, 9, 10});
    writePkb.setParentT(18, {19, 20, 21});
    writePkb.setParentT(24, {25, 26});

    SECTION("Affects(int, int)") {
        std::vector<Parameter> params1 = {Parameter("3", ParameterType::FIXED_INT),
                                          Parameter("7", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {};
        REQUIRE(expected1 == res1);

        std::vector<Parameter> params2 = {Parameter("23", ParameterType::FIXED_INT),
                                          Parameter("25", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"23", "25"}};
        REQUIRE(expected2 == res2);

        std::vector<Parameter> params3 = {Parameter("20", ParameterType::FIXED_INT),
                                          Parameter("20", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {{"20", "20"}};
        REQUIRE(expected3 == res3);
    }

    SECTION("Affects(int, _)") {
        std::vector<Parameter> params1 = {Parameter("22", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"22", "26"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("29", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(unit_testing_utils::equals(expected2, res2));
    }

    SECTION("Affects(_, int)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("20", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"16", "20"}, {"20", "20"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));
    }

    SECTION("Affects(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"16", "20"}, {"20", "20"}, {"22", "26"}, {"23", "25"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected1, res2));

        std::vector<Parameter> params3 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals(expected1, res3));

        std::vector<Parameter> params4 = {Parameter("a", ParameterType::ASSIGN), Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {{"20", "20"}};
        REQUIRE(unit_testing_utils::equals(expected4, res4));
    }

    SECTION("Affects*(int, a)") {
        std::vector<Parameter> params1 = {Parameter("16", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"16", "20"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("16", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected1, res2));

        std::vector<Parameter> params4 = {Parameter("24", ParameterType::FIXED_INT), // not an assign statement
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTST, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(unit_testing_utils::equals(expected4, res4));
    }

    SECTION("Affects*(a, int)") {
        std::vector<Parameter> params1 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("20", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"16", "20"}, {"20", "20"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("20", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected1, res2));

        std::vector<Parameter> params3 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("18", ParameterType::FIXED_INT)}; // not an assign statement
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTST, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {};
        REQUIRE(unit_testing_utils::equals(expected3, res3));

        std::vector<Parameter> params4 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("100", ParameterType::FIXED_INT)}; // not in source code
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTST, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(unit_testing_utils::equals(expected4, res4));
    }

    SECTION("Affects*(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"16", "20"}, {"20", "20"}, {"22", "26"}, {"23", "25"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected1, res2));

        std::vector<Parameter> params3 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTST, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals(expected1, res3));

        std::vector<Parameter> params4 = {Parameter("a", ParameterType::ASSIGN), Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTST, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {{"20", "20"}};
        REQUIRE(unit_testing_utils::equals(expected4, res4));
    }
}