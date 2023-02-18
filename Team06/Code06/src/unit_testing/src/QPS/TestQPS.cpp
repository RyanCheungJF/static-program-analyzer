#include "catch.hpp"
#include "qps/QPS.h"
using namespace std;

TEST_CASE("processQueries / bad query string / throw corresponding error") {
	string query = "stamt s; Select s such that Follows(1, s)";
	QPS qps;
	ReadPKB r;
	vector<string> result = qps.processQueries(query, r);
	vector<string> expected{ "SyntaxError" };
	CHECK(result == expected);
}