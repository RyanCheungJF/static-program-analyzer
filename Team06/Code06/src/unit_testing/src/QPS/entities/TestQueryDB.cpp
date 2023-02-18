#include "catch.hpp"
#include "qps/entities/QueryDB.h"
#include <vector>
#include <string>
using namespace std;

TEST_CASE("insertTable / insertion of two tables with intersection / return one table with 2 values ") {
    AppConstants CONSTANTS;
    QueryDB qdb;
    vector<vector<string>> content1 = {{"1"}, {"2"}, {"3"}};
    vector<vector<string>> content2 = {{"1"}, {"2"}};
    vector<Parameter> params = {Parameter("s", CONSTANTS.STMT)};
    Table t1(params, content1);
    Table t2(params, content2);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    vector<string> finalContent = qdb.fetch(params[0]);

    REQUIRE(((finalContent[0] =="1" && (finalContent[1] =="2")) && finalContent.size() == 2));
}

TEST_CASE("insertTable / insertion of two tables without intersection / return two tables") {
    AppConstants CONSTANTS;
    QueryDB qdb;
    vector<vector<string>> content1 = {{"1"}, {"2"}, {"3"}};
    vector<vector<string>> content2 = {{"4"}, {"5"}};
    vector<Parameter> params1 = {Parameter("s1", CONSTANTS.STMT)};
    vector<Parameter> params2 = {Parameter("s2", CONSTANTS.STMT)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    vector<string> finalContent = qdb.fetch(params1[0]);

    REQUIRE(((finalContent[0] =="1" && (finalContent[1] =="2") && (finalContent[2] == "3")) && finalContent.size() == 3));
}

TEST_CASE("insertTable / insertion of three tables with intersection / return ONE table") {
    AppConstants CONSTANTS;
    QueryDB qdb;
    vector<vector<string>> content1 = {{"1", "x"}, {"2", "y"}, {"3", "y"}};
    vector<vector<string>> content2 = {{"4", "x"}, {"5", "y"}};
    vector<vector<string>> content3 = {{"4", "99"}, {"5", "99"}};
    vector<Parameter> params1 = {Parameter("s1", CONSTANTS.STMT), Parameter("v", CONSTANTS.VARIABLE)};
    vector<Parameter> params2 = {Parameter("s2", CONSTANTS.STMT), Parameter("v", CONSTANTS.VARIABLE)};
    vector<Parameter> params3 = {Parameter("s2", CONSTANTS.STMT), Parameter("a", CONSTANTS.ASSIGN)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    Table t3(params3, content3);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    qdb.insertTable(t3);
    vector<string> finalContent = qdb.fetch(params1[0]);
    vector<vector<string>> expectedCols = {
            {"1", "2", "3"},
            {"x", "y"},
            {"4", "5"},
            {"99"}
    };
    vector<Parameter> expectedParams = {
            Parameter("s1", CONSTANTS.STMT),
            Parameter("v", CONSTANTS.VARIABLE),
            Parameter("s2", CONSTANTS.STMT),
            Parameter("a", CONSTANTS.ASSIGN)
    };
    vector<string> aa = qdb.fetch(Parameter("s1", CONSTANTS.STMT));
    bool a = qdb.fetch(Parameter("s1", CONSTANTS.STMT)) == expectedCols[0];
    bool b = qdb.fetch(Parameter("v", CONSTANTS.VARIABLE)) == expectedCols[1];
    bool c = qdb.fetch(Parameter("s2", CONSTANTS.STMT)) == expectedCols[2];
    bool d = qdb.fetch(Parameter("a", CONSTANTS.ASSIGN)) == expectedCols[3];
    REQUIRE((a && b && c && d));
}

TEST_CASE("insertTable / intersecting headers but non intersecting content / return ONE empty table") {
    AppConstants CONSTANTS;
    QueryDB qdb;
    vector<vector<string>> content1 = {{"1", "x"}, {"2", "y"}, {"3", "y"}};
    vector<vector<string>> content2 = {{"4", "a"}, {"5", "b"}};
    vector<vector<string>> content3 = {{"4", "99"}, {"5", "99"}};
    vector<Parameter> params1 = {Parameter("s1", CONSTANTS.STMT), Parameter("v", CONSTANTS.VARIABLE)};
    vector<Parameter> params2 = {Parameter("s2", CONSTANTS.STMT), Parameter("v", CONSTANTS.VARIABLE)};
    vector<Parameter> params3 = {Parameter("s2", CONSTANTS.STMT), Parameter("a", CONSTANTS.ASSIGN)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    Table t3(params3, content3);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    qdb.insertTable(t3);
    vector<string> finalContent = qdb.fetch(params1[0]);
    vector<vector<string>> expectedCols = {
            {},
            {},
            {},
            {}
    };
    vector<Parameter> expectedParams = {
            Parameter("s1", CONSTANTS.STMT),
            Parameter("v", CONSTANTS.VARIABLE),
            Parameter("s2", CONSTANTS.STMT),
            Parameter("a", CONSTANTS.ASSIGN)
    };
    vector<string> aa = qdb.fetch(Parameter("s1", CONSTANTS.STMT));
    bool a = qdb.fetch(Parameter("s1", CONSTANTS.STMT)) == expectedCols[0];
    bool b = qdb.fetch(Parameter("v", CONSTANTS.VARIABLE)) == expectedCols[1];
    bool c = qdb.fetch(Parameter("s2", CONSTANTS.STMT)) == expectedCols[2];
    bool d = qdb.fetch(Parameter("a", CONSTANTS.ASSIGN)) == expectedCols[3];
    REQUIRE((a && b && c && d));
}

TEST_CASE("insertTable / third table intersects with first two tables / return ONE empty table") {
    AppConstants CONSTANTS;
    QueryDB qdb;
    vector<vector<string>> content1 = {{"1", "x"}, {"2", "y"}, {"3", "y"}};
    vector<vector<string>> content2 = {{"4", "a"}, {"5", "b"}};
    vector<vector<string>> content3 = {{"4", "1"}, {"5", "2"}, {"4", "3"}, {"5", "3"}};
    vector<Parameter> params1 = {Parameter("s1", CONSTANTS.STMT), Parameter("x", CONSTANTS.VARIABLE)};
    vector<Parameter> params2 = {Parameter("s2", CONSTANTS.STMT), Parameter("y", CONSTANTS.VARIABLE)};
    vector<Parameter> params3 = {Parameter("s2", CONSTANTS.STMT), Parameter("s1", CONSTANTS.STMT)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    Table t3(params3, content3);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    qdb.insertTable(t3);
    vector<string> finalContent = qdb.fetch(params1[0]);
    vector<vector<string>> expectedCols = {
            {"1", "2", "3"},
            {"4", "5"},
            {"x", "y"},
            {"a", "b"}
    };
    vector<string> aa = qdb.fetch(Parameter("s1", CONSTANTS.STMT));
    bool a = qdb.fetch(Parameter("s1", CONSTANTS.STMT)) == expectedCols[0];
    bool b = qdb.fetch(Parameter("s2", CONSTANTS.STMT)) == expectedCols[1];
    bool c = qdb.fetch(Parameter("x", CONSTANTS.VARIABLE)) == expectedCols[2];
    bool d = qdb.fetch(Parameter("y", CONSTANTS.VARIABLE)) == expectedCols[3];
    REQUIRE((a && b && c && d));
}