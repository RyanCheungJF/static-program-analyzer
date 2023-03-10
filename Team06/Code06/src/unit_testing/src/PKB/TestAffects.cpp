#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

TEST_CASE("findRelationship(shared_ptr<Relationship> rs): Affects") {
    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

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
     * }
     * print y; //10
     * print x; //11
     * call proc2; //12 which uses x but does not modifies x [should still be false]
     * call proc3 //13 which uses and modifies x [definitely false]
     * x = y + 3; //14
     *
     * Valid Affects relationships:
     * (2, 5), (3, 6), (6, 9), (6, 10), (7, 8), (8, 2), (9, 3), (9, 11)
     */
    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph1 = {
        {1, {
                {AppConstants::PARENTS, {9}},
                {AppConstants::CHILDREN, {2}}
            }
        },
        {2, {
                    {AppConstants::PARENTS, {1}},
                    {AppConstants::CHILDREN, {3}}
            }
        },
        {3, {
                    {AppConstants::PARENTS, {2}},
                    {AppConstants::CHILDREN, {4}}
            }
        },
        {4, {
                    {AppConstants::PARENTS, {3}},
                    {AppConstants::CHILDREN, {5, 6}}
            }
        },
        {5, {
                    {AppConstants::PARENTS, {4}},
                    {AppConstants::CHILDREN, {7}}
            }
        },
        {6, {
                    {AppConstants::PARENTS, {4}},
                    {AppConstants::CHILDREN, {7}}
            }
        },
        {7, {
                    {AppConstants::PARENTS, {5, 6}},
                    {AppConstants::CHILDREN, {8}}
            }
        },
        {8, {
                    {AppConstants::PARENTS, {7}},
                    {AppConstants::CHILDREN, {9}}
            }
        },
        {9, {
                    {AppConstants::PARENTS, {8}},
                    {AppConstants::CHILDREN, {1, 10}}
            }
        },
        {10, {
                    {AppConstants::PARENTS, {9}},
                    {AppConstants::CHILDREN, {11}}
            }
        },
        {11, {
                     {AppConstants::PARENTS, {10}},
                     {AppConstants::CHILDREN, {12}}
             }
        },
        {12, {
                     {AppConstants::PARENTS, {11}},
                     {AppConstants::CHILDREN, {13}}
             }
        },
        {13, {
                     {AppConstants::PARENTS, {12}},
                     {AppConstants::CHILDREN, {14}}
             }
        },
        {14, {
                     {AppConstants::PARENTS, {13}},
                     {AppConstants::CHILDREN, {}}
             }
        }
    };

    ProcName proc1 = "proc1";
    writePkb.writeCFG(proc1, graph1);
    writePkb.setStatement("while", 1);
    writePkb.setStatement("assign", 2);
    writePkb.setStatement("assign", 3);
    writePkb.setStatement("if", 4);
    writePkb.setStatement("assign", 5);
    writePkb.setStatement("assign", 6);
    writePkb.setStatement("assign", 7);
    writePkb.setStatement("assign", 8);
    writePkb.setStatement("assign", 9);
    writePkb.setStatement("print", 10);
    writePkb.setStatement("print", 11);
    writePkb.setStatement("call", 12);
    writePkb.setStatement("call", 13);
    writePkb.setStatement("assign", 14);
    writePkb.setProcedure(proc1, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
    
    writePkb.setModifiesS(1, {"y", "x", "v"});
    writePkb.setModifiesS(2, {"y"});
    writePkb.setModifiesS(3, {"x"});
    writePkb.setModifiesS(4, {"v", "y"});
    writePkb.setModifiesS(5, {"v"});
    writePkb.setModifiesS(6, {"y"});
    writePkb.setModifiesS(7, {"v"});
    writePkb.setModifiesS(8, {"v"});
    writePkb.setModifiesS(9, {"x"});
    writePkb.setModifiesS(10, {"y"});
    writePkb.setModifiesS(11, {"x"});
    writePkb.setModifiesP("proc1", {"y", "x", "v"});

    writePkb.setUsesS(1, {"a", "b"});
    writePkb.setUsesS(2, {"v"});
    writePkb.setUsesS(3, {"x"});
    writePkb.setUsesS(4, {"x", "v", "y"});
    writePkb.setUsesS(5, {"y"});
    writePkb.setUsesS(6, {"x"});
    writePkb.setUsesS(7, {});
    writePkb.setUsesS(8, {"v"});
    writePkb.setUsesS(9, {"y"});
    writePkb.setUsesS(10, {});
    writePkb.setUsesS(11, {});
    writePkb.setUsesP("proc1", {"a", "b", "y", "x", "v"});


//    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> graph2 = {
//        {9, {{AppConstants::PARENTS, {11}}, {AppConstants::CHILDREN, {10}}}},
//        {10, {{AppConstants::PARENTS, {9}}, {AppConstants::CHILDREN, {11}}}},
//        {11, {{AppConstants::PARENTS, {10}}, {AppConstants::CHILDREN, {12, 9}}}},
//        {12, {{AppConstants::PARENTS, {10}}, {AppConstants::CHILDREN, {}}}}};
//
//    ProcName proc2 = "proc2";
//    writePkb.writeCFG(proc2, graph2);
//    writePkb.setStatement("while", 9);
//    writePkb.setStatement("assign", 10);
//    writePkb.setStatement("assign", 11);
//    writePkb.setStatement("print", 12);
//    writePkb.setProcedure(proc2, {9, 10, 11, 12});

}