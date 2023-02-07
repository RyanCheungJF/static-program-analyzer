#include "catch.hpp"
#include <string>
#include "PKB/PKB.h"
#include "PKB/ReadPKB.h"
#include "PKB/WritePKB.h"
#include "SP.h"
#include "qps/QPS.h"
#include "../SP/Tokenizer.h"
#include "../SP/Parser.h"
#include "../SP/AST/Node/Procedure.h"
#include "../SP/SPExceptions.h"
using namespace std;

TEST_CASE("adhoc") {
    string filename = "/Users/faruq/Desktop/Books/Y3-S2/CS3203/team_project/22s2-cp-spa-team-06/Team06/Code06/tests/Sample_source.txt";
//    string filename = "/Users/admin/Downloads/this sem/cs3203project/Team06/Code06/tests/Sample_source.txt";
//    string filename = "../../../tests/Sample_source.txt";
    SP sourceProcessor;
    QPS qps;
    WritePKB writePKB;
    ReadPKB readPKB;
    PKB pkb;
    FollowsStorage fs;
    EntityStorage et;
    ProcedureStorage ps;
    StmtStorage sts;
    ConstantStorage cs;
    PatternStorage pt;
    pkb.entityStorage = &et;
    pkb.procedureStorage = &ps;
    pkb.statementStorage = &sts;
    pkb.followsStorage = &fs;
    pkb.constantStorage = &cs;
    pkb.patternStorage = &pt;
    writePKB.setInstancePKB(pkb);
    readPKB.setInstancePKB(pkb);
    sourceProcessor.processFile(filename, &writePKB);

    bool res = true;
    string input = "assign a; Select a pattern a(_, _\"x + 1\"_)";
    vector<string> expected{ "7", "20", "27" };
    vector<string> result = qps.processQueries(input, readPKB);
    for (string r : result) {
        cout << r << "\n";
    }
    REQUIRE(res);
}

TEST_CASE("adhoc2") {
    string filename = "/Users/faruq/Desktop/Books/Y3-S2/CS3203/team_project/22s2-cp-spa-team-06/Team06/Code06/tests/Sample_source.txt";
//    string filename = "/Users/admin/Downloads/this sem/cs3203project/Team06/Code06/tests/Sample_source.txt";
//    string filename = "../../../tests/Sample_source.txt";
//    string filename = "C:/CS3203_Project/Team06/Code06/tests/Sample_source.txt";
    SP sourceProcessor;
    QPS qps;
    WritePKB writePKB;
    ReadPKB readPKB;
    PKB pkb;
    FollowsStorage fs;
    EntityStorage et;
    ProcedureStorage ps;
    StmtStorage sts;
    ConstantStorage cs;
    PatternStorage pt;
    pkb.entityStorage = &et;
    pkb.procedureStorage = &ps;
    pkb.statementStorage = &sts;
    pkb.followsStorage = &fs;
    pkb.constantStorage = &cs;
    pkb.patternStorage = &pt;
    writePKB.setInstancePKB(pkb);
    readPKB.setInstancePKB(pkb);
    sourceProcessor.processFile(filename, &writePKB);
//    string input = "stmt ifs; Select ifs such that Follows(5, ifs)";
    string input = "variable a; stmt s1; Select s1 such that Follows(2, s1);";
//    string input = "variable a; Select a";
    vector<string> expected{ "3" };
    vector<string> res = qps.processQueries(input, readPKB);
    for (string r : res) {
        cout << r << "\n";
    }
    REQUIRE(expected == res);
}

TEST_CASE("assign a; select a;") {
    string filename = "/Users/faruq/Desktop/Books/Y3-S2/CS3203/team_project/22s2-cp-spa-team-06/Team06/Code06/tests/Sample_source.txt";
    SP sourceProcessor;
    QPS qps;
    WritePKB writePKB;
    ReadPKB readPKB;
    PKB pkb;
    FollowsStorage fs;
    EntityStorage et;
    ProcedureStorage ps;
    StmtStorage sts;
    ConstantStorage cs;
    PatternStorage pt;
    pkb.entityStorage = &et;
    pkb.procedureStorage = &ps;
    pkb.statementStorage = &sts;
    pkb.followsStorage = &fs;
    pkb.constantStorage = &cs;
    pkb.patternStorage = &pt;
    writePKB.setInstancePKB(pkb);
    readPKB.setInstancePKB(pkb);
    sourceProcessor.processFile(filename, &writePKB);
//    string input = "stmt ifs; Select ifs such that Follows(5, ifs)";
    string input = "assign a; Select a;";
    vector<string> expected{ "1", "24", "2", "7", "18", "3", "5", "8", "9", "11", "15", "17", "19", "20", "21", "23"};
//    string input = "assign a; Select a pattern a(_, _\"x + 1\"_)";
//    string input = "variable v; Select v";
//    vector<string> expected{ "2" };
    vector<string> res = qps.processQueries(input, readPKB);
    for (string r : res) {
        cout << r << "\n";
    }
    REQUIRE(res == expected);
}

TEST_CASE("if i; Select i;") {
    string filename = "/Users/faruq/Desktop/Books/Y3-S2/CS3203/team_project/22s2-cp-spa-team-06/Team06/Code06/tests/Sample_source.txt";
    SP sourceProcessor;
    QPS qps;
    WritePKB writePKB;
    ReadPKB readPKB;
    PKB pkb;
    FollowsStorage fs;
    EntityStorage et;
    ProcedureStorage ps;
    StmtStorage sts;
    ConstantStorage cs;
    PatternStorage pt;
    pkb.entityStorage = &et;
    pkb.procedureStorage = &ps;
    pkb.statementStorage = &sts;
    pkb.followsStorage = &fs;
    pkb.constantStorage = &cs;
    pkb.patternStorage = &pt;
    writePKB.setInstancePKB(pkb);
    readPKB.setInstancePKB(pkb);
    sourceProcessor.processFile(filename, &writePKB);
//    string input = "stmt ifs; Select ifs such that Follows(5, ifs)";
    string input = "if i; Select i;";
    vector<string> expected{ "6", "13", "22"};
//    string input = "assign a; Select a pattern a(_, _\"x + 1\"_)";
//    string input = "variable v; Select v";
//    vector<string> expected{ "2" };
    vector<string> res = qps.processQueries(input, readPKB);
    for (string r : res) {
        cout << r << "\n";
    }
    REQUIRE(res == expected);
}

TEST_CASE("if i;") {
    string filename = "/Users/faruq/Desktop/Books/Y3-S2/CS3203/team_project/22s2-cp-spa-team-06/Team06/Code06/tests/Sample_source.txt";
    SP sourceProcessor;
    QPS qps;
    WritePKB writePKB;
    ReadPKB readPKB;
    PKB pkb;
    FollowsStorage fs;
    EntityStorage et;
    ProcedureStorage ps;
    StmtStorage sts;
    ConstantStorage cs;
    PatternStorage pt;
    pkb.entityStorage = &et;
    pkb.procedureStorage = &ps;
    pkb.statementStorage = &sts;
    pkb.followsStorage = &fs;
    pkb.constantStorage = &cs;
    pkb.patternStorage = &pt;
    writePKB.setInstancePKB(pkb);
    readPKB.setInstancePKB(pkb);
    sourceProcessor.processFile(filename, &writePKB);
    SECTION("if i; select i such that follows(5, i)") {
        string input = "if i; Select i such that Follows(5, i)";
        vector<string> expected{ "6" };
        vector<string> res = qps.processQueries(input, readPKB);
        for (string r : res) {
            cout << r << "\n";
        }
        REQUIRE(res == expected);
    }

    SECTION("if i; stmt s; Select s such that Follows(2, s)") {
        string input = "stmt s; Select s such that Follows(5, s)";
        vector<string> expected{ "6", "13", "22" };
        vector<string> res = qps.processQueries(input, readPKB);
        for (string r : res) {
            cout << r << "\n";
        }
        REQUIRE(res == expected);
    }
}