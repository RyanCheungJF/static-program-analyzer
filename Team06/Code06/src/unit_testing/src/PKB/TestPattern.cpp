#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/utils/utils.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;
using namespace std;

TEST_CASE("Check Pattern Storage Write correctness") {
    PatternStorage ps;
    ps.writePattern(1, {"x", "y"});

    SECTION("Check Writes do not override existing values") {
        ps.writePattern(2, {"y"});

        unordered_set<StmtNum> res = ps.getStmtNums("y");

        unordered_set<StmtNum> expected = {1, 2};
        REQUIRE(res.size() == 2);
        REQUIRE(equals(res, expected));
    }

    SECTION("Check that writing existing values do not cause duplicates") {
        ps.writePattern(1, {"x", "y"});

        unordered_set<StmtNum> res = ps.getStmtNums("y");

        unordered_set<StmtNum> expected = {1};
        REQUIRE(res.size() == 1);
        REQUIRE(equals(res, expected));
    }
}

TEST_CASE("Check Pattern Storage Write and non-empty Gets") {
    PatternStorage ps;
    ps.writePattern(1, {"x", "y"});
    ps.writePattern(2, {"y", "z"});
    ps.writePattern(3, {"z"});

    SECTION("Get statement numbers of existing pattern") {
        unordered_set<StmtNum> res = ps.getStmtNums("y");
        unordered_set<StmtNum> expected = {1, 2};
        REQUIRE(res.size() == 2);
        REQUIRE(equals(res, expected));
    }

    SECTION("Get all statement number - variable pairs") {
        pair<vector<StmtNum>, vector<Ent>> res = ps.getAllPairs();
        vector<StmtNum> stmtNums = res.first;
        vector<Ent> vars = res.second;

        vector<StmtNum> expectedStmtNums = {1, 1, 2, 2, 3};
        REQUIRE(stmtNums.size() == 5);
        REQUIRE(equals(stmtNums, expectedStmtNums));

        vector<Ent> expectedVars = {"x", "y", "y", "z", "z"};
        REQUIRE(res.second.size() == 5);
        REQUIRE(equals(vars, expectedVars));
    }
}

TEST_CASE("Check Pattern Storage Write and empty Gets") {
    PatternStorage ps;
    ps.writePattern(1, {"x", "y"});
    ps.writePattern(2, {"y", "z"});
    ps.writePattern(3, {"z"});

    SECTION("Get statement numbers of non-existing pattern") {
        unordered_set<StmtNum> res = ps.getStmtNums("non-existing");
        REQUIRE(res.size() == 0);
    }
}

TEST_CASE("Test If/While Pattern on WritePKB and ReadPKB") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    writePkb.setIfPattern(1, {"x", "y", "z"});
    writePkb.setIfPattern(2, {"z"});

    writePkb.setWhilePattern(1, {"x", "y", "z"});
    writePkb.setWhilePattern(2, {"z"});

    SECTION("if(\"x\", _, _)") {
        Parameter param1 = Parameter("if", AppConstants::IF);
        Parameter param2 = Parameter("z", AppConstants::FIXED_STRING);
        vector<string> exprSpecs = {"_", "_"};
        Pattern pattern = Pattern(param1, param2, exprSpecs);
        std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern);
        std::vector<std::vector<std::string>> expected = {{"1", "z"}, {"2", "z"}};
        REQUIRE(res.size() == 2);
        REQUIRE(equals(res, expected));
    }
    SECTION("while(v, _)") {
        Parameter param1 = Parameter("while", AppConstants::WHILE);
        Parameter param2 = Parameter("v", AppConstants::VARIABLE);
        vector<string> exprSpecs = {"_"};
        Pattern pattern = Pattern(param1, param2, exprSpecs);

        std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern);
        std::vector<std::vector<std::string>> expected = {{"1", "x"}, {"1", "y"}, {"1", "z"}, {"2", "z"}};
        REQUIRE(res.size() == 4);
        REQUIRE(equals(res, expected));
    }

    SECTION("if(_, _, _)") {
        Parameter param1 = Parameter("if", AppConstants::IF);
        Parameter param2 = Parameter("_", AppConstants::WILDCARD);
        vector<string> exprSpecs = {"_", "_"};
        Pattern pattern = Pattern(param1, param2, exprSpecs);

        std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern);
        std::vector<std::vector<std::string>> expected = {{"1", "x"}, {"1", "y"}, {"1", "z"}, {"2", "z"}};
        REQUIRE(res.size() == 4);
        REQUIRE(equals(res, expected));
    }

    SECTION("while(\"non-exist\", _, _)") {
        Parameter param1 = Parameter("while", AppConstants::WHILE);
        Parameter param2 = Parameter("non-exist", AppConstants::FIXED_STRING);
        vector<string> exprSpecs = {"_", "_"};
        Pattern pattern = Pattern(param1, param2, exprSpecs);

        std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern);
        REQUIRE(res.size() == 0);
    }
}


TEST_CASE("Support for pattern query of type pattern(\"a\", _\"v\"_)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    //line 1: z = a + b / c
    //line 2: z = z * 5

    std::string lhs = "z";
    std::unique_ptr<Expression> line1rhs = pkb_utils::buildSubtree("a + b / c");
    std::unique_ptr<Expression> line2rhs = pkb_utils::buildSubtree("z * 5");

    writePkb.writePattern(lhs, 1, std::move(line1rhs));
    writePkb.writePattern(lhs, 2, std::move(line2rhs));

    Parameter param1;
    Parameter param2 = Parameter("z", AppConstants::FIXED_STRING);
    vector<string> exprSpecs1 = {"_b / c_"};
    vector<string> exprSpecs2 = {"_5_"};
    Pattern pattern1 = Pattern(param1, param2, exprSpecs1);
    Pattern pattern2 = Pattern(param1, param2, exprSpecs2);

    std::vector<std::vector<std::string>> res1 = readPkb.findPattern(pattern1);
    std::vector<std::vector<std::string>> res2 = readPkb.findPattern(pattern2);

    std::vector<std::vector<std::string>> check1 = { {"1", "z"} };
    std::vector<std::vector<std::string>> check2 = { {"2", "z"} };

    REQUIRE(unit_testing_utils::equals(check1, res1));
    REQUIRE(unit_testing_utils::equals(check2, res2));
    REQUIRE(readPkb.findPattern(pattern2).size() == 1);
}


TEST_CASE("Support for pattern query of type pattern(\"a\", _") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    //line 1: z = a + b / c
    //line 2: z = z * 5
    std::string lhs = "z";
    std::unique_ptr<Expression> line1rhs = pkb_utils::buildSubtree("a + b / c");
    std::unique_ptr<Expression> line2rhs = pkb_utils::buildSubtree("z * 5");

    writePkb.writePattern(lhs, 1, std::move(line1rhs));
    writePkb.writePattern(lhs, 2, std::move(line2rhs));
   
    Parameter param1;
    Parameter param2 = Parameter("z", AppConstants::FIXED_STRING);
    vector<string> exprSpecs = {"_"};
    Pattern pattern1 = Pattern(param1, param2, exprSpecs);

    std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern1);

    std::vector<std::vector<std::string>> check = { {"1", "z"}, {"2", "z"} };
    REQUIRE(unit_testing_utils::equals(check, res));
}

TEST_CASE("Support for pattern query of type pattern(_, \"v\")") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    //line 1: z = a + b / c
    //line 2: z = z * 5

    std::string lhs = "z";
    std::unique_ptr<Expression> line1rhs = pkb_utils::buildSubtree("a + b / c");
    std::unique_ptr<Expression> line2rhs = pkb_utils::buildSubtree("z * 5");

    writePkb.writePattern(lhs, 1, std::move(line1rhs));
    writePkb.writePattern(lhs, 2, std::move(line2rhs));

    Parameter param1;
    Parameter param2 = Parameter("_", AppConstants::WILDCARD);
    vector<string> exprSpecs1 = {"a + b / c"};
    vector<string> exprSpecs2 = {"z * 5"};
    vector<string> exprSpecs3 = {"z"};
    Pattern pattern1 = Pattern(param1, param2, exprSpecs1);
    Pattern pattern2 = Pattern(param1, param2, exprSpecs2);
    Pattern pattern3 = Pattern(param1, param2, exprSpecs3);

    std::vector<std::vector<std::string>> res1 = readPkb.findPattern(pattern1);
    std::vector<std::vector<std::string>> res2 = readPkb.findPattern(pattern2);
    std::vector<std::vector<std::string>> res3 = readPkb.findPattern(pattern3);


    std::vector<std::vector<std::string>> check1 = { {"1", "z"} };
    std::vector<std::vector<std::string>> check2 = { {"2", "z"} };
    
    REQUIRE(unit_testing_utils::equals(check1, res1));
    REQUIRE(unit_testing_utils::equals(check2, res2));
    REQUIRE(res3.empty());
}

TEST_CASE("Support for Select v pattern a (v, _)\'") {


    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    //line 1: z = a + b / c
    //line 2: z = z * 5

    std::string lhs = "z";
    std::unique_ptr<Expression> line1rhs = pkb_utils::buildSubtree("a + b / c");
    std::unique_ptr<Expression> line2rhs = pkb_utils::buildSubtree("z * 5");

    writePkb.writePattern(lhs, 1, std::move(line1rhs));
    writePkb.writePattern(lhs, 2, std::move(line2rhs));

    Parameter param1;
    Parameter param2 = Parameter("v", AppConstants::VARIABLE);
    vector<string> exprSpecs = {"_"};
    Pattern pattern = Pattern(param1, param2, exprSpecs);

    std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern);
    std::vector<std::vector<std::string>> check = { {"1", "z"}, {"2", "z"} };

    REQUIRE(unit_testing_utils::equals(check, res));
}



