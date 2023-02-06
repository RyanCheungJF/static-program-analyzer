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
//    string input = "stmt ifs; Select ifs such that Follows(5, ifs)";
    string input = "assign v; Select v";
//    string input = "variable v; Select v";
    vector<string> expected{ "2" };
    vector<string> res = qps.processQueries(input, readPKB);
    for (string r : res) {
        cout << r << "\n";
    }
    REQUIRE(true);
}