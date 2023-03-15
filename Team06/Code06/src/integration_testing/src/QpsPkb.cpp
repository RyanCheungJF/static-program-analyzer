
#include <algorithm>

#include "../../spa/src/PKB/ReadPKB.cpp"
#include "../../spa/src/PKB/WritePKB.cpp"
#include "../../spa/src/PKB/utils/utils.h"
#include "../../spa/src/QPS/QPS.h"
#include "../../spa/src/utils/AppConstants.h"
#include "catch.hpp"

using namespace std;

PKB buildPkb() {

    PKB pkb;
    WritePKB writePkb;

    pkb.initializePkb();
    writePkb.setInstancePKB(pkb);

    //  procedure main {
    // 1     x = 1;
    // 2     call sub;
    // 3	 while (y == x) {
    // 4         y = x + 2;
    // 5		    read y;
    //       }
    // 6     x = x + 2;
    //  }
    //
    //   procedure sub {
    // 7     print x;
    // 8     y = x + 2;
    // 9     if (count > 0) then {
    // 10         read x;
    //       } else {
    // 11         z = x * y;
    //       }
    // 12    call end;
    //   }
    //
    //   procedure end {
    // 13    print end;
    //   }

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> mainCFG = {
        {1, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {2}}}},
        {2, {{AppConstants::PARENTS, {1}}, {AppConstants::CHILDREN, {3}}}},
        {3, {{AppConstants::PARENTS, {2, 5}}, {AppConstants::CHILDREN, {4, 6}}}},
        {4, {{AppConstants::PARENTS, {3}}, {AppConstants::CHILDREN, {5}}}},
        {5, {{AppConstants::PARENTS, {4}}, {AppConstants::CHILDREN, {3}}}},
        {6, {{AppConstants::PARENTS, {3}}, {AppConstants::CHILDREN, {}}}}};

    writePkb.writeCFG("main", mainCFG);

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> subCFG = {
        {7, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {8, 9}}}},
        {8, {{AppConstants::PARENTS, {7}}, {AppConstants::CHILDREN, {9}}}},
        {9, {{AppConstants::PARENTS, {7, 8}}, {AppConstants::CHILDREN, {10, 11}}}},
        {10, {{AppConstants::PARENTS, {9}}, {AppConstants::CHILDREN, {12}}}},
        {11, {{AppConstants::PARENTS, {9}}, {AppConstants::CHILDREN, {12}}}},
        {12, {{AppConstants::PARENTS, {10, 11}}, {AppConstants::CHILDREN, {}}}}};

    writePkb.writeCFG("sub", subCFG);

    std::unordered_map<StmtNum, std::unordered_map<std::string, std::unordered_set<StmtNum>>> endCFG = {
        {13, {{AppConstants::PARENTS, {}}, {AppConstants::CHILDREN, {}}}}};

    writePkb.writeCFG("end", endCFG);

    unordered_set<int> mainProcNums = {1, 2, 3, 4, 5, 6};
    writePkb.setProcedure("main", mainProcNums);

    unordered_set<int> subProcNums = {7, 8, 9, 10, 11, 12};
    writePkb.setProcedure("sub", subProcNums);

    unordered_set<int> endProcNums = {13};
    writePkb.setProcedure("end", endProcNums);

    writePkb.setCall(2, "sub");
    writePkb.setCall(12, "end");

    writePkb.setStatement(AppConstants::ASSIGN, 1);
    writePkb.setStatement(AppConstants::ASSIGN, 4);
    writePkb.setStatement(AppConstants::ASSIGN, 6);
    writePkb.setStatement(AppConstants::ASSIGN, 8);
    writePkb.setStatement(AppConstants::ASSIGN, 11);
    writePkb.setStatement(AppConstants::IF, 9);
    writePkb.setStatement(AppConstants::WHILE, 3);
    writePkb.setStatement(AppConstants::CALL, 2);
    writePkb.setStatement(AppConstants::CALL, 12);
    writePkb.setStatement(AppConstants::PRINT, 7);
    writePkb.setStatement(AppConstants::PRINT, 13);
    writePkb.setStatement(AppConstants::READ, 5);
    writePkb.setStatement(AppConstants::READ, 10);

    writePkb.setEntity(1, {"x"});
    writePkb.setEntity(3, {"x", "y"});
    writePkb.setEntity(4, {"x", "y"});
    writePkb.setEntity(5, {"y"});
    writePkb.setEntity(6, {"x"});
    writePkb.setEntity(7, {"x"});
    writePkb.setEntity(8, {"x", "y"});
    writePkb.setEntity(9, {"count"});
    writePkb.setEntity(10, {"x"});
    writePkb.setEntity(11, {"x", "y", "z"});
    writePkb.setEntity(13, {"end"});

    writePkb.setConstant(1, unordered_set<int>{1});
    writePkb.setConstant(4, unordered_set<int>{2});
    writePkb.setConstant(6, unordered_set<int>{2});
    writePkb.setConstant(8, unordered_set<int>{2});
    writePkb.setConstant(9, unordered_set<int>{0});

    writePkb.setFollows(1, 2);
    writePkb.setFollows(2, 3);
    writePkb.setFollows(4, 5);
    writePkb.setFollows(7, 8);
    writePkb.setFollows(8, 9);

    writePkb.setFollowsT(1, unordered_set<int>{2, 3, 6});
    writePkb.setFollowsT(2, unordered_set<int>{3, 6});
    writePkb.setFollowsT(3, unordered_set<int>{6});
    writePkb.setFollowsT(4, unordered_set<int>{5});
    writePkb.setFollowsT(7, unordered_set<int>{8, 9, 12});
    writePkb.setFollowsT(8, unordered_set<int>{9, 12});

    writePkb.setParent(3, 4);
    writePkb.setParent(3, 5);
    writePkb.setParent(9, 10);
    writePkb.setParent(9, 11);

    writePkb.setParentT(3, {3, 4});
    writePkb.setParentT(9, {10, 11});

    writePkb.setUsesS(2, {"x", "y", "count"});
    writePkb.setUsesS(3, {"x", "y"});
    writePkb.setUsesS(4, {"x"});
    writePkb.setUsesS(6, {"x"});
    writePkb.setUsesS(7, {"x"});
    writePkb.setUsesS(8, {"x"});
    writePkb.setUsesS(9, {"x", "y", "count"});
    writePkb.setUsesS(11, {"x", "y"});
    writePkb.setUsesS(12, {"end"});
    writePkb.setUsesP("main", {"x", "y", "count", "end"});
    writePkb.setUsesP("sub", {"x", "y", "count", "end"});
    writePkb.setUsesP("end", {"end"});

    writePkb.setModifiesS(1, {"x"});
    writePkb.setModifiesS(2, {"x", "y", "z"});
    writePkb.setModifiesS(4, {"y"});
    writePkb.setModifiesS(5, {"y"});
    writePkb.setModifiesS(6, {"x"});
    writePkb.setModifiesS(8, {"y"});
    writePkb.setModifiesS(10, {"x"});
    writePkb.setModifiesS(11, {"z"});
    writePkb.setModifiesP("main", {"x", "y", "z"});
    writePkb.setModifiesP("sub", {"x", "y", "z"});

    writePkb.setCalls("main", {"sub"});
    writePkb.setCalls("sub", {"end"});
    writePkb.setCallsT("main", {"sub", "end"});
    writePkb.setCallsT("sub", {"end"});

    string lhs = "x";
    unique_ptr<Expression> patternTree = pkb_utils::buildSubtree("1");
    writePkb.writePattern(lhs, 1, move(patternTree));

    lhs = "y";
    patternTree = pkb_utils::buildSubtree("x + 2");
    writePkb.writePattern(lhs, 4, move(patternTree));

    lhs = "x";
    patternTree = pkb_utils::buildSubtree("x + 2");
    writePkb.writePattern(lhs, 6, move(patternTree));

    lhs = "y";
    patternTree = pkb_utils::buildSubtree("x + 2");
    writePkb.writePattern(lhs, 8, move(patternTree));

    lhs = "z";
    patternTree = pkb_utils::buildSubtree("x * y");
    writePkb.writePattern(lhs, 11, move(patternTree));

    writePkb.setWhilePattern(3, {"x", "y"});
    writePkb.setIfPattern(9, {"count"});

    return pkb;
}

bool exists(vector<string>& result, string item) {
    return find(result.begin(), result.end(), item) != result.end();
}

TEST_CASE("Single Select Query") {
    PKB pkb = buildPkb();
    ReadPKB readPkb;
    readPkb.setInstancePKB(pkb);
    QPS qps;
    vector<string> result;

    SECTION("Select stmt") {
        string query = R"(
		stmt s;
		Select s)";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 13);
        REQUIRE(exists(result, "1"));
        REQUIRE(exists(result, "2"));
        REQUIRE(exists(result, "3"));
        REQUIRE(exists(result, "4"));
        REQUIRE(exists(result, "5"));
        REQUIRE(exists(result, "6"));
        REQUIRE(exists(result, "7"));
        REQUIRE(exists(result, "8"));
        REQUIRE(exists(result, "9"));
        REQUIRE(exists(result, "10"));
        REQUIRE(exists(result, "11"));
        REQUIRE(exists(result, "12"));
        REQUIRE(exists(result, "13"));
    }

    SECTION("Select read") {
        string query = R"(
		read r;
		Select r)";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 2);
        REQUIRE(exists(result, "5"));
        REQUIRE(exists(result, "10"));
    }

    SECTION("Select print") {
        string query = R"(
		print p;
		Select p)";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 2);
        REQUIRE(exists(result, "7"));
        REQUIRE(exists(result, "13"));
    }

    SECTION("Select call") {
        string query = R"(
		call c;
		Select c)";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 2);
        REQUIRE(exists(result, "2"));
        REQUIRE(exists(result, "12"));
    }

    SECTION("Select while") {
        string query = R"(
		while w;
		Select w)";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 1);
        REQUIRE(exists(result, "3"));
    }

    SECTION("Select if") {
        string query = R"(
		if ifs;
		Select ifs)";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 1);
        REQUIRE(exists(result, "9"));
    }

    SECTION("Select assign") {
        string query = R"(
		assign a;
		Select a)";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 5);
        REQUIRE(exists(result, "1"));
        REQUIRE(exists(result, "4"));
        REQUIRE(exists(result, "6"));
        REQUIRE(exists(result, "8"));
        REQUIRE(exists(result, "11"));
    }

    SECTION("Select variable") {
        string query = R"(
		variable v;
		Select v)";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 5);
        REQUIRE(exists(result, "x"));
        REQUIRE(exists(result, "y"));
        REQUIRE(exists(result, "z"));
        REQUIRE(exists(result, "count"));
        REQUIRE(exists(result, "end"));
    }

    SECTION("Select constant") {
        string query = R"(
		constant c;
		Select c)";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 3);
        REQUIRE(exists(result, "0"));
        REQUIRE(exists(result, "1"));
        REQUIRE(exists(result, "2"));
    }
}

TEST_CASE("Select synonym with single such that clause, synonym is in clause") {
    PKB pkb = buildPkb();
    ReadPKB readPkb;
    readPkb.setInstancePKB(pkb);
    QPS qps;
    vector<string> result;

    SECTION("Follows") {
        SECTION("wildcard, syn") {
            string query = R"(
			stmt s;
			Select s such that Follows(_, s))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 5);
            REQUIRE(exists(result, "2"));
            REQUIRE(exists(result, "3"));
            REQUIRE(exists(result, "5"));
            REQUIRE(exists(result, "8"));
            REQUIRE(exists(result, "9"));
        }

        SECTION("int, syn") {
            string query = R"(
			stmt s;
			Select s such that Follows(s, 2))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 1);
            REQUIRE(exists(result, "1"));
        }
    }

    SECTION("FollowsT") {
        SECTION("syn, wildcard") {
            string query = R"(
			assign a;
			Select a such that Follows*(a, _))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 3);
            REQUIRE(exists(result, "1"));
            REQUIRE(exists(result, "4"));
            REQUIRE(exists(result, "8"));
        }

        SECTION("syn, syn") {
            string query = R"(
			stmt s; while w;
			Select s such that Follows*(s, w))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 2);
            REQUIRE(exists(result, "1"));
            REQUIRE(exists(result, "2"));
        }
    }

    SECTION("Parent") {
        SECTION("syn, int") {
            string query = R"(
			while w;
			Select w such that Parent(w, 4))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 1);
            REQUIRE(exists(result, "3"));
        }

        SECTION("syn, syn") {
            string query = R"(
			stmt s1, s2;
			Select s2 such that Parent(s1, s2))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 4);
            REQUIRE(exists(result, "4"));
            REQUIRE(exists(result, "5"));
            REQUIRE(exists(result, "10"));
            REQUIRE(exists(result, "11"));
        }
    }

    SECTION("ParentT") {
        SECTION("syn, int") {
            string query = R"(
			if ifs;
			Select ifs such that Parent*(ifs, 11))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 1);
            REQUIRE(exists(result, "9"));
        }

        SECTION("syn, syn") {
            string query = R"(
			stmt s1, s2;
			Select s1 such that Parent*(s1, s2))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 2);
            REQUIRE(exists(result, "3"));
            REQUIRE(exists(result, "9"));
        }
    }

    SECTION("Uses") {
        SECTION("int, syn") {
            string query = R"(
			variable v;
			Select v such that Uses(2, v))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 3);
            REQUIRE(exists(result, "x"));
            REQUIRE(exists(result, "y"));
            REQUIRE(exists(result, "count"));
        }

        SECTION("stmtSyn, syn") {
            string query = R"(
			print pn; variable v;
			Select pn such that Uses(pn, v))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 1);
            REQUIRE(exists(result, "7"));
        }

        SECTION("procSyn, var") {
            string query = R"(
			procedure p;
			Select p such that Uses(p, "count"))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 2);
            REQUIRE(exists(result, "main"));
            REQUIRE(exists(result, "sub"));
        }
    }

    SECTION("Modifies") {
        SECTION("stmtSyn, var") {
            string query = R"(
			stmt s;
			Select s such that Modifies(s, "y"))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 4);
            REQUIRE(exists(result, "2"));
            REQUIRE(exists(result, "4"));
            REQUIRE(exists(result, "5"));
            REQUIRE(exists(result, "8"));
        }

        SECTION("stmtSyn, wildcard") {
            string query = R"(
			read r;
			Select r such that Modifies(r, _))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 2);
            REQUIRE(exists(result, "5"));
            REQUIRE(exists(result, "10"));
        }

        SECTION("procSyn, syn") {
            string query = R"(
			procedure p; variable v;
			Select p such that Modifies(p, v))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 2);
            REQUIRE(exists(result, "main"));
            REQUIRE(exists(result, "sub"));
        }

        SECTION("proc, syn") {
            string query = R"(
			variable v;
			Select v such that Modifies("main", v))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 3);
            REQUIRE(exists(result, "x"));
            REQUIRE(exists(result, "y"));
            REQUIRE(exists(result, "z"));
        }
    }

    SECTION("Calls") {
        SECTION("proc, procSyn") {
            string query = R"(
			procedure p;
			Select p such that Calls("main", p))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 1);
            REQUIRE(exists(result, "sub"));
        }

        SECTION("wildcard, procSyn") {
            string query = R"(
			procedure p;
			Select p such that Calls(_, p))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 2);
            REQUIRE(exists(result, "sub"));
            REQUIRE(exists(result, "end"));
        }

        SECTION("procSyn, wildcard") {
            string query = R"(
			procedure p;
			Select p such that Calls(p, _))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 2);
            REQUIRE(exists(result, "main"));
            REQUIRE(exists(result, "sub"));
        }

        SECTION("procSyn, proc") {
            string query = R"(
			procedure p;
			Select p such that Calls(p, "sub"))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 1);
            REQUIRE(exists(result, "main"));
        }
    }

    SECTION("CallsT") {
        SECTION("proc, procSyn") {
            string query = R"(
			procedure p;
			Select p such that Calls*("main", p))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 2);
            REQUIRE(exists(result, "sub"));
            REQUIRE(exists(result, "end"));
        }

        SECTION("wildcard, procSyn") {
            string query = R"(
			procedure p;
			Select p such that Calls*(_, p))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 2);
            REQUIRE(exists(result, "sub"));
            REQUIRE(exists(result, "end"));
        }

        SECTION("procSyn, wildcard") {
            string query = R"(
			procedure p;
			Select p such that Calls*(p, _))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 2);
            REQUIRE(exists(result, "main"));
            REQUIRE(exists(result, "sub"));
        }

        SECTION("procSyn, proc") {
            string query = R"(
			procedure p;
			Select p such that Calls*(p, "end"))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 2);
            REQUIRE(exists(result, "main"));
            REQUIRE(exists(result, "sub"));
        }
    }

    SECTION("Next") {
        SECTION("syn, wildcard") {
            string query = R"(
			assign a;
			Select a such that Next(a, _))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 4);
            REQUIRE(exists(result, "1"));
            REQUIRE(exists(result, "4"));
            REQUIRE(exists(result, "8"));
            REQUIRE(exists(result, "11"));
        }

        SECTION("int, syn") {
            string query = R"(
			stmt s;
			Select s such that Next(1, s))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 1);
            REQUIRE(exists(result, "2"));
        }

        SECTION("syn, syn") {
            string query = R"(
			stmt s; assign a;
			Select s such that Next(s, a))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(exists(result, "3"));
            REQUIRE(exists(result, "7"));
            REQUIRE(exists(result, "9"));
        }
    }

    SECTION("NextT") {
        SECTION("syn, wildcard") {
            string query = R"(
			assign a;
			Select a such that Next*(a, _))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 4);
            REQUIRE(exists(result, "1"));
            REQUIRE(exists(result, "4"));
            REQUIRE(exists(result, "8"));
            REQUIRE(exists(result, "11"));
        }

        SECTION("int, syn") {
            string query = R"(
			stmt s;
			Select s such that Next*(1, s))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 5);
            REQUIRE(exists(result, "2"));
            REQUIRE(exists(result, "3"));
            REQUIRE(exists(result, "4"));
            REQUIRE(exists(result, "5"));
            REQUIRE(exists(result, "6"));
        }

        SECTION("syn, int") {
            string query = R"(
			stmt s;
			Select s such that Next*(s, 3))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 5);
            REQUIRE(exists(result, "1"));
            REQUIRE(exists(result, "2"));
            REQUIRE(exists(result, "3"));
            REQUIRE(exists(result, "4"));
            REQUIRE(exists(result, "5"));
        }
    }

    SECTION("Affects") {
        SECTION("int, syn") {
            string query = R"(
			assign a;
			Select a such that Affects(1, a))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 0);
        }

        SECTION("syn, wildcard") {
            string query = R"(
			assign a;
			Select a such that Affects(a, _))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 1);
            REQUIRE(exists(result, "8"));
        }
    }

    SECTION("AffectsT") {
        SECTION("syn, int") {
            string query = R"(
			assign a;
			Select a such that Affects*(a, 11))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 1);
            REQUIRE(exists(result, "8"));
        }

        SECTION("syn, syn") {
            string query = R"(
			assign a1, a2;
			Select a2 such that Affects*(a1, a2))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 1);
            REQUIRE(exists(result, "11"));
        }
    }
}

TEST_CASE("Select synonym from single such that clause, synonym is not in clause") {
    PKB pkb = buildPkb();
    ReadPKB readPkb;
    readPkb.setInstancePKB(pkb);
    QPS qps;
    vector<string> result;

    SECTION("clause is false, Parent") {
        string query = R"(
		if ifs;
		Select ifs such that Parent(1, 5))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.empty());
    }

    SECTION("clause is false, Modifies") {
        string query = R"(
		variable v;
		Select v such that Modifies(1, "y"))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.empty());
    }

    SECTION("clause is false, Calls") {
        string query = R"(
		procedure p;
		Select p such that Calls("main", "end"))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.empty());
    }

    SECTION("clause is true, Follows*") {
        string query = R"(
		assign a;
		Select a such that Follows*(_, 6))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 5);
        REQUIRE(exists(result, "1"));
        REQUIRE(exists(result, "4"));
        REQUIRE(exists(result, "6"));
        REQUIRE(exists(result, "8"));
        REQUIRE(exists(result, "11"));
    }

    SECTION("clause is true, Uses") {
        string query = R"(
		while w; assign a; variable v;
		Select w such that Uses(a, v))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 1);
        REQUIRE(exists(result, "3"));
    }

    SECTION("clause is true, CallsT") {
        string query = R"(
		procedure p;
		Select p such that Calls*("main", "end"))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 3);
        REQUIRE(exists(result, "main"));
        REQUIRE(exists(result, "sub"));
        REQUIRE(exists(result, "end"));
    }
}

TEST_CASE("Select synonym from single assign pattern clause, synonym is in clause") {
    PKB pkb = buildPkb();
    ReadPKB readPkb;
    readPkb.setInstancePKB(pkb);
    QPS qps;
    vector<string> result;

    SECTION("syn", "partial wildcard") {
        string query = R"(
		assign a; variable v;
		Select a pattern a(v, _"x"_))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 4);
        REQUIRE(exists(result, "4"));
        REQUIRE(exists(result, "6"));
        REQUIRE(exists(result, "8"));
        REQUIRE(exists(result, "11"));
    }

    SECTION("exact", "wildcard") {
        string query = R"(
		assign a;
		Select a pattern a("y", _))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 2);
        REQUIRE(exists(result, "4"));
        REQUIRE(exists(result, "8"));
    }

    SECTION("wildcard", "exact") {
        string query = R"(
		assign a;
		Select a pattern a(_, "x * y"))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 1);
        REQUIRE(exists(result, "11"));
    }
}

TEST_CASE("Select synonym from single if/while pattern clause, synonym is in clause") {
    PKB pkb = buildPkb();
    ReadPKB readPkb;
    readPkb.setInstancePKB(pkb);
    QPS qps;
    vector<string> result;

    SECTION("if", "exact") {
        string query = R"(
		if if; 
		Select if pattern if("count", _, _))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == "9");
        REQUIRE(exists(result, "9"));
    }

    SECTION("if", "syn") {
        string query = R"(
		if if; variable v;
		Select if pattern if(v, _, _))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 1);
        REQUIRE(exists(result, "9"));
    }

    SECTION("while", "exact") {
        string query = R"(
		while w; 
		Select w pattern w("abcdef", _))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 0);
    }

    SECTION("while", "syn") {
        string query = R"(
		while w; variable v;
		Select v pattern w(v, _))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 2);
        REQUIRE(exists(result, "x"));
        REQUIRE(exists(result, "y"));
    }
}

TEST_CASE("Select synonym from single such that  clause, synonym is in clause") {
    PKB pkb = buildPkb();
    ReadPKB readPkb;
    readPkb.setInstancePKB(pkb);
    QPS qps;
    vector<string> result;

    SECTION("if", "exact") {
        string query = R"(
		if if; 
		Select if pattern if("count", _, _))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 1);
        REQUIRE(result[0] == "9");
        REQUIRE(exists(result, "9"));
    }

    SECTION("if", "syn") {
        string query = R"(
		if if; variable v;
		Select if pattern if(v, _, _))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 1);
        REQUIRE(exists(result, "9"));
    }

    SECTION("while", "exact") {
        string query = R"(
		while w; 
		Select w pattern w("abcdef", _))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 0);
    }

    SECTION("while", "syn") {
        string query = R"(
		while w; variable v;
		Select v pattern w(v, _))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 2);
        REQUIRE(exists(result, "x"));
        REQUIRE(exists(result, "y"));
    }
}

TEST_CASE("Select synonym from multi clause, synonym is in both clauses") {
    PKB pkb = buildPkb();
    ReadPKB readPkb;
    readPkb.setInstancePKB(pkb);
    QPS qps;
    vector<string> result;

    SECTION("select assign from Follows*, pattern") {
        string query = R"(
		assign a; variable v;
		Select a such that Follows*(1, a) pattern a(v, _"x"_))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 1);
        REQUIRE(exists(result, "6"));
    }

    SECTION("Check that changing order of clauses does not affect results") {
        string query1 = R"(
		assign a; variable v;
		Select a such that Follows*(1, a) pattern a(v, _"x"_))";

        result = qps.processQueries(query1, readPkb);

        string query2 = R"(
		assign a; variable v;
		Select a pattern a(v, _"x"_) such that Follows*(1, a))";

        vector<string> result2 = qps.processQueries(query2, readPkb);

        REQUIRE(result == result2);
    }

    SECTION("select assign from pattern, Parent") {
        string query = R"(
		assign a; variable v; while w;
		Select a pattern a(v, _"x"_) such that Parent(w, a))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 1);
        REQUIRE(exists(result, "4"));
    }

    SECTION("select variable from Parent*, pattern") {
        string query = R"(
		assign a; variable v; stmt s;
		Select v such that Parent*(s, a) pattern a(v, _"x"_))";

        result = qps.processQueries(query, readPkb);
        REQUIRE(result.size() == 2);
        REQUIRE(exists(result, "y"));
        REQUIRE(exists(result, "z"));
    }
}

TEST_CASE("Select synonym from multi clause, synonym is NOT in both clauses") {
    PKB pkb = buildPkb();
    ReadPKB readPkb;
    readPkb.setInstancePKB(pkb);
    QPS qps;
    vector<string> result;
    SECTION("synonym in ONE clause") {
        SECTION("Both clauses are non-empty/true") {
            string query = R"(
			assign a; variable v;
			Select a such that Follows*(1, 2) pattern a(v, _"x"_))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 4);
            REQUIRE(exists(result, "4"));
            REQUIRE(exists(result, "6"));
            REQUIRE(exists(result, "8"));
            REQUIRE(exists(result, "11"));
        }

        SECTION("One clause is empty/false") {
            string query = R"(
			assign a; variable v;
			Select a such that Parent(1, 2) pattern a(v, _"x"_))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 0);
        }

        SECTION("Both clauses are empty/false") {
            string query = R"(
			assign a; variable v;
			Select a such that Parent(1, 2) pattern a(v, "x"))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 0);
        }
    }

    SECTION("synonym in none of the clauses") {
        SECTION("Both clauses are non-empty/true") {
            string query = R"(
			assign a; variable v; constant c;
			Select c such that Follows*(1, 2) pattern a(v, _"x"_))";

            result = qps.processQueries(query, readPkb);
            REQUIRE(result.size() == 3);
            REQUIRE(exists(result, "0"));
            REQUIRE(exists(result, "1"));
            REQUIRE(exists(result, "2"));
        }
    }
}
