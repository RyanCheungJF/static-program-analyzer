#include "catch.hpp"
#include "qps/entities/QueryDB.h"
#include <vector>
#include <string>
using namespace std;

TEST_CASE("insertTable / insertion of two tables with intersection / return one table with 2 values ") {
    QueryDB qdb;
    vector<vector<string>> content1 = {{"1"}, {"2"}, {"3"}};
    vector<vector<string>> content2 = {{"1"}, {"2"}};
    vector<Parameter> params = {Parameter("s", "stmt")};
    Table t1(params, content1);
    Table t2(params, content2);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    vector<string> finalContent = qdb.fetch(params[0]);

    REQUIRE(((finalContent[0] =="1" && (finalContent[1] =="2")) && finalContent.size() == 2));
}

TEST_CASE("insertTable / insertion of two tables without intersection / return two tables") {
    QueryDB qdb;
    vector<vector<string>> content1 = {{"1"}, {"2"}, {"3"}};
    vector<vector<string>> content2 = {{"4"}, {"5"}};
    vector<Parameter> params1 = {Parameter("s1", "stmt")};
    vector<Parameter> params2 = {Parameter("s2", "stmt")};
    Table t1(params1, content1);
    Table t2(params2, content2);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    vector<string> finalContent = qdb.fetch(params1[0]);

    REQUIRE(((finalContent[0] =="1" && (finalContent[1] =="2") && (finalContent[2] == "3")) && finalContent.size() == 3));
}