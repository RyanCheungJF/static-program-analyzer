#include "../../../spa/src/PKB/PKB.h"
#include "../utils/utils.h"
#include "catch.hpp"

using namespace std;
using namespace unit_testing_utils;

TEST_CASE("Test Relationship Cache") {
    RelationshipCache cache;

    // Follows(1, 2)
    Parameter param1 = Parameter("1", ParameterType::FIXED_INT);
    Parameter param2 = Parameter("2", ParameterType::FIXED_INT);
    vector<Parameter> params1;
    params1.push_back(param1);
    params1.push_back(param2);
    shared_ptr<Relationship> rs1 = Relationship::makeRelationship(AppConstants::FOLLOWS, params1);

    vector<vector<string>> cacheValue = {{"1", "2"}};
    cache.addResult(rs1, cacheValue);

    // Modifies("x", y)
    param1 = Parameter("x", ParameterType::FIXED_STRING);
    param2 = Parameter("y", ParameterType::VARIABLE);
    params1 = vector<Parameter>();
    params1.push_back(param1);
    params1.push_back(param2);
    rs1 = Relationship::makeRelationship(AppConstants::MODIFIES, params1);

    cacheValue = {{"x", "a"}, {"x", "b"}};
    cache.addResult(rs1, cacheValue);

    SECTION("Cache hit") {
        SECTION("Follows(1, 2)") {
            Parameter param3 = Parameter("1", ParameterType::FIXED_INT);
            Parameter param4 = Parameter("2", ParameterType::FIXED_INT);
            vector<Parameter> params2;
            params2.push_back(param3);
            params2.push_back(param4);
            shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::FOLLOWS, params2);

            vector<vector<string>> res = cache.findResult(rs2);
            vector<vector<string>> expected = {{"1", "2"}};
            REQUIRE(equals(res, expected));
        }

        SECTION("Modifies(\"x\", y1)") {
            Parameter param3 = Parameter("x", ParameterType::FIXED_STRING);
            Parameter param4 = Parameter("y1", ParameterType::VARIABLE);
            vector<Parameter> params2;
            params2.push_back(param3);
            params2.push_back(param4);
            shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

            vector<vector<string>> res = cache.findResult(rs2);
            vector<vector<string>> expected = {{"x", "a"}, {"x", "b"}};
            REQUIRE(equals(res, expected));
        }
    }

    // Follows(1, 3)
    SECTION("Cache miss") {
        SECTION("Follows(1, 3)") {
            Parameter param3 = Parameter("1", ParameterType::FIXED_INT);
            Parameter param4 = Parameter("3", ParameterType::FIXED_INT);
            vector<Parameter> params2;
            params2.push_back(param3);
            params2.push_back(param4);
            shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::FOLLOWS, params2);

            vector<vector<string>> res = cache.findResult(rs2);
            vector<vector<string>> expected = {};
            REQUIRE(equals(res, expected));
        }

        SECTION("Modifies(\"x\", \"y\")") {
            Parameter param3 = Parameter("x", ParameterType::FIXED_STRING);
            Parameter param4 = Parameter("y", ParameterType::FIXED_STRING);
            vector<Parameter> params2;
            params2.push_back(param3);
            params2.push_back(param4);
            shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

            vector<vector<string>> res = cache.findResult(rs2);
            vector<vector<string>> expected = {};
            REQUIRE(equals(res, expected));
        }

        SECTION("Modifies(\"x1\", y)") {
            Parameter param3 = Parameter("x1", ParameterType::FIXED_STRING);
            Parameter param4 = Parameter("y", ParameterType::VARIABLE);
            vector<Parameter> params2;
            params2.push_back(param3);
            params2.push_back(param4);
            shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

            vector<vector<string>> res = cache.findResult(rs2);
            vector<vector<string>> expected = {};
            REQUIRE(equals(res, expected));
        }

        SECTION("Modifies(x, y1)") {
            Parameter param3 = Parameter("x", ParameterType::PROCEDURE);
            Parameter param4 = Parameter("y", ParameterType::VARIABLE);
            vector<Parameter> params2;
            params2.push_back(param3);
            params2.push_back(param4);
            shared_ptr<Relationship> rs2 = Relationship::makeRelationship(AppConstants::MODIFIES, params2);

            vector<vector<string>> res = cache.findResult(rs2);
            vector<vector<string>> expected = {};
            REQUIRE(equals(res, expected));
        }
    }
}

TEST_CASE("Test Parameter Cache") {
    ParameterCache cache;

    // variable v;
    // Select v
    Parameter p1 = Parameter("v", ParameterType::VARIABLE);
    shared_ptr<Parameter> param1 = make_shared<Parameter>(p1);

    vector<string> cacheValue = {"x", "y"};
    cache.addResult(param1, cacheValue);

    // variable x;
    // Select x
    SECTION("Cache hit") {
        Parameter p2 = Parameter("x", ParameterType::VARIABLE);
        shared_ptr<Parameter> param2 = make_shared<Parameter>(p2);

        vector<string> res = cache.findResult(param2);
        vector<string> expected = {"x", "y"};
        REQUIRE(equals(res, expected));
    }

    // constant v;
    // Select v
    SECTION("Cache miss") {
        Parameter p2 = Parameter("v", ParameterType::CONSTANT);
        shared_ptr<Parameter> param2 = make_shared<Parameter>(p2);

        vector<string> res = cache.findResult(param2);
        vector<string> expected = {};
        REQUIRE(equals(res, expected));
    }
}

TEST_CASE("Test Pattern Cache") {
    PatternCache cache;

    // Pattern a("z", "_b / c_")
    Parameter param1 = Parameter("a", ParameterType::ASSIGN);
    Parameter param2 = Parameter("z", ParameterType::FIXED_STRING);
    vector<string> exprSpecs = {"_b / c_"};
    Pattern p1 = Pattern(param1, param2, exprSpecs);

    // Pattern if(v, _, _)
    Parameter param3 = Parameter("if", ParameterType::IF);
    Parameter param4 = Parameter("v", ParameterType::VARIABLE);
    exprSpecs = {"_", "_"};
    Pattern p2 = Pattern(param3, param4, exprSpecs);

    shared_ptr<Pattern> pattern1 = make_shared<Pattern>(p1);
    shared_ptr<Pattern> pattern2 = make_shared<Pattern>(p2);

    vector<vector<string>> cacheValue1 = {{"1", "z"}};
    vector<vector<string>> cacheValue2 = {{"2", "z"}};

    cache.addResult(pattern1, cacheValue1);
    cache.addResult(pattern2, cacheValue2);

    // Pattern a("z", "_b / c_")
    SECTION("Cache hit, Assign pattern") {
        Parameter param6 = Parameter("a2", ParameterType::ASSIGN);
        Parameter param7 = Parameter("z", ParameterType::FIXED_STRING);
        vector<string> exprSpecs = {"_b / c_"};
        Pattern p3 = Pattern(param6, param7, exprSpecs);
        shared_ptr<Pattern> pattern3 = make_shared<Pattern>(p3);

        vector<vector<string>> res = cache.findResult(pattern3);
        vector<vector<string>> expected = {{"1", "z"}};
        REQUIRE(equals(res, expected));
    }

    // Pattern if(v2, _, _)
    SECTION("Cache hit, If pattern") {
        Parameter param6 = Parameter("if", ParameterType::IF);
        Parameter param7 = Parameter("v2", ParameterType::VARIABLE);
        vector<string> exprSpecs = {"_", "_"};
        Pattern p3 = Pattern(param6, param7, exprSpecs);
        shared_ptr<Pattern> pattern3 = make_shared<Pattern>(p3);

        vector<vector<string>> res = cache.findResult(pattern3);
        vector<vector<string>> expected = {{"2", "z"}};
        REQUIRE(equals(res, expected));
    }

    SECTION("Cache miss, Pattern a(\"z\", \"b / c\")") {
        Parameter param6 = Parameter("a", ParameterType::ASSIGN);
        Parameter param7 = Parameter("z", ParameterType::FIXED_STRING);
        vector<string> exprSpecs = {"b / c"};
        Pattern p3 = Pattern(param6, param7, exprSpecs);
        shared_ptr<Pattern> pattern3 = make_shared<Pattern>(p3);

        vector<vector<string>> res = cache.findResult(pattern3);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }

    SECTION("Cache miss, Pattern a(z, \"_b / c_\")") {
        Parameter param6 = Parameter("a", ParameterType::ASSIGN);
        Parameter param7 = Parameter("z", ParameterType::VARIABLE);
        vector<string> exprSpecs = {"_b / c_"};
        Pattern p3 = Pattern(param6, param7, exprSpecs);
        shared_ptr<Pattern> pattern3 = make_shared<Pattern>(p3);

        vector<vector<string>> res = cache.findResult(pattern3);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }

    // Pattern while("z", _)
    SECTION("Cache miss, While pattern") {
        Parameter param6 = Parameter("while", ParameterType::WHILE);
        Parameter param7 = Parameter("z", ParameterType::FIXED_STRING);
        vector<string> exprSpecs = {"_"};
        Pattern p3 = Pattern(param6, param7, exprSpecs);
        shared_ptr<Pattern> pattern3 = make_shared<Pattern>(p3);

        vector<vector<string>> res = cache.findResult(pattern3);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }

    // Pattern if("v", _, _)
    SECTION("Cache miss, If pattern") {
        Parameter param6 = Parameter("if", ParameterType::IF);
        Parameter param7 = Parameter("v", ParameterType::FIXED_STRING);
        vector<string> exprSpecs = {"_", "_"};
        Pattern p3 = Pattern(param6, param7, exprSpecs);
        shared_ptr<Pattern> pattern3 = make_shared<Pattern>(p3);

        vector<vector<string>> res = cache.findResult(pattern3);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }
}

TEST_CASE("Test Attribute Cache") {
    AttributeCache cache;

    // call c;
    // Select c.procName
    Parameter p1 = Parameter("c", ParameterType::CALL, AttributeType::PROCNAME);
    shared_ptr<Parameter> param1 = make_shared<Parameter>(p1);

    vector<vector<string>> cacheValue = {{"1", "main"}};
    cache.addResult(param1, cacheValue);

    // variable v;
    // Select v.varName
    Parameter p2 = Parameter("v", ParameterType::VARIABLE, AttributeType::VARNAME);
    shared_ptr<Parameter> param2 = make_shared<Parameter>(p2);

    cacheValue = {{"var", "var"}};
    cache.addResult(param2, cacheValue);
    
    SECTION("Cache Hit") {
        // call c;
        // Select c.procName
        SECTION("find c.procName") {
            Parameter p3 = Parameter("c", ParameterType::CALL, AttributeType::PROCNAME);
            shared_ptr<Parameter> param3 = make_shared<Parameter>(p3);

            vector<vector<string>> res = cache.findResult(param3);
            vector<vector<string>> expected = {{"1", "main"}};
            REQUIRE(equals(res, expected));
        }

        // call c1;
        // Select c1.procName
        SECTION("find c1.procName") {
            Parameter p3 = Parameter("c1", ParameterType::CALL, AttributeType::PROCNAME);
            shared_ptr<Parameter> param3 = make_shared<Parameter>(p3);

            vector<vector<string>> res = cache.findResult(param3);
            vector<vector<string>> expected = {{"1", "main"}};
            REQUIRE(equals(res, expected));
        }

        // variable v1;
        // Select v1.varName
        SECTION("find c1.procName") {
            Parameter p3 = Parameter("v1", ParameterType::VARIABLE, AttributeType::VARNAME);
            shared_ptr<Parameter> param3 = make_shared<Parameter>(p3);

            vector<vector<string>> res = cache.findResult(param3);
            vector<vector<string>> expected = {{"var", "var"}};
            REQUIRE(equals(res, expected));
        }
    }

    SECTION("Cache Miss") {
        // call c;
        // Select c.stmt#
        SECTION("find c.stmt#") {
            Parameter p3 = Parameter("c", ParameterType::CALL, AttributeType::STMTNO);
            shared_ptr<Parameter> param3 = make_shared<Parameter>(p3);

            vector<vector<string>> res = cache.findResult(param3);
            REQUIRE(res.empty());
        }

        // print c;
        // Select c.varName
        SECTION("find c.varName") {
            Parameter p3 = Parameter("c", ParameterType::PRINT, AttributeType::VARNAME);
            shared_ptr<Parameter> param3 = make_shared<Parameter>(p3);

            vector<vector<string>> res = cache.findResult(param3);
            REQUIRE(res.empty());
        }

        // constant c;
        // Select c.value
        SECTION("find c.value") {
            Parameter p3 = Parameter("c", ParameterType::CONSTANT, AttributeType::VALUE);
            shared_ptr<Parameter> param3 = make_shared<Parameter>(p3);

            vector<vector<string>> res = cache.findResult(param3);
            REQUIRE(res.empty());
        }
    }
}
