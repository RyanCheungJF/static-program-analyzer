#include "catch.hpp"
#include "qps/QPS.h"
using namespace std;

TEST_CASE("processQueries / bad query string / throw corresponding error") {
    string query = "assign a; Select n pattern a(\"k\", _)";
    QPS qps;
    ReadPKB r;
    vector<string> result = qps.processQueries(query, r);
    vector<string> expected{"SemanticError"};
    CHECK(result == expected);
}
