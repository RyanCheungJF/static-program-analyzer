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
            {1,
                    {
                            {AppConstants::PARENTS, {}},
                            {AppConstants::CHILDREN, {2, 3}}
                    }
            },
            {2,
                    {
                            {AppConstants::PARENTS, {1}},
                            {AppConstants::CHILDREN, {5}}
                    }
            },
            {3,
                    {
                            {AppConstants::PARENTS, {1}},
                            {AppConstants::CHILDREN, {4}}
                    }
            },
            {4,
                    {
                            {AppConstants::PARENTS, {3}},
                            {AppConstants::CHILDREN, {5}}
                    }
            },
            {5,
                    {
                            {AppConstants::PARENTS, {2, 4}},
                            {AppConstants::CHILDREN, {6}}
                    }
            },
            {6,
                    {
                            {AppConstants::PARENTS, {5, 7}},
                            {AppConstants::CHILDREN, {7}}
                    }
            },
            {7,
                    {
                            {AppConstants::PARENTS, {6}},
                            {AppConstants::CHILDREN, {6, 8}}
                    }
            },
            {8,
                    {
                            {AppConstants::PARENTS, {7}},
                            {AppConstants::CHILDREN, {}}
                    }
            }
    };

    ProcName proc1 = "proc1";
    writePkb.writeCFG(proc1, graph1);
    writePkb.setStatement("if", 1);
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("read", 3);
    writePkb.setStatement("print", 4);
    writePkb.setStatement("call", 5);
    writePkb.setStatement("while", 6);
    writePkb.setStatement("assign", 7);
    writePkb.setStatement("print", 8);
    writePkb.setProcedure(proc1, {1,2,3,4,5,6,7,8});

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph2 = {
            {9,
                    {
                            {AppConstants::PARENTS, {11}},
                            {AppConstants::CHILDREN, {10}}
                    }
            },
            {10,
                    {
                            {AppConstants::PARENTS, {9}},
                            {AppConstants::CHILDREN, {11}}
                    }
            },
            {11,
                    {
                            {AppConstants::PARENTS, {10}},
                            {AppConstants::CHILDREN, {12, 9}}
                    }
            },
            {12,
                    {
                            {AppConstants::PARENTS, {10}},
                            {AppConstants::CHILDREN, {}}
                    }
            }
    };

    ProcName proc2 = "proc2";
    writePkb.writeCFG(proc2, graph2);
    writePkb.setStatement("while", 9);
    writePkb.setStatement("assign", 10);
    writePkb.setStatement("assign", 11);
    writePkb.setStatement("print", 12);
    writePkb.setProcedure(proc2, {9, 10, 11, 12});

     SECTION("Next*(int, int)") {
         std::vector<Parameter> params = {Parameter("1", AppConstants::FIXED_INT),
                                           Parameter("2", AppConstants::FIXED_INT)};
         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
         std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
         REQUIRE(unit_testing_utils::equals({{"1", "2"}}, res));
     }

     SECTION("Next*(int, _)") {
         std::vector<Parameter> params = {Parameter("1", AppConstants::FIXED_INT),
                                          Parameter("_", AppConstants::WILDCARD)};
         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
         std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected = { { "1", "2" }, { "1", "3" }, { "1", "4" },
                                                            { "1", "5" }, { "1", "6" }, { "1", "7" },
                                                            { "1", "8" } };
        REQUIRE(unit_testing_utils::equals(expected, res));
     }

     SECTION("Next*(int, stmt)") {
         std::vector<Parameter> params = {Parameter("10", AppConstants::FIXED_INT),
                                          Parameter("s2", AppConstants::STMT)};
         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
         std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected = { { "10", "11" }, { "10", "12" }, {"10", "9"},
                                                           {"10", "10"}};

        REQUIRE(unit_testing_utils::equals(expected, res));
     }

     SECTION("Next*(_, int)") {
         std::vector<Parameter> params = {Parameter("_", AppConstants::WILDCARD),
                                          Parameter("5", AppConstants::FIXED_INT)};
         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXTT, params);
         std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
        std::vector<std::vector<std::string>> expected = {{"1", "5"}, {"2", "5"}, {"3", "5"},
                                                          {"4", "5"}};
        REQUIRE(unit_testing_utils::equals(expected, res));
//        REQUIRE(expected == res);
     }

//     SECTION("Next(stmt, int)") {
//         std::vector<Parameter> params = {Parameter("asTeeAmTii", AppConstants::STMT),
//                                          Parameter("1", AppConstants::FIXED_INT)};
//         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
//         std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
//         REQUIRE(unit_testing_utils::equals({}, res));


//         std::vector<Parameter> params2 = {Parameter("asTeeAmTii", AppConstants::STMT),
//                                          Parameter("8", AppConstants::FIXED_INT)};
//         shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXT, params2);
//         std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
//         REQUIRE(unit_testing_utils::equals({{"7", "8"}}, res2));
//     }

//     SECTION("Next(stmttype, int)") {
//         std::vector<Parameter> params1 = {Parameter("a1", AppConstants::ASSIGN),
//                                           Parameter("8", AppConstants::FIXED_INT)};
//         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params1);
//         std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
//         REQUIRE(unit_testing_utils::equals({{"7", "8"}}, res1));

//         std::vector<Parameter> params2 = {Parameter("i1", AppConstants::IF),
//                                           Parameter("7", AppConstants::FIXED_INT)};
//         shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXT, params2);
//         std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
//         REQUIRE(unit_testing_utils::equals({}, res2));

//         std::vector<Parameter> params3 = {Parameter("re", AppConstants::READ),
//                                           Parameter("4", AppConstants::FIXED_INT)};
//         shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::NEXT, params3);
//         std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
//         REQUIRE(unit_testing_utils::equals({{"3", "4"}}, res3));
//     }

//     SECTION("Next(int, stmttype)") {
//         std::vector<Parameter> params1 = {Parameter("11", AppConstants::FIXED_INT),
//                                           Parameter("w", AppConstants::WHILE)};
//         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params1);
//         std::vector<std::vector<std::string>> res1 = readPkb.findRelationship(rs1);
//         REQUIRE(unit_testing_utils::equals({{"11", "9"}}, res1));

//         std::vector<Parameter> params2 = {Parameter("11", AppConstants::FIXED_INT),
//                                           Parameter("prr", AppConstants::PRINT)};
//         shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXT, params2);
//         std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
//         REQUIRE(unit_testing_utils::equals({{"11", "12"}}, res2));

//         std::vector<Parameter> params3 = {Parameter("4", AppConstants::FIXED_INT),
//                                           Parameter("ca", AppConstants::CALL)};
//         shared_ptr<Relationship> rs3 = Relationship::makeRelationship(AppConstants::NEXT, params3);
//         std::vector<std::vector<std::string>> res3 = readPkb.findRelationship(rs3);
//         REQUIRE(unit_testing_utils::equals({{"4", "5"}}, res3));


//         std::vector<Parameter> params4 = {Parameter("5", AppConstants::FIXED_INT),
//                                           Parameter("ca", AppConstants::CALL)};
//         shared_ptr<Relationship> rs4 = Relationship::makeRelationship(AppConstants::NEXT, params4);
//         std::vector<std::vector<std::string>> res4 = readPkb.findRelationship(rs4);
//         REQUIRE(unit_testing_utils::equals({}, res4));
//     }

//     SECTION("Next(stmttype, _)") {
//         std::vector<Parameter> params = {Parameter("a2", AppConstants::ASSIGN),
//                                          Parameter("_", AppConstants::WILDCARD)};
//         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
//         std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
//         REQUIRE(unit_testing_utils::equals({{"2", "5"}, {"7", "6"}, {"7", "8"}, {"11", "9"}, {"10", "11"}, {"11", "12"}}, res));
//     }

//     SECTION("Next(stmttype, stmt)") {
//         std::vector<Parameter> params = {Parameter("i2", AppConstants::IF),
//                                          Parameter("s2", AppConstants::STMT)};
//         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
//         std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
//         REQUIRE(unit_testing_utils::equals({{"1", "2"}, {"1", "3"}}, res));
//     }

//     SECTION("Next(_, stmttype)") {
//         std::vector<Parameter> params = {Parameter("_", AppConstants::WILDCARD),
//                                          Parameter("re2", AppConstants::CALL)};
//         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
//         std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
//         REQUIRE(unit_testing_utils::equals({{"2", "5"}, {"4", "5"}}, res));
//     }

//     SECTION("Next(stmt, stmttype)") {
//         std::vector<Parameter> params = {Parameter("st3", AppConstants::STMT),
//                                          Parameter("re2", AppConstants::WHILE)};
//         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
//         std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
//         REQUIRE(unit_testing_utils::equals({{"5", "6"}, {"7", "6"}, {"11", "9"}}, res));
//     }

//     SECTION("Next(stmttype, stmttype)") {
//         std::vector<Parameter> params = {Parameter("w3", AppConstants::WHILE),
//                                          Parameter("as3", AppConstants::ASSIGN)};
//         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
//         std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
//         REQUIRE(unit_testing_utils::equals({{"6", "7"}, {"9", "10"}}, res));


//         std::vector<Parameter> params2 = {Parameter("as4", AppConstants::ASSIGN),
//                                           Parameter("w4", AppConstants::WHILE)};
//         shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXT, params2);
//         std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
//         REQUIRE(unit_testing_utils::equals({{"7", "6"}, {"11", "9"}}, res2));
//     }

//     SECTION("Next(_, _)") {
//         std::vector<std::vector<std::string>> expected = { { "1", "3" }, { "1", "2" }, { "2", "5" },
//                                                            { "3", "4" }, { "4", "5" }, { "5", "6" },
//                                                            { "6", "7" }, { "7", "8" }, { "7", "6" },
//                                                            { "9", "10" }, { "10", "11" }, { "11", "9" },
//                                                            { "11", "12" } };

//         std::vector<Parameter> params = {Parameter("_", AppConstants::WILDCARD),
//                                          Parameter("_", AppConstants::WILDCARD)};
//         shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::NEXT, params);
//         std::vector<std::vector<std::string>> res = readPkb.findRelationship(rs1);
//         REQUIRE(unit_testing_utils::equals(expected, res));

//         std::vector<Parameter> params2 = {Parameter("st1", AppConstants::STMT),
//                                          Parameter("st2", AppConstants::STMT)};
//         shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::NEXT, params2);
//         std::vector<std::vector<std::string>> res2 = readPkb.findRelationship(rs2);
//         REQUIRE(unit_testing_utils::equals(expected, res2));
//     }

//    // TODO: need to remap the values
//    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph3 = {
//            {13,
//                    {
//                            {AppConstants::PARENTS, {18}},
//                            {AppConstants::CHILDREN, {14}}
//                    }
//            },
//            {14,
//                    {
//                            {AppConstants::PARENTS, {13}},
//                            {AppConstants::CHILDREN, {15, 17}}
//                    }
//            },
//            {15,
//                    {
//                            {AppConstants::PARENTS, {14, 16}},
//                            {AppConstants::CHILDREN, {16}}
//                    }
//            },
//            {16,
//                    {
//                            {AppConstants::PARENTS, {15}},
//                            {AppConstants::CHILDREN, {18}}
//                    }
//            },
//            {17,
//                    {
//                            {AppConstants::PARENTS, {14, 17}},
//                            {AppConstants::CHILDREN, {18}}
//                    }
//            },
//            {18,
//                    {
//                            {AppConstants::PARENTS, {16, 17}},
//                            {AppConstants::CHILDREN, {13, 19}}
//                    }
//            },
//            {19,
//                    {
//                            {AppConstants::PARENTS, {18}},
//                            {AppConstants::CHILDREN, {20, 21}}
//                    }
//            },
//            {20,
//                    {
//                            {AppConstants::PARENTS, {19}},
//                            {AppConstants::CHILDREN, {}}
//                    }
//            },
//            {21,
//                    {
//                            {AppConstants::PARENTS, {19}},
//                            {AppConstants::CHILDREN, {22}}
//                    }
//            },
//            {22,
//                    {
//                            {AppConstants::PARENTS, {21}},
//                            {AppConstants::CHILDREN, {}}
//                    }
//            }
//    };
//
//    ProcName proc3 = "proc3";
//    writePkb.writeCFG(proc1, graph1);
//    writePkb.setStatement("while", 13);
//    writePkb.setStatement("if", 14);
//    writePkb.setStatement("while", 15);
//    writePkb.setStatement("print", 16);
//    writePkb.setStatement("while", 17);
//    writePkb.setStatement("if", 6);
//    writePkb.setStatement("read", 7);
//    writePkb.setStatement("call", 8);
//    writePkb.setProcedure(proc3, {1,2,3,4,5,6,7,8});

}