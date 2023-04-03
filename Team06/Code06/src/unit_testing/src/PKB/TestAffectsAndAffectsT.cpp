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

    writePkb.setProcAssignStmt(proc1, 2);
    writePkb.setProcAssignStmt(proc1, 3);
    writePkb.setProcAssignStmt(proc1, 5);
    writePkb.setProcAssignStmt(proc1, 6);
    writePkb.setProcAssignStmt(proc1, 7);
    writePkb.setProcAssignStmt(proc1, 8);
    writePkb.setProcAssignStmt(proc1, 9);
    writePkb.setProcAssignStmt(proc1, 10);
    writePkb.setProcAssignStmt(proc1, 11);
    writePkb.setProcAssignStmt(proc1, 12);
    writePkb.setProcAssignStmt(proc1, 13);
    writePkb.setProcAssignStmt(proc1, 14);
    writePkb.setProcAssignStmt(proc1, 18);
    writePkb.setProcAssignStmt(proc1, 19);
    writePkb.setProcAssignStmt(proc1, 20);
    writePkb.setProcAssignStmt(proc1, 22);
    writePkb.setProcAssignStmt(proc1, 24);
    writePkb.setProcAssignStmt(proc1, 25);
    writePkb.setProcAssignStmt(proc1, 26);
    writePkb.setProcAssignStmt(proc2, 29);

    std::unordered_set<StmtNum> p1nums = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
                                          15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
    writePkb.setProcedure(proc1, p1nums);
    writePkb.setStatement(AppConstants::ASSIGN, 28);
    std::unordered_set<StmtNum> p2nums = {28};
    writePkb.setProcedure(proc2, p2nums);

    std::unordered_set<Ent> ms1 = {"y", "x", "v"};
    std::unordered_set<Ent> ms2 = {"y"};
    std::unordered_set<Ent> ms3 = {"x"};
    std::unordered_set<Ent> ms4 = {"v", "y"};
    std::unordered_set<Ent> ms5 = {"v"};
    std::unordered_set<Ent> ms6 = {"y"};
    std::unordered_set<Ent> ms7 = {"v"};
    std::unordered_set<Ent> ms8 = {"v"};
    std::unordered_set<Ent> ms9 = {"x"};
    std::unordered_set<Ent> ms10 = {"c"};
    std::unordered_set<Ent> ms11 = {"d"};
    std::unordered_set<Ent> ms12 = {"f"};
    std::unordered_set<Ent> ms13 = {"e"};
    std::unordered_set<Ent> ms14 = {"a"};
    std::unordered_set<Ent> ms15 = {"b"};
    std::unordered_set<Ent> ms16 = {"j"};
    std::unordered_set<Ent> ms17 = {};
    std::unordered_set<Ent> ms18 = {"d"};
    std::unordered_set<Ent> ms19 = {"h"};
    std::unordered_set<Ent> ms20 = {"i"};
    std::unordered_set<Ent> ms21 = {};
    std::unordered_set<Ent> ms22 = {"j"};
    std::unordered_set<Ent> ms23 = {"k", "l"};
    std::unordered_set<Ent> ms24 = {"k"};
    std::unordered_set<Ent> ms25 = {"l"};
    std::unordered_set<Ent> ms26 = {"m"};
    writePkb.setModifiesS(1, ms1);
    writePkb.setModifiesS(2, ms2);
    writePkb.setModifiesS(3, ms3);
    writePkb.setModifiesS(4, ms4);
    writePkb.setModifiesS(5, ms5);
    writePkb.setModifiesS(6, ms6);
    writePkb.setModifiesS(7, ms7);
    writePkb.setModifiesS(8, ms8);
    writePkb.setModifiesS(9, ms9);
    writePkb.setModifiesS(10, ms10);
    writePkb.setModifiesS(11, ms11);
    writePkb.setModifiesS(12, ms12);
    writePkb.setModifiesS(13, ms13);
    writePkb.setModifiesS(14, ms14);
    writePkb.setModifiesS(15, ms15);
    writePkb.setModifiesS(16, ms16);
    writePkb.setModifiesS(17, ms17);
    writePkb.setModifiesS(18, ms18);
    writePkb.setModifiesS(19, ms19);
    writePkb.setModifiesS(20, ms20);
    writePkb.setModifiesS(21, ms21);
    writePkb.setModifiesS(22, ms22);
    writePkb.setModifiesS(23, ms23);
    writePkb.setModifiesS(24, ms24);
    writePkb.setModifiesS(25, ms25);
    writePkb.setModifiesS(26, ms26);

    std::unordered_set<Ent> mp1 = {"y", "x", "v", "c", "d", "f", "e", "a", "b", "h", "j", "k", "l", "m", "i"};
    writePkb.setModifiesP(proc1, mp1);

    std::unordered_set<Ent> ms29 = {"j"};
    writePkb.setModifiesS(29, ms29);
    writePkb.setModifiesP(proc2, ms29);

    std::unordered_set<Ent> us1{"a", "b"};
    std::unordered_set<Ent> us2{"v"};
    std::unordered_set<Ent> us3{"x"};
    std::unordered_set<Ent> us4{"x", "v", "y"};
    std::unordered_set<Ent> us5{"y"};
    std::unordered_set<Ent> us6{"x"};
    std::unordered_set<Ent> us7{};
    std::unordered_set<Ent> us8{"v"};
    std::unordered_set<Ent> us9{"y"};
    std::unordered_set<Ent> us10{};
    std::unordered_set<Ent> us11{"c"};
    std::unordered_set<Ent> us12{"e"};
    std::unordered_set<Ent> us13{"g"};
    std::unordered_set<Ent> us14{"a"};
    std::unordered_set<Ent> us17{"d"};
    std::unordered_set<Ent> us18{"d"};
    std::unordered_set<Ent> us19{"h"};
    std::unordered_set<Ent> us20{"h", "i"};
    std::unordered_set<Ent> us21{};
    std::unordered_set<Ent> us22{"j"};
    std::unordered_set<Ent> us23{"k", "l"};
    std::unordered_set<Ent> us24{"l"};
    std::unordered_set<Ent> us25{"k"};
    std::unordered_set<Ent> us26{"i"};
    std::unordered_set<Ent> us27{"k"};
    std::unordered_set<Ent> us28{"l"};
    writePkb.setUsesS(1, us1);
    writePkb.setUsesS(2, us2);
    writePkb.setUsesS(3, us3);
    writePkb.setUsesS(4, us4);
    writePkb.setUsesS(5, us5);
    writePkb.setUsesS(6, us6);
    writePkb.setUsesS(7, us7);
    writePkb.setUsesS(8, us8);
    writePkb.setUsesS(9, us9);
    writePkb.setUsesS(10, us10);
    writePkb.setUsesS(11, us11);
    writePkb.setUsesS(12, us12);
    writePkb.setUsesS(13, us13);
    writePkb.setUsesS(14, us14);
    writePkb.setUsesS(17, us17);
    writePkb.setUsesS(18, us18);
    writePkb.setUsesS(19, us19);
    writePkb.setUsesS(20, us20);
    writePkb.setUsesS(21, us21);
    writePkb.setUsesS(22, us22);
    writePkb.setUsesS(23, us23);
    writePkb.setUsesS(24, us24);
    writePkb.setUsesS(25, us25);
    writePkb.setUsesS(26, us26);
    writePkb.setUsesS(27, us27);
    writePkb.setUsesS(28, us28);

    std::unordered_set<Ent> up1 = {"a", "b", "y", "x", "v", "c", "e", "g", "d", "h", "j", "k", "l", "i"};
    writePkb.setUsesP(proc1, up1);

    std::unordered_set<Ent> us29 = {"j"};
    writePkb.setUsesS(29, us29);
    writePkb.setUsesP(proc2, us29);

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
        REQUIRE(res1.size() == 1);

        std::vector<Parameter> params2 = {Parameter("19", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"19", "19"}, {"19", "20"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("15", ParameterType::FIXED_INT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {};
        REQUIRE(expected3 == res3);

        std::vector<Parameter> params4 = {Parameter("4", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTS, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(expected4 == res4);

        std::vector<Parameter> params5 = {Parameter("20", ParameterType::FIXED_INT),
                                          Parameter("c", ParameterType::CALL)};
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
        REQUIRE(res1.size() == 1);

        std::vector<Parameter> params2 = {Parameter("s", ParameterType::STMT),
                                          Parameter("3", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"9", "3"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("c", ParameterType::CALL),
                                          Parameter("3", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTS, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        std::vector<std::vector<std::string>> expected3 = {};
        REQUIRE(unit_testing_utils::equals(expected3, res3));

        std::vector<Parameter> params4 = {Parameter("s", ParameterType::STMT),
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
        REQUIRE(expected5 == res5);
    }

    SECTION("Affects(_, _)") {
        std::vector<Parameter> params1 = {Parameter("a", ParameterType::ASSIGN), Parameter("s", ParameterType::STMT)};
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
        std::vector<std::vector<std::string>> expected4 = {};
        REQUIRE(res4.size() == 1);

        std::vector<Parameter> params5 = {Parameter("s", ParameterType::STMT), Parameter("s", ParameterType::STMT)};
        shared_ptr<Relationship> rs5 = Relationship::makeRelationship(AppConstants::AFFECTS, params5);
        std::vector<std::vector<std::string>> res5 = readPkb.findRelationship(rs5);
        std::vector<std::vector<std::string>> expected5 = {{"14", "14"}, {"19", "19"}};
        REQUIRE(unit_testing_utils::equals(expected5, res5));
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
        REQUIRE(res1.size() == 1);

        std::vector<Parameter> params2 = {Parameter("9", ParameterType::FIXED_INT),
                                          Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"9", "3"}, {"9", "6"}, {"9", "9"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("9", ParameterType::FIXED_INT),
                                          Parameter("pn", ParameterType::PRINT)};
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
        REQUIRE(res1.size() == 1);

        std::vector<Parameter> params2 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("6", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"2", "6"}, {"3", "6"}, {"6", "6"},
                                                           {"7", "6"}, {"8", "6"}, {"9", "6"}};
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
        REQUIRE(res1.size() == 1);

        std::vector<Parameter> params2 = {Parameter("aa", ParameterType::ASSIGN),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTST, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {
            {"2", "5"},   {"2", "3"},   {"2", "6"},   {"2", "9"},   {"3", "6"},   {"3", "9"},   {"3", "3"},
            {"6", "9"},   {"6", "3"},   {"6", "6"},   {"7", "8"},   {"7", "2"},   {"7", "5"},   {"7", "9"},
            {"7", "6"},   {"7", "3"},   {"8", "2"},   {"8", "5"},   {"8", "9"},   {"8", "6"},   {"8", "3"},
            {"9", "3"},   {"9", "6"},   {"9", "9"},   {"10", "11"}, {"10", "18"}, {"11", "18"}, {"13", "12"},
            {"14", "14"}, {"19", "19"}, {"19", "20"}, {"19", "26"}, {"20", "26"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));

        std::vector<Parameter> params3 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("as", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::AFFECTST, params3);
        std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
        REQUIRE(unit_testing_utils::equals(expected2, res3));

        std::vector<Parameter> params4 = {Parameter("s", ParameterType::STMT),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTST, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        REQUIRE(unit_testing_utils::equals(expected2, res4));

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

    writePkb.setProcAssignStmt(proc1, 1);
    writePkb.setProcAssignStmt(proc1, 2);
    writePkb.setProcAssignStmt(proc1, 3);
    writePkb.setProcAssignStmt(proc1, 4);
    writePkb.setProcAssignStmt(proc1, 7);
    writePkb.setProcAssignStmt(proc2, 11);
    writePkb.setProcAssignStmt(proc2, 12);
    writePkb.setProcAssignStmt(proc2, 13);
    writePkb.setProcAssignStmt(proc2, 14);
    writePkb.setProcAssignStmt(proc2, 19);
    writePkb.setProcAssignStmt(proc3, 20);
    writePkb.setProcAssignStmt(proc3, 21);

    std::unordered_set<StmtNum> proc1nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::unordered_set<StmtNum> proc2nums = {11, 12, 13, 14, 15, 16, 17, 18, 19};
    std::unordered_set<StmtNum> proc3nums = {20, 21, 22};
    writePkb.setProcedure(proc1, proc1nums);
    writePkb.setProcedure(proc2, proc2nums);
    writePkb.setProcedure(proc3, proc3nums);

    std::unordered_set<Ent> ms1 = {"x"};
    std::unordered_set<Ent> ms2 = {"y"};
    std::unordered_set<Ent> ms3 = {"z"};
    std::unordered_set<Ent> ms4 = {"c"};
    std::unordered_set<Ent> ms5 = {"c"};
    std::unordered_set<Ent> ms7 = {"c"};
    std::unordered_set<Ent> ms8 = {"y", "a", "b", "c", "d", "e"};
    std::unordered_set<Ent> ms9 = {"d", "e"};
    std::unordered_set<Ent> ms11 = {"y"};
    std::unordered_set<Ent> ms12 = {"a"};
    std::unordered_set<Ent> ms13 = {"b"};
    std::unordered_set<Ent> ms14 = {"c"};
    std::unordered_set<Ent> ms15 = {"b"};
    std::unordered_set<Ent> ms16 = {"d", "e", "a"};
    std::unordered_set<Ent> ms17 = {"d", "e"};
    std::unordered_set<Ent> ms19 = {"a"};
    std::unordered_set<Ent> ms20 = {"d"};
    std::unordered_set<Ent> ms21 = {"e"};
    writePkb.setModifiesS(1, ms1);
    writePkb.setModifiesS(2, ms2);
    writePkb.setModifiesS(3, ms3);
    writePkb.setModifiesS(4, ms4);
    writePkb.setModifiesS(5, ms5);
    writePkb.setModifiesS(7, ms7);
    writePkb.setModifiesS(8, ms8);
    writePkb.setModifiesS(9, ms9);
    writePkb.setModifiesS(11, ms11);
    writePkb.setModifiesS(12, ms12);
    writePkb.setModifiesS(13, ms13);
    writePkb.setModifiesS(14, ms14);
    writePkb.setModifiesS(15, ms15);
    writePkb.setModifiesS(16, ms16);
    writePkb.setModifiesS(17, ms17);
    writePkb.setModifiesS(19, ms19);
    writePkb.setModifiesS(20, ms20);
    writePkb.setModifiesS(21, ms21);

    std::unordered_set<Ent> mp1 = {"x", "y", "z", "a", "b", "c", "d", "e"};
    std::unordered_set<Ent> mp2 = {"y", "a", "b", "c", "d", "e"};
    std::unordered_set<Ent> mp3 = {"d", "e"};
    writePkb.setModifiesP(proc1, mp1);
    writePkb.setModifiesP(proc2, mp2);
    writePkb.setModifiesP(proc3, mp3);

    std::unordered_set<Ent> us1 = {"x"};
    std::unordered_set<Ent> us2 = {"y", "x"};
    std::unordered_set<Ent> us3 = {"x", "y", "c"};
    std::unordered_set<Ent> us5 = {"x", "c", "a", "y", "z", "b", "d"};
    std::unordered_set<Ent> us6 = {"c"};
    std::unordered_set<Ent> us7 = {"c"};
    std::unordered_set<Ent> us8 = {"x", "c", "a", "y", "z", "b", "c", "d"};
    std::unordered_set<Ent> us9 = {"d"};
    std::unordered_set<Ent> us11 = {"x"};
    std::unordered_set<Ent> us12 = {"c"};
    std::unordered_set<Ent> us13 = {"a", "y", "x", "z"};
    std::unordered_set<Ent> us16 = {"b", "c", "d"};
    std::unordered_set<Ent> us17 = {"d"};
    std::unordered_set<Ent> us19 = {"b"};
    std::unordered_set<Ent> us21 = {"d"};
    writePkb.setUsesS(1, us1);
    writePkb.setUsesS(2, us2);
    writePkb.setUsesS(3, us3);
    writePkb.setUsesS(5, us5);
    writePkb.setUsesS(6, us6);
    writePkb.setUsesS(7, us7);
    writePkb.setUsesS(8, us8);
    writePkb.setUsesS(9, us9);
    writePkb.setUsesS(11, us11);
    writePkb.setUsesS(12, us12);
    writePkb.setUsesS(13, us13);
    writePkb.setUsesS(16, us16);
    writePkb.setUsesS(17, us17);
    writePkb.setUsesS(19, us19);
    writePkb.setUsesS(21, us21);

    std::unordered_set<Ent> up1 = {"x", "y", "c", "a", "y", "z", "b", "d"};
    std::unordered_set<Ent> up2 = {"x", "c", "a", "y", "z", "b", "d"};
    std::unordered_set<Ent> up3 = {"d"};
    writePkb.setUsesP(proc1, up1);
    writePkb.setUsesP(proc2, up2);
    writePkb.setUsesP(proc3, up3);

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
        REQUIRE(res1.size() == 1);

        std::vector<Parameter> params2 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("a1", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected2 = {{"1", "2"}, {"1", "3"},   {"2", "3"},   {"7", "7"},
                                                           {"4", "7"}, {"11", "13"}, {"12", "13"}, {"20", "21"}};
        REQUIRE(unit_testing_utils::equals(expected2, res2));
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

    writePkb.setProcAssignStmt(proc1, 2);
    writePkb.setProcAssignStmt(proc1, 5);
    writePkb.setProcAssignStmt(proc1, 6);
    writePkb.setProcAssignStmt(proc1, 7);
    writePkb.setProcAssignStmt(proc1, 8);

    std::unordered_set<StmtNum> proc1nums = {1, 2, 3, 4, 5, 6, 7, 8};
    writePkb.setProcedure(proc1, proc1nums);

    std::unordered_set<Ent> ms1 = {"b", "d", "a"};
    std::unordered_set<Ent> ms2 = {"d"};
    std::unordered_set<Ent> ms3 = {"b", "d", "a"};
    std::unordered_set<Ent> ms4 = {"a"};
    std::unordered_set<Ent> ms5 = {"a"};
    std::unordered_set<Ent> ms6 = {"b"};
    std::unordered_set<Ent> ms7 = {"d"};
    std::unordered_set<Ent> ms8 = {"b"};
    writePkb.setModifiesS(1, ms1);
    writePkb.setModifiesS(2, ms2);
    writePkb.setModifiesS(3, ms3);
    writePkb.setModifiesS(4, ms4);
    writePkb.setModifiesS(5, ms5);
    writePkb.setModifiesS(6, ms6);
    writePkb.setModifiesS(7, ms7);
    writePkb.setModifiesS(8, ms8);

    std::unordered_set<Ent> mp1 = {"d", "a", "b"};
    writePkb.setModifiesP(proc1, mp1);

    std::unordered_set<Ent> us1 = {"v", "d", "a", "c", "b", "d", "e"};
    std::unordered_set<Ent> us3 = {"d", "a", "c", "b", "d", "e"};
    std::unordered_set<Ent> us4 = {"e", "d"};
    std::unordered_set<Ent> us5 = {"d", "e"};
    std::unordered_set<Ent> us6 = {"a", "c"};
    std::unordered_set<Ent> us7 = {"b"};
    std::unordered_set<Ent> us8 = {"d"};
    writePkb.setUsesS(1, us1);
    writePkb.setUsesS(3, us3);
    writePkb.setUsesS(4, us4);
    writePkb.setUsesS(5, us5);
    writePkb.setUsesS(6, us6);
    writePkb.setUsesS(7, us7);
    writePkb.setUsesS(8, us8);

    std::unordered_set<Ent> up1 = {"v", "d", "a", "c", "b", "d", "e"};
    writePkb.setUsesP(proc1, up1);

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
        REQUIRE(res1.size() == 1);

        std::vector<Parameter> params2 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected = {{"2", "5"}, {"2", "8"}, {"5", "6"},
                                                          {"6", "7"}, {"7", "5"}, {"7", "8"}};
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

    writePkb.setProcAssignStmt(proc1, 1);
    writePkb.setProcAssignStmt(proc1, 3);
    writePkb.setProcAssignStmt(proc1, 5);
    writePkb.setProcAssignStmt(proc1, 6);
    writePkb.setProcAssignStmt(proc1, 7);
    writePkb.setProcAssignStmt(proc1, 8);
    writePkb.setProcAssignStmt(proc1, 9);
    writePkb.setProcAssignStmt(proc1, 10);
    writePkb.setProcAssignStmt(proc1, 11);
    writePkb.setProcAssignStmt(proc1, 12);

    std::unordered_set<StmtNum> proc1nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    writePkb.setProcedure(proc1, proc1nums);

    std::unordered_set<Ent> ms1 = {"a"};
    std::unordered_set<Ent> ms3 = {"c"};
    std::unordered_set<Ent> ms5 = {"b"};
    std::unordered_set<Ent> ms6 = {"f"};
    std::unordered_set<Ent> ms7 = {"e"};
    std::unordered_set<Ent> ms8 = {"a"};
    std::unordered_set<Ent> ms9 = {"e"};
    std::unordered_set<Ent> ms10 = {"e"};
    std::unordered_set<Ent> ms11 = {"v"};
    std::unordered_set<Ent> ms12 = {"g"};
    writePkb.setModifiesS(1, ms1);
    writePkb.setModifiesS(3, ms3);
    writePkb.setModifiesS(5, ms5);
    writePkb.setModifiesS(6, ms6);
    writePkb.setModifiesS(7, ms7);
    writePkb.setModifiesS(8, ms8);
    writePkb.setModifiesS(9, ms9);
    writePkb.setModifiesS(10, ms10);
    writePkb.setModifiesS(11, ms11);
    writePkb.setModifiesS(12, ms12);

    std::unordered_set<Ent> mp1 = {"a", "c", "b", "f", "e", "v", "g"};
    writePkb.setModifiesP(proc1, mp1);

    std::unordered_set<Ent> us6 = {"a", "b"};
    std::unordered_set<Ent> us8 = {"a", "d", "f"};
    std::unordered_set<Ent> us9 = {"b", "c", "e"};
    std::unordered_set<Ent> us10 = {"b"};
    std::unordered_set<Ent> us11 = {"e"};
    std::unordered_set<Ent> us12 = {"a"};
    writePkb.setUsesS(6, us6);
    writePkb.setUsesS(8, us8);
    writePkb.setUsesS(9, us9);
    writePkb.setUsesS(10, us10);
    writePkb.setUsesS(11, us11);
    writePkb.setUsesS(12, us12);

    std::unordered_set<Ent> up1 = {"a", "b", "c", "d", "e", "f"};
    writePkb.setUsesP(proc1, up1);

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
        REQUIRE(res1.size() == 1);

        std::vector<Parameter> params2 = {Parameter("a", ParameterType::ASSIGN),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        std::vector<std::vector<std::string>> expected = {{"1", "6"}, {"1", "8"},  {"1", "12"}, {"5", "10"},
                                                          {"6", "8"}, {"8", "12"}, {"10", "11"}};
        REQUIRE(unit_testing_utils::equals(expected, res2));

        std::vector<Parameter> params3 = {Parameter("s", ParameterType::STMT),
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

    writePkb.setProcAssignStmt(proc1, 3);
    writePkb.setProcAssignStmt(proc1, 7);
    writePkb.setProcAssignStmt(proc1, 8);

    std::unordered_set<StmtNum> proc1nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    writePkb.setProcedure(proc1, proc1nums);

    std::unordered_set<Ent> ms1 = {"c", "d", "f"};
    std::unordered_set<Ent> ms2 = {"c"};
    std::unordered_set<Ent> ms3 = {"c"};
    std::unordered_set<Ent> ms4 = {"c"};
    std::unordered_set<Ent> ms5 = {"c"};
    std::unordered_set<Ent> ms6 = {"c"};
    std::unordered_set<Ent> ms7 = {"c"};
    std::unordered_set<Ent> ms8 = {"d"};
    std::unordered_set<Ent> ms9 = {};
    std::unordered_set<Ent> ms10 = {"c"};
    std::unordered_set<Ent> ms11 = {"f"};
    writePkb.setModifiesS(1, ms1);
    writePkb.setModifiesS(2, ms2);
    writePkb.setModifiesS(3, ms3);
    writePkb.setModifiesS(4, ms4);
    writePkb.setModifiesS(5, ms5);
    writePkb.setModifiesS(6, ms6);
    writePkb.setModifiesS(7, ms7);
    writePkb.setModifiesS(8, ms8);
    writePkb.setModifiesS(9, ms9);
    writePkb.setModifiesS(10, ms10);
    writePkb.setModifiesS(11, ms11);

    std::unordered_set<Ent> mp1 = {"c", "d", "f"};
    writePkb.setModifiesP(proc1, mp1);

    std::unordered_set<Ent> us1 = {"a", "b", "c"};
    std::unordered_set<Ent> us2 = {"a", "b", "c"};
    std::unordered_set<Ent> us3 = {"a", "b"};
    std::unordered_set<Ent> us4 = {"c"};
    std::unordered_set<Ent> us5 = {"b", "c"};
    std::unordered_set<Ent> us6 = {"c"};
    std::unordered_set<Ent> us7 = {""};
    std::unordered_set<Ent> us8 = {"d", "e"};
    std::unordered_set<Ent> us9 = {"g"};
    std::unordered_set<Ent> us10 = {"c"};
    writePkb.setUsesS(1, us1);
    writePkb.setUsesS(2, us2);
    writePkb.setUsesS(3, us3);
    writePkb.setUsesS(4, us4);
    writePkb.setUsesS(5, us5);
    writePkb.setUsesS(6, us6);
    writePkb.setUsesS(7, us7);
    writePkb.setUsesS(8, us8);
    writePkb.setUsesS(9, us9);
    writePkb.setUsesS(10, us10);

    std::unordered_set<Ent> up1 = {"a", "b", "c", "d", "e", "g"};
    writePkb.setUsesP(proc1, up1);

    std::unordered_set<StmtNum> proc1par = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    writePkb.setParentT(1, proc1par);

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

    writePkb.setProcAssignStmt(proc1, 8);
    writePkb.setProcAssignStmt(proc1, 12);
    writePkb.setProcAssignStmt(proc2, 16);
    writePkb.setProcAssignStmt(proc2, 17);
    writePkb.setProcAssignStmt(proc2, 20);
    writePkb.setProcAssignStmt(proc2, 22);
    writePkb.setProcAssignStmt(proc2, 23);
    writePkb.setProcAssignStmt(proc2, 25);
    writePkb.setProcAssignStmt(proc2, 26);
    writePkb.setProcAssignStmt(proc3, 29);

    std::unordered_set<StmtNum> proc1nums = {1, 2, 3, 4, 5, 6, 7, 8, 8, 10, 11, 12};
    std::unordered_set<StmtNum> proc2nums = {13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28};
    std::unordered_set<StmtNum> proc3nums = {29};
    writePkb.setProcedure(proc1, proc1nums);
    writePkb.setProcedure(proc2, proc2nums);
    writePkb.setProcedure(proc3, proc3nums);

    std::unordered_set<Ent> ms1 = {"p0licemAN"};
    std::unordered_set<Ent> ms2 = {"p0liceWahMan"};
    std::unordered_set<Ent> ms3 = {"pOl3icEwOM5N"};
    std::unordered_set<Ent> ms4 = {"a", "i", "N", "x", "y", "read", "rEaD", "rEEE"};
    std::unordered_set<Ent> ms5 = {"read"};
    std::unordered_set<Ent> ms6 = {"read"};
    std::unordered_set<Ent> ms7 = {"rEaD"};
    std::unordered_set<Ent> ms8 = {"rEEE"};
    std::unordered_set<Ent> ms9 = {"a", "i", "N", "x", "y"};
    std::unordered_set<Ent> ms10 = {};
    std::unordered_set<Ent> ms11 = {"a"};
    std::unordered_set<Ent> ms12 = {"m0ArWeirdNaMe5AnDtH7sIsVer9Lon6AnDILov3Ch3rr1E2"};
    writePkb.setModifiesS(1, ms1);
    writePkb.setModifiesS(2, ms2);
    writePkb.setModifiesS(3, ms3);
    writePkb.setModifiesS(4, ms4);
    writePkb.setModifiesS(5, ms5);
    writePkb.setModifiesS(6, ms6);
    writePkb.setModifiesS(7, ms7);
    writePkb.setModifiesS(8, ms8);
    writePkb.setModifiesS(9, ms9);
    writePkb.setModifiesS(10, ms10);
    writePkb.setModifiesS(11, ms11);
    writePkb.setModifiesS(12, ms12);

    std::unordered_set<Ent> mp1 = {"p0licemAN",
                                   "p0liceWahMan",
                                   "pOl3icEwOM5N",
                                   "a",
                                   "i",
                                   "N",
                                   "x",
                                   "y",
                                   "read",
                                   "rEaD",
                                   "rEEE",
                                   "m0ArWeirdNaMe5AnDtH7sIsVer9Lon6AnDILov3Ch3rr1E2"};
    writePkb.setModifiesP(proc1, mp1);

    std::unordered_set<Ent> ms13 = {"a"};
    std::unordered_set<Ent> ms14 = {"a"};
    std::unordered_set<Ent> ms15 = {"a"};
    std::unordered_set<Ent> ms16 = {"i"};
    std::unordered_set<Ent> ms17 = {"N"};
    std::unordered_set<Ent> ms18 = {"a", "i"};
    std::unordered_set<Ent> ms19 = {"a"};
    std::unordered_set<Ent> ms20 = {"i"};
    std::unordered_set<Ent> ms21 = {"i"};
    std::unordered_set<Ent> ms22 = {"x"};
    std::unordered_set<Ent> ms23 = {"y"};
    std::unordered_set<Ent> ms24 = {"x", "y"};
    std::unordered_set<Ent> ms25 = {"x"};
    std::unordered_set<Ent> ms26 = {"y"};
    std::unordered_set<Ent> ms27 = {};
    std::unordered_set<Ent> ms28 = {};
    writePkb.setModifiesS(13, ms13);
    writePkb.setModifiesS(14, ms14);
    writePkb.setModifiesS(15, ms15);
    writePkb.setModifiesS(16, ms16);
    writePkb.setModifiesS(17, ms17);
    writePkb.setModifiesS(18, ms18);
    writePkb.setModifiesS(19, ms19);
    writePkb.setModifiesS(20, ms20);
    writePkb.setModifiesS(21, ms21);
    writePkb.setModifiesS(22, ms22);
    writePkb.setModifiesS(23, ms23);
    writePkb.setModifiesS(24, ms24);
    writePkb.setModifiesS(25, ms25);
    writePkb.setModifiesS(26, ms26);
    writePkb.setModifiesS(27, ms27);
    writePkb.setModifiesS(28, ms28);

    std::unordered_set<Ent> mp2 = {"a", "i", "N", "x", "y"};
    writePkb.setModifiesP(proc2, mp2);

    std::unordered_set<Ent> mp3 = {"a"};
    writePkb.setModifiesS(29, mp3);
    writePkb.setModifiesP(proc3, mp3);

    std::unordered_set<Ent> us1 = {};
    std::unordered_set<Ent> us2 = {};
    std::unordered_set<Ent> us3 = {};
    std::unordered_set<Ent> us4 = {"p0licemAN", "p0liceWahMan", "rEaD", "read", "i", "N", "x", "y", "print"};
    std::unordered_set<Ent> us5 = {};
    std::unordered_set<Ent> us6 = {};
    std::unordered_set<Ent> us7 = {};
    std::unordered_set<Ent> us8 = {"rEaD", "read"};
    std::unordered_set<Ent> us9 = {"i", "N", "x", "y"};
    std::unordered_set<Ent> us10 = {"print"};
    std::unordered_set<Ent> us11 = {};
    std::unordered_set<Ent> us12 = {};
    writePkb.setUsesS(1, us1);
    writePkb.setUsesS(2, us2);
    writePkb.setUsesS(3, us3);
    writePkb.setUsesS(4, us4);
    writePkb.setUsesS(5, us5);
    writePkb.setUsesS(6, us6);
    writePkb.setUsesS(7, us7);
    writePkb.setUsesS(8, us8);
    writePkb.setUsesS(9, us9);
    writePkb.setUsesS(10, us10);
    writePkb.setUsesS(11, us11);
    writePkb.setUsesS(12, us12);

    std::unordered_set<Ent> up1 = {"i",    "N", "x", "y", "p0licemAN", "p0liceWahMan", "rEaD",
                                   "read", "i", "N", "x", "y",         "print"};
    writePkb.setUsesP(proc1, up1);

    std::unordered_set<Ent> us13 = {};
    std::unordered_set<Ent> us14 = {};
    std::unordered_set<Ent> us15 = {};
    std::unordered_set<Ent> us16 = {};
    std::unordered_set<Ent> us17 = {};
    std::unordered_set<Ent> us18 = {"i", "N"};
    std::unordered_set<Ent> us19 = {};
    std::unordered_set<Ent> us20 = {"i"};
    std::unordered_set<Ent> us21 = {"i"};
    std::unordered_set<Ent> us22 = {};
    std::unordered_set<Ent> us23 = {};
    std::unordered_set<Ent> us24 = {"x", "y"};
    std::unordered_set<Ent> us25 = {"y"};
    std::unordered_set<Ent> us26 = {"x"};
    std::unordered_set<Ent> us27 = {"x"};
    std::unordered_set<Ent> us28 = {"y"};
    writePkb.setUsesS(13, us13);
    writePkb.setUsesS(14, us14);
    writePkb.setUsesS(15, us15);
    writePkb.setUsesS(16, us16);
    writePkb.setUsesS(17, us17);
    writePkb.setUsesS(18, us18);
    writePkb.setUsesS(19, us19);
    writePkb.setUsesS(20, us20);
    writePkb.setUsesS(21, us21);
    writePkb.setUsesS(22, us22);
    writePkb.setUsesS(23, us23);
    writePkb.setUsesS(24, us24);
    writePkb.setUsesS(25, us25);
    writePkb.setUsesS(26, us26);
    writePkb.setUsesS(27, us27);
    writePkb.setUsesS(28, us28);

    std::unordered_set<Ent> up2 = {"i", "N", "x", "y"};
    writePkb.setUsesP(proc2, up2);

    std::unordered_set<Ent> up3 = {};
    writePkb.setUsesS(29, up3);
    writePkb.setUsesP(proc3, up3);

    std::unordered_set<StmtNum> parT4 = {5, 6, 7, 8, 9, 10};
    std::unordered_set<StmtNum> parT18 = {19, 20, 21};
    std::unordered_set<StmtNum> parT24 = {25, 26};
    writePkb.setParentT(4, parT4);
    writePkb.setParentT(18, parT18);
    writePkb.setParentT(24, parT24);

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
        std::vector<Parameter> params1 = {Parameter("s", ParameterType::STMT),
                                          Parameter("20", ParameterType::FIXED_INT)};
        shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::AFFECTS, params1);
        std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected1 = {{"16", "20"}, {"20", "20"}};
        REQUIRE(unit_testing_utils::equals(expected1, res1));

        std::vector<Parameter> params2 = {Parameter("_", ParameterType::WILDCARD),
                                          Parameter("_", ParameterType::WILDCARD)};
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::AFFECTS, params2);
        std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
        REQUIRE(res2.size() == 1);
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
        REQUIRE(res3.size() == 1);

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
        REQUIRE(res3.size() == 1);

        std::vector<Parameter> params4 = {Parameter("a", ParameterType::ASSIGN), Parameter("a", ParameterType::ASSIGN)};
        shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::AFFECTST, params4);
        std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
        std::vector<std::vector<std::string>> expected4 = {{"20", "20"}};
        REQUIRE(unit_testing_utils::equals(expected4, res4));
    }
}
