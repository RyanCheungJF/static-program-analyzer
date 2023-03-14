#include "../../../spa/src/PKB/PKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace std;
using namespace unit_testing_utils;

TEST_CASE("Test Relationship Cache") {
    RelationshipCache cache;

    // Follows(1, 2)
    Parameter param1 = Parameter("1", AppConstants::FIXED_INT);
    Parameter param2 = Parameter("2", AppConstants::FIXED_INT);
    vector<Parameter> params1;
    params1.push_back(param1);
    params1.push_back(param2);
    shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::FOLLOWS, params1);

    cache.addResult(rs1, {{"1", "2"}});

    // Follows(1, 2)
    SECTION("Cache hit") {
        Parameter param3 = Parameter("1", AppConstants::FIXED_INT);
        Parameter param4 = Parameter("2", AppConstants::FIXED_INT);
        vector<Parameter> params2;
        params2.push_back(param3);
        params2.push_back(param4);
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::FOLLOWS, params2);

        vector<vector<string>> res = cache.findResult(rs2);
        vector<vector<string>> expected = {{"1", "2"}};
        REQUIRE(equals(res, expected));
    }

    // Follows(1, 3)
    SECTION("Cache miss") {
        Parameter param3 = Parameter("1", AppConstants::FIXED_INT);
        Parameter param4 = Parameter("3", AppConstants::FIXED_INT);
        vector<Parameter> params2;
        params2.push_back(param3);
        params2.push_back(param4);
        shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::FOLLOWS, params2);

        vector<vector<string>> res = cache.findResult(rs2);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }
}

TEST_CASE("Test Parameter Cache") {
    ParameterCache cache;

    // variable v;
    // Select v
    Parameter p1 = Parameter("v", AppConstants::VARIABLE);
    shared_ptr<Parameter> param1 = make_shared<Parameter>(p1);

    cache.addResult(param1, {"x", "y"});

    // variable x;
    // Select x
    SECTION("Cache hit") {
        Parameter p2 = Parameter("x", AppConstants::VARIABLE);
        shared_ptr<Parameter> param2 = make_shared<Parameter>(p2);

        vector<string> res = cache.findResult(param2);
        vector<string> expected = {"x", "y"};
        REQUIRE(equals(res, expected));
    }

    // constant v;
    // Select v
    SECTION("Cache miss") {
        Parameter p2 = Parameter("v", AppConstants::CONSTANT);
        shared_ptr<Parameter> param2 = make_shared<Parameter>(p2);

        vector<string> res = cache.findResult(param2);
        vector<string> expected = {};
        REQUIRE(equals(res, expected));
    }
}

TEST_CASE("Test Pattern Cache") {
    PatternCache cache;

    // Pattern a("z", "_b / c_")
    Parameter param1 = Parameter("a", AppConstants::ASSIGN);
    Parameter param2 = Parameter("z", AppConstants::FIXED_STRING);
    Pattern p1 = Pattern(param1, param2, "_b / c_");

    // Pattern if("z", _, _)
    Parameter param3 = Parameter("if", AppConstants::IF);
    Parameter param4 = Parameter("z", AppConstants::FIXED_STRING);
    Parameter param5 = Parameter("_", AppConstants::WILDCARD);
    Pattern p2 = Pattern(param3, param4, "_", param5);

    shared_ptr<Pattern> pattern1 = make_shared<Pattern>(p1);
    shared_ptr<Pattern> pattern2 = make_shared<Pattern>(p2);

    cache.addResult(pattern1, {{"1", "z"}});
    cache.addResult(pattern2, {{"2", "z"}});

    // Pattern a("z", "_b / c_")
    SECTION("Cache hit, Assign pattern") {
        Parameter param6 = Parameter("a", AppConstants::ASSIGN);
        Parameter param7 = Parameter("z", AppConstants::FIXED_STRING);
        Pattern p3 = Pattern(param6, param7, "_b / c_");
        shared_ptr<Pattern> pattern3 = make_shared<Pattern>(p3);
        

        vector<vector<string>> res = cache.findResult(pattern3);
        vector<vector<string>> expected = {{"1", "z"}};
        REQUIRE(equals(res, expected));
    }

    // Pattern if("z", _, _)
    SECTION("Cache hit, If pattern") {
        Parameter param6 = Parameter("if", AppConstants::IF);
        Parameter param7 = Parameter("z", AppConstants::FIXED_STRING);
        Parameter param8 = Parameter("_", AppConstants::WILDCARD);
        Pattern p3 = Pattern(param6, param7, "_", param8);
        shared_ptr<Pattern> pattern3 = make_shared<Pattern>(p3);

        vector<vector<string>> res = cache.findResult(pattern3);
        vector<vector<string>> expected = {{"2", "z"}};
        REQUIRE(equals(res, expected));
    }

    // Pattern a("z", "b / c")
    SECTION("Cache miss, Assign pattern") {
        Parameter param6 = Parameter("a", AppConstants::ASSIGN);
        Parameter param7 = Parameter("z", AppConstants::FIXED_STRING);
        Pattern p3 = Pattern(param6, param7, "b / c");
        shared_ptr<Pattern> pattern3 = make_shared<Pattern>(p3);

        vector<vector<string>> res = cache.findResult(pattern3);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }

    // Pattern while("z", _)
    SECTION("Cache miss, Assign pattern") {
        Parameter param6 = Parameter("while", AppConstants::WHILE);
        Parameter param7 = Parameter("z", AppConstants::FIXED_STRING);
        Pattern p3 = Pattern(param6, param7, "_");
        shared_ptr<Pattern> pattern3 = make_shared<Pattern>(p3);

        vector<vector<string>> res = cache.findResult(pattern3);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }
}
