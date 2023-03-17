#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

TEST_CASE("findRelationship(shared_ptr<Relationship> rs): Affects & Affects*") {
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
        {24, {{AppConstants::PARENTS, {23}}, {AppConstants::CHILDREN, {26}}}},
        {25, {{AppConstants::PARENTS, {23}}, {AppConstants::CHILDREN, {26}}}},
        {26, {{AppConstants::PARENTS, {24, 25}}, {AppConstants::CHILDREN, {27}}}},
        {27, {{AppConstants::PARENTS, {26}}, {AppConstants::CHILDREN, {}}}}};

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph2 = {
        {28, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {}}}}};
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
     *     call proc2; //16, modifies c
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
     * (2, 5), (3, 6), (6, 9),
     * (7, 8), (8, 2), (9, 3),
     * (10, 11), (11, 18) (13, 12),
     * (14, 14), (19, 19), (19, 20)
     * (20, 26)
     *
     * Valid Affects* relationships:
     * (2, 5)
     * (3, 6), (3, 9), (3, 3),
     * (6, 9), (6, 3), (6, 6)
     * (7, 8), (7, 2), (7, 5)
     * (8, 2), (8, 5)
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

    writePkb.setStatement("while", 1);
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("if", 4);
    writePkb.setStatement("assign", 5);
    writePkb.setStatement("assign", 6);
    writePkb.setStatement("assign", 7);
    writePkb.setStatement("assign", 8);
    writePkb.setStatement("assign", 9);
    writePkb.setStatement("assign", 10);
    writePkb.setStatement("assign", 11);
    writePkb.setStatement("assign", 12);
    writePkb.setStatement("assign", 13);
    writePkb.setStatement("assign", 14);
    writePkb.setStatement("read", 15);
    writePkb.setStatement("call", 16);
    writePkb.setCall(16, proc2);
    writePkb.setStatement("print", 17);
    writePkb.setStatement("assign", 18);
    writePkb.setStatement("assign", 19);
    writePkb.setStatement("assign", 20);
    writePkb.setStatement("call", 21);
    writePkb.setCall(21, proc2);
    writePkb.setStatement("assign", 22);
    writePkb.setStatement("if", 23);
    writePkb.setStatement("assign", 24);
    writePkb.setStatement("assign", 25);
    writePkb.setStatement("assign", 26);
    writePkb.setStatement("print", 27);
    writePkb.setStatement("print", 28);

    writePkb.setProcedure(
        proc1, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27});
    writePkb.setStatement("assign", 28);
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
    writePkb.setModifiesS(16, {"c"});
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
        std::vector<Parameter> params1 = {Parameter("6", AppConstants::FIXED_INT),
                                          Parameter("9", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"6", "9"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("7", AppConstants::FIXED_INT),
                                          Parameter("8", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"7", "8"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("8", AppConstants::FIXED_INT),
                                          Parameter("8", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(res3.empty());

        std::vector<Parameter> params4 = {Parameter("22", AppConstants::FIXED_INT),
                                          Parameter("22", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(expected4 == res4);

        std::vector<Parameter> params5 = {Parameter("24", AppConstants::FIXED_INT),
                                          Parameter("25", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs5 = Relationship::makeRelationship(AppConstants::AFFECTS, params5);
        std::vector<std::vector<std::string>> res5 = readPkb.findRelationship(rs5);
        std::vector<std::vector<std::string>> expected5 = {};
        REQUIRE(expected5 == res5);
    }

    SECTION("Affects(int, _)") {
        std::vector<Parameter> params1 = {Parameter("19", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"19", "19"}, {"19", "20"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("1", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(expected2 == res2);

        std::vector<Parameter> params3 = {Parameter("15", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {};
        REQUIRE(expected3 == res3);

        std::vector<Parameter> params4 = {Parameter("4", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(expected4 == res4);

        std::vector<Parameter> params5 = {Parameter("21", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs5 = Relationship::makeRelationship(AppConstants::AFFECTS, params5);
        std::vector<std::vector<std::string>> res5 = readPkb.findRelationship(rs5);
        std::vector<std::vector<std::string>> expected5 = {};
        REQUIRE(expected5 == res5);
    }

    SECTION("Affects(_, int)") {
        std::vector<Parameter> params1 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("3", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"9", "3"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("1", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("4", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {};
        REQUIRE(unit_testing_utils::equals(expected3, res3));

        std::vector<Parameter> params4 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("16", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(unit_testing_utils::equals(expected4, res4));
    }

    SECTION("Affects(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"2", "5"},   {"3", "6"},   {"6", "9"},   {"7", "8"},
                                                           {"8", "2"},   {"9", "3"},   {"10", "11"}, {"11", "18"},
                                                           {"13", "12"}, {"14", "14"}, {"19", "20"}, {"19", "19"}};
        //        REQUIRE(expected1 == res1);
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("aa", AppConstants::ASSIGN),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected1, res2));

        std::vector<Parameter> params3 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("as", AppConstants::ASSIGN)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals(expected1, res3));

        std::vector<Parameter> params4 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        REQUIRE(unit_testing_utils::equals(expected1, res4));
    }

    SECTION("Affects*(int, int)") {
        std::vector<Parameter> params1 = {Parameter("19", AppConstants::FIXED_INT),
                                          Parameter("20", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"19", "20"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("7", AppConstants::FIXED_INT),
                                          Parameter("5", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"7", "5"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("8", AppConstants::FIXED_INT),
                                          Parameter("8", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTST, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(res3.empty());
    }

    SECTION("Affects*(int, _)") {
        std::vector<Parameter> params1 = {Parameter("9", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"9", "3"}, {"9", "6"}, {"9", "9"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("1", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(expected2 == res2);

        std::vector<Parameter> params3 = {Parameter("15", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTST, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {};
        REQUIRE(expected3 == res3);

        std::vector<Parameter> params4 = {Parameter("4", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTST, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(expected4 == res4);
    }

    SECTION("Affects*(_, int)") {
        std::vector<Parameter> params1 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("6", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"3", "6"}, {"6", "6"}, {"9", "6"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("1", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("4", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTST, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {};
        REQUIRE(unit_testing_utils::equals(expected3, res3));

        std::vector<Parameter> params4 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("16", AppConstants::FIXED_INT)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTST, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(unit_testing_utils::equals(expected4, res4));
    }

    SECTION("Affects*(_, _)") {
        std::vector<Parameter> params1 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTST, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {
            {"2", "5"},   {"3", "6"},   {"3", "9"},   {"3", "3"},   {"6", "9"},   {"6", "3"},  {"6", "6"}, {"7", "8"},
            {"7", "2"},   {"7", "5"},   {"8", "2"},   {"8", "5"},   {"9", "3"},   {"9", "6"},  {"9", "9"}, {"10", "11"},
            {"10", "18"}, {"11", "18"}, {"13", "12"}, {"14", "14"}, {"19", "19"}, {"19", "20"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("aa", AppConstants::ASSIGN),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(unit_testing_utils::equals(expected1, res2));

        std::vector<Parameter> params3 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("as", AppConstants::ASSIGN)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTST, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals(expected1, res3));

        std::vector<Parameter> params4 = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("_", AppConstants::WILDCARD)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTST, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        REQUIRE(unit_testing_utils::equals(expected1, res4));
    }
}
