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

    vector<string> cacheValue = {"x", "y"};
    cache.addResult(p1, cacheValue);

    // variable x;
    // Select x
    SECTION("Cache hit") {
        Parameter p2 = Parameter("x", ParameterType::VARIABLE);

        vector<string> res = cache.findResult(p2);
        vector<string> expected = {"x", "y"};
        REQUIRE(equals(res, expected));
    }

    // constant v;
    // Select v
    SECTION("Cache miss") {
        Parameter p2 = Parameter("v", ParameterType::CONSTANT);

        vector<string> res = cache.findResult(p2);
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

    vector<vector<string>> cacheValue1 = {{"1", "z"}};
    vector<vector<string>> cacheValue2 = {{"2", "z"}};

    cache.addResult(p1, cacheValue1);
    cache.addResult(p2, cacheValue2);

    // Pattern a("z", "_b / c_")
    SECTION("Cache hit, Assign pattern") {
        Parameter param6 = Parameter("a2", ParameterType::ASSIGN);
        Parameter param7 = Parameter("z", ParameterType::FIXED_STRING);
        vector<string> queryExprSpecs = {"_b / c_"};
        Pattern p3 = Pattern(param6, param7, queryExprSpecs);

        vector<vector<string>> res = cache.findResult(p3);
        vector<vector<string>> expected = {{"1", "z"}};
        REQUIRE(equals(res, expected));
    }

    // Pattern if(v2, _, _)
    SECTION("Cache hit, If pattern") {
        Parameter param6 = Parameter("if", ParameterType::IF);
        Parameter param7 = Parameter("v2", ParameterType::VARIABLE);
        vector<string> queryExprSpecs = {"_", "_"};
        Pattern p3 = Pattern(param6, param7, queryExprSpecs);

        vector<vector<string>> res = cache.findResult(p3);
        vector<vector<string>> expected = {{"2", "z"}};
        REQUIRE(equals(res, expected));
    }

    // pattern a("z", "b / c")
    SECTION("Cache miss, Pattern a(\"z\", \"b / c\")") {
        Parameter param6 = Parameter("a", ParameterType::ASSIGN);
        Parameter param7 = Parameter("z", ParameterType::FIXED_STRING);
        vector<string> queryExprSpecs = {"b / c"};
        Pattern p3 = Pattern(param6, param7, queryExprSpecs);

        vector<vector<string>> res = cache.findResult(p3);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }

    // pattern a(z, "_b / c_")
    SECTION("Cache miss, Pattern a(z, \"_b / c_\")") {
        Parameter param6 = Parameter("a", ParameterType::ASSIGN);
        Parameter param7 = Parameter("z", ParameterType::VARIABLE);
        vector<string> queryExprSpecs = {"_b / c_"};
        Pattern p3 = Pattern(param6, param7, queryExprSpecs);

        vector<vector<string>> res = cache.findResult(p3);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }

    // pattern a("y", "_b / c_")
    SECTION("Cache miss, Pattern a(\"y\", \"_b / c_\")") {
        Parameter param6 = Parameter("a", ParameterType::ASSIGN);
        Parameter param7 = Parameter("y", ParameterType::FIXED_STRING);
        vector<string> queryExprSpecs = {"_b / c_"};
        Pattern p3 = Pattern(param6, param7, queryExprSpecs);

        vector<vector<string>> res = cache.findResult(p3);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }

    // Pattern while("z", _)
    SECTION("Cache miss, While pattern") {
        Parameter param6 = Parameter("while", ParameterType::WHILE);
        Parameter param7 = Parameter("z", ParameterType::FIXED_STRING);
        vector<string> queryExprSpecs = {"_"};
        Pattern p3 = Pattern(param6, param7, queryExprSpecs);

        vector<vector<string>> res = cache.findResult(p3);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }

    // Pattern if("v", _, _)
    SECTION("Cache miss, If pattern") {
        Parameter param6 = Parameter("if", ParameterType::IF);
        Parameter param7 = Parameter("v", ParameterType::FIXED_STRING);
        vector<string> queryExprSpecs = {"_", "_"};
        Pattern p3 = Pattern(param6, param7, queryExprSpecs);

        vector<vector<string>> res = cache.findResult(p3);
        vector<vector<string>> expected = {};
        REQUIRE(equals(res, expected));
    }
}

TEST_CASE("Test Attribute Cache") {
    AttributeCache cache;

    // call c;
    // Select c.procName
    Parameter p1 = Parameter("c", ParameterType::CALL, AttributeType::PROCNAME);

    vector<vector<string>> cacheValue = {{"1", "main"}};
    cache.addResult(p1, cacheValue);

    // variable v;
    // Select v.varName
    Parameter p2 = Parameter("v", ParameterType::VARIABLE, AttributeType::VARNAME);

    cacheValue = {{"var", "var"}};
    cache.addResult(p2, cacheValue);

    SECTION("Cache Hit") {
        // call c;
        // Select c.procName
        SECTION("find c.procName") {
            Parameter p3 = Parameter("c", ParameterType::CALL, AttributeType::PROCNAME);

            vector<vector<string>> res = cache.findResult(p3);
            vector<vector<string>> expected = {{"1", "main"}};
            REQUIRE(equals(res, expected));
        }

        // call c1;
        // Select c1.procName
        SECTION("find c1.procName") {
            Parameter p3 = Parameter("c1", ParameterType::CALL, AttributeType::PROCNAME);

            vector<vector<string>> res = cache.findResult(p3);
            vector<vector<string>> expected = {{"1", "main"}};
            REQUIRE(equals(res, expected));
        }

        // variable v1;
        // Select v1.varName
        SECTION("find c1.procName") {
            Parameter p3 = Parameter("v1", ParameterType::VARIABLE, AttributeType::VARNAME);

            vector<vector<string>> res = cache.findResult(p3);
            vector<vector<string>> expected = {{"var", "var"}};
            REQUIRE(equals(res, expected));
        }
    }

    SECTION("Cache Miss") {
        // call c;
        // Select c.stmt#
        SECTION("find c.stmt#") {
            Parameter p3 = Parameter("c", ParameterType::CALL, AttributeType::STMTNO);

            vector<vector<string>> res = cache.findResult(p3);
            REQUIRE(res.empty());
        }

        // print c;
        // Select c.varName
        SECTION("find c.varName") {
            Parameter p3 = Parameter("c", ParameterType::PRINT, AttributeType::VARNAME);

            vector<vector<string>> res = cache.findResult(p3);
            REQUIRE(res.empty());
        }

        // constant c;
        // Select c.value
        SECTION("find c.value") {
            Parameter p3 = Parameter("c", ParameterType::CONSTANT, AttributeType::VALUE);

            vector<vector<string>> res = cache.findResult(p3);
            REQUIRE(res.empty());
        }
    }
}

TEST_CASE("Test Comparison Cache") {
    ComparisonCache cache;
    // with "1" = "1"
    Parameter param1 = Parameter("1", ParameterType::FIXED_INT, AttributeType::NONE);
    Parameter param2 = Parameter("1", ParameterType::FIXED_INT, AttributeType::NONE);
    Comparison c1 = Comparison(ComparisonOperator::EQUALS, param1, param2);

    vector<vector<string>> cacheValue = {{"1", "1"}};
    cache.addResult(c1, cacheValue);

    // with pn.stmt# = "1"
    Parameter param5 = Parameter("pn", ParameterType::PRINT, AttributeType::STMTNO);
    Parameter param6 = Parameter("1", ParameterType::FIXED_INT, AttributeType::NONE);
    Comparison c2 = Comparison(ComparisonOperator::EQUALS, param5, param6);

    cacheValue = {{"1", "1"}};
    cache.addResult(c2, cacheValue);

    // with "var" = r.varName
    Parameter param7 = Parameter("var", ParameterType::FIXED_STRING, AttributeType::NONE);
    Parameter param8 = Parameter("r", ParameterType::READ, AttributeType::VARNAME);
    Comparison c3 = Comparison(ComparisonOperator::EQUALS, param7, param8);

    cacheValue = {{"var", "2"}};
    cache.addResult(c3, cacheValue);

    // with pn.varName = v.varName
    Parameter param9 = Parameter("pn", ParameterType::PRINT, AttributeType::VARNAME);
    Parameter param10 = Parameter("v", ParameterType::VARIABLE, AttributeType::VARNAME);
    Comparison c4 = Comparison(ComparisonOperator::EQUALS, param9, param10);

    cacheValue = {{"1", "var"}};
    cache.addResult(c4, cacheValue);

    SECTION("Fixed = Fixed") {
        SECTION("Cache hit") {
            SECTION("find with \"1\" = \"1\"") {
                Parameter param3 = Parameter("1", ParameterType::FIXED_INT, AttributeType::NONE);
                Parameter param4 = Parameter("1", ParameterType::FIXED_INT, AttributeType::NONE);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                vector<vector<string>> expected = {{"1", "1"}};
                REQUIRE(equals(res, expected));
            }
        }

        SECTION("Cache miss") {
            SECTION("find with \"1\" = \"2\"") {
                Parameter param3 = Parameter("1", ParameterType::FIXED_INT, AttributeType::NONE);
                Parameter param4 = Parameter("2", ParameterType::FIXED_INT, AttributeType::NONE);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }

            SECTION("find with \"hello\" = \"2\"") {
                Parameter param3 = Parameter("hello", ParameterType::FIXED_STRING, AttributeType::NONE);
                Parameter param4 = Parameter("2", ParameterType::FIXED_INT, AttributeType::NONE);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }
        }
    }

    SECTION("Param = Fixed") {
        SECTION("Cache hit") {
            SECTION("find with pn.stmt# = \"1\"") {
                Parameter param3 = Parameter("pn", ParameterType::PRINT, AttributeType::STMTNO);
                Parameter param4 = Parameter("1", ParameterType::FIXED_INT, AttributeType::NONE);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                vector<vector<string>> expected = {{"1", "1"}};
                REQUIRE(equals(res, expected));
            }

            SECTION("find with pn1.stmt# = \"1\"") {
                Parameter param3 = Parameter("pn", ParameterType::PRINT, AttributeType::STMTNO);
                Parameter param4 = Parameter("1", ParameterType::FIXED_INT, AttributeType::NONE);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                vector<vector<string>> expected = {{"1", "1"}};
                REQUIRE(equals(res, expected));
            }
        }

        SECTION("Cache miss") {
            SECTION("find with pn.stmt# = \"3\"") {
                Parameter param3 = Parameter("pn", ParameterType::PRINT, AttributeType::STMTNO);
                Parameter param4 = Parameter("3", ParameterType::FIXED_INT, AttributeType::NONE);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }

            SECTION("find with pn.varName = \"1\"") {
                Parameter param3 = Parameter("pn", ParameterType::PRINT, AttributeType::VARNAME);
                Parameter param4 = Parameter("1", ParameterType::FIXED_INT, AttributeType::NONE);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }

            SECTION("find with pn.stmt# = \"1\", pn is stmt instead of print") {
                Parameter param3 = Parameter("pn", ParameterType::STMT, AttributeType::STMTNO);
                Parameter param4 = Parameter("1", ParameterType::FIXED_INT, AttributeType::NONE);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }
        }
    }

    SECTION("Fixed = Param") {
        // with "var1" = "var1"
        Parameter param11 = Parameter("var1", ParameterType::FIXED_STRING, AttributeType::NONE);
        Parameter param12 = Parameter("var1", ParameterType::FIXED_STRING, AttributeType::NONE);
        Comparison c6 = Comparison(ComparisonOperator::EQUALS, param11, param12);

        cacheValue = {{"var1", "var1"}};
        cache.addResult(c6, cacheValue);
        SECTION("Cache hit") {
            SECTION("find with \"var\" = r.varName") {
                Parameter param3 = Parameter("var", ParameterType::FIXED_STRING, AttributeType::NONE);
                Parameter param4 = Parameter("r", ParameterType::READ, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                vector<vector<string>> expected = {{"var", "2"}};
                REQUIRE(equals(res, expected));
            }

            SECTION("find with \"var\" = r1.varName") {
                Parameter param3 = Parameter("var", ParameterType::FIXED_STRING, AttributeType::NONE);
                Parameter param4 = Parameter("r1", ParameterType::READ, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                vector<vector<string>> expected = {{"var", "2"}};
                REQUIRE(equals(res, expected));
            }
        }

        SECTION("Cache miss") {
            SECTION("find with \"var1\" = r.varName") {
                Parameter param3 = Parameter("var1", ParameterType::FIXED_STRING, AttributeType::NONE);
                Parameter param4 = Parameter("r", ParameterType::READ, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }

            SECTION("find with \"var\" = r.varName, r is a print statement") {
                Parameter param3 = Parameter("var", ParameterType::FIXED_STRING, AttributeType::NONE);
                Parameter param4 = Parameter("r", ParameterType::PRINT, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }

            SECTION("find with \"var1\" = var1.varName") {
                Parameter param3 = Parameter("var1", ParameterType::FIXED_STRING, AttributeType::NONE);
                Parameter param4 = Parameter("var1", ParameterType::READ, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }
        }
    }

    SECTION("Param = Param") {
        SECTION("Cache hit") {
            SECTION("find with pn.varName = v.varName") {
                Parameter param3 = Parameter("pn", ParameterType::PRINT, AttributeType::VARNAME);
                Parameter param4 = Parameter("v", ParameterType::VARIABLE, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                vector<vector<string>> expected = {{"1", "var"}};
                REQUIRE(equals(res, expected));
            }

            SECTION("find with pn1.varName = v1.varName") {
                Parameter param3 = Parameter("pn1", ParameterType::PRINT, AttributeType::VARNAME);
                Parameter param4 = Parameter("v1", ParameterType::VARIABLE, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                vector<vector<string>> expected = {{"1", "var"}};
                REQUIRE(equals(res, expected));
            }
        }

        SECTION("Cache miss") {
            SECTION("find with pn.varName = r.varName, pn is a variable") {
                Parameter param3 = Parameter("pn", ParameterType::VARIABLE, AttributeType::VARNAME);
                Parameter param4 = Parameter("r", ParameterType::READ, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }

            SECTION("find with pn.varName = pn.varName") {
                Parameter param3 = Parameter("pn", ParameterType::PRINT, AttributeType::VARNAME);
                Parameter param4 = Parameter("pn", ParameterType::PRINT, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }

            SECTION("find with pn.varName = r.varName, r is print statement") {
                Parameter param3 = Parameter("pn", ParameterType::PRINT, AttributeType::VARNAME);
                Parameter param4 = Parameter("r", ParameterType::PRINT, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }

            SECTION("find with pn.varName = r.stmt#") {
                Parameter param3 = Parameter("pn", ParameterType::PRINT, AttributeType::VARNAME);
                Parameter param4 = Parameter("r", ParameterType::READ, AttributeType::STMTNO);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }

            SECTION("find with pn.stmt# = r.varName") {
                Parameter param3 = Parameter("pn", ParameterType::PRINT, AttributeType::STMTNO);
                Parameter param4 = Parameter("r", ParameterType::READ, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }

            SECTION("find with pn.stmt# = r.varName") {
                Parameter param3 = Parameter("pn", ParameterType::PRINT, AttributeType::STMTNO);
                Parameter param4 = Parameter("r", ParameterType::READ, AttributeType::VARNAME);
                Comparison c5 = Comparison(ComparisonOperator::EQUALS, param3, param4);

                vector<vector<string>> res = cache.findResult(c5);
                REQUIRE(res.empty());
            }
        }
    }
}