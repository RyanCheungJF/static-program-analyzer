#include "../../../spa/src/PKB/ReadPKB.h"
#include "../../../spa/src/PKB/WritePKB.h"
#include "../../../spa/src/PKB/utils/utils.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace unit_testing_utils;

TEST_CASE("PatternStorage: fringe cases") {
    PatternWithExprStorage ps;

    SECTION("* getPatternWithLHS(std::string lhs): empty storage") {
        auto res = ps.getPatternWithLHS("x");
        REQUIRE(res == NULL);
    }

    std::string lhs = "z";
    std::unique_ptr<Expression> line1rhs = pkb_utils::buildSubtree("a + b / c");
    ps.writePattern(lhs, 1, std::move(line1rhs));

    SECTION("* getPatternWithLHS(std::string lhs): non-empty storage, LHS not present") {
        auto res = ps.getPatternWithLHS("x");
        REQUIRE(res == NULL);
    }
}

TEST_CASE("Support for pattern query of type pattern(\"a\", \"v\")") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    // line 1: z = a + b / c
    // line 2: z = z * 5

    std::string lhs = "z";
    std::unique_ptr<Expression> line1rhs = pkb_utils::buildSubtree("a + b / c");
    std::unique_ptr<Expression> line2rhs = pkb_utils::buildSubtree("z * 5");

    writePkb.writePattern(lhs, 1, std::move(line1rhs));
    writePkb.writePattern(lhs, 2, std::move(line2rhs));

    Parameter param1 = Parameter("a", AppConstants::ASSIGN);
    Parameter param2 = Parameter("z", AppConstants::FIXED_STRING);
    Pattern pattern1 = Pattern(param1, param2, "a + b / c");
    Pattern pattern2 = Pattern(param1, param2, "z * 5");

    std::vector<std::vector<std::string>> res1 = readPkb.findPattern(pattern1);
    std::vector<std::vector<std::string>> res2 = readPkb.findPattern(pattern2);

    std::vector<std::vector<std::string>> check1 = {{"1", "z"}};
    std::vector<std::vector<std::string>> check2 = {{"2", "z"}};
    ;
    REQUIRE(check1 == res1);
    REQUIRE(unit_testing_utils::equals(check1, res1));
    REQUIRE(unit_testing_utils::equals(check2, res2));
}

TEST_CASE("Support for pattern query of type pattern(\"a\", _\"v\"_)") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    // line 1: z = a + b / c
    // line 2: z = z * 5

    std::string lhs = "z";
    std::unique_ptr<Expression> line1rhs = pkb_utils::buildSubtree("a + b / c");
    std::unique_ptr<Expression> line2rhs = pkb_utils::buildSubtree("z * 5");

    writePkb.writePattern(lhs, 1, std::move(line1rhs));
    writePkb.writePattern(lhs, 2, std::move(line2rhs));

    Parameter param1 = Parameter("a", AppConstants::ASSIGN);
    Parameter param2 = Parameter("z", AppConstants::FIXED_STRING);
    Pattern pattern1 = Pattern(param1, param2, "_b / c_");
    Pattern pattern2 = Pattern(param1, param2, "_5_");

    std::vector<std::vector<std::string>> res1 = readPkb.findPattern(pattern1);
    std::vector<std::vector<std::string>> res2 = readPkb.findPattern(pattern2);

    std::vector<std::vector<std::string>> check1 = {{"1", "z"}};
    std::vector<std::vector<std::string>> check2 = {{"2", "z"}};

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

    // line 1: z = a + b / c
    // line 2: z = z * 5
    std::string lhs = "z";
    std::unique_ptr<Expression> line1rhs = pkb_utils::buildSubtree("a + b / c");
    std::unique_ptr<Expression> line2rhs = pkb_utils::buildSubtree("z * 5");

    writePkb.writePattern(lhs, 1, std::move(line1rhs));
    writePkb.writePattern(lhs, 2, std::move(line2rhs));

    Parameter param1 = Parameter("a", AppConstants::ASSIGN);
    Parameter param2 = Parameter("z", AppConstants::FIXED_STRING);
    Pattern pattern1 = Pattern(param1, param2, "_");

    std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern1);

    std::vector<std::vector<std::string>> check = {{"1", "z"}, {"2", "z"}};
    REQUIRE(unit_testing_utils::equals(check, res));
}

TEST_CASE("Support for pattern query of type pattern(_, \"v\")") {

    WritePKB writePkb;
    ReadPKB readPkb;
    PKB pkb;
    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);
    readPkb.setInstancePKB(pkb);

    // line 1: z = a + b / c
    // line 2: z = z * 5

    std::string lhs = "z";
    std::unique_ptr<Expression> line1rhs = pkb_utils::buildSubtree("a + b / c");
    std::unique_ptr<Expression> line2rhs = pkb_utils::buildSubtree("z * 5");

    writePkb.writePattern(lhs, 1, std::move(line1rhs));
    writePkb.writePattern(lhs, 2, std::move(line2rhs));

    Parameter param1 = Parameter("a", AppConstants::ASSIGN);
    Parameter param2 = Parameter("_", AppConstants::WILDCARD);
    Pattern pattern1 = Pattern(param1, param2, "a + b / c");
    Pattern pattern2 = Pattern(param1, param2, "z * 5");
    Pattern pattern3 = Pattern(param1, param2, "z");

    std::vector<std::vector<std::string>> res1 = readPkb.findPattern(pattern1);
    std::vector<std::vector<std::string>> res2 = readPkb.findPattern(pattern2);
    std::vector<std::vector<std::string>> res3 = readPkb.findPattern(pattern3);

    std::vector<std::vector<std::string>> check1 = {{"1", "z"}};
    std::vector<std::vector<std::string>> check2 = {{"2", "z"}};

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

    // line 1: z = a + b / c
    // line 2: z = z * 5

    std::string lhs = "z";
    std::unique_ptr<Expression> line1rhs = pkb_utils::buildSubtree("a + b / c");
    std::unique_ptr<Expression> line2rhs = pkb_utils::buildSubtree("z * 5");

    writePkb.writePattern(lhs, 1, std::move(line1rhs));
    writePkb.writePattern(lhs, 2, std::move(line2rhs));

    Parameter param1 = Parameter("a", AppConstants::ASSIGN);
    Parameter param2 = Parameter("v", AppConstants::VARIABLE);
    Pattern pattern = Pattern(param1, param2, "_");

    std::vector<std::vector<std::string>> res = readPkb.findPattern(pattern);
    std::vector<std::vector<std::string>> check = {{"1", "z"}, {"2", "z"}};

    REQUIRE(unit_testing_utils::equals(check, res));
}
