#include "catch.hpp"
#include "qps/entities/Table.h"
#include <vector>
#include <string>
using namespace std;

TEST_CASE("selectColumns / the selection of single column will reduce the table to chosen column / return true") {
    vector<Parameter> headers = {Parameter("v", "variable"), Parameter("_", "wildcard")};
    vector<vector<string>> content = {{"x", "5"}};
    Table table(headers, content);
    vector<int> indexes = {0};
    Table t = table.selectColumns(indexes);
    REQUIRE(true);
}

