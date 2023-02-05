#include "catch.hpp"
#include <string>
#include "PKB/PKB.h"
#include "PKB/ReadPKB.h"
#include "PKB/WritePKB.h"
#include "SP.h"
#include "qps/QPS.h"
using namespace std;

TEST_CASE("adhoc") {
    string sampleProgram = "procedure Example {\n"
                           "  x = 2;\n"
                           "  z = 3;\n"
                           "  i = 5;\n"
                           "  while (i!=0) {\n"
                           "    x = x - 1;\n"
                           "    if (x==1) then {\n"
                           "      z = x + 1; }\n"
                           "    else {\n"
                           "      y = z + x; }\n"
                           "    z = z + x + i;\n"
                           "    call q;\n"
                           "    i = i - 1; }\n"
                           "  call p; }\n"
                           "\n"
                           "procedure p {\n"
                           "  if (x<0) then {\n"
                           "    while (i>0) {\n"
                           "      x = z * 3 + 2 * y;\n"
                           "      call q;\n"
                           "      i = i - 1; }\n"
                           "    x = x + 1;\n"
                           "    z = x + z; }\n"
                           "  else {\n"
                           "    z = 1; }\n"
                           "  z = z + x + i; }\n"
                           "\n"
                           "procedure q {\n"
                           "  if (x==1) then {\n"
                           "    z = x + 1; }\n"
                           "  else {\n"
                           "    x = z + x; } }";
    SP sourceProcessor;
    WritePKB writePKB;
    ReadPKB readPKB;
    PKB pkb;
    FollowsStorage fs;
    EntityStorage et;
    ProcedureStorage ps;
    StmtStorage sts;
    QPS qps;
    pkb.entityStorage = &et;
    pkb.procedureStorage = &ps;
    pkb.statementStorage = &sts;
    pkb.followsStorage = &fs;
    writePKB.setInstancePKB(pkb);
    readPKB.setInstancePKB(pkb);
//    QPS qps;
//    WritePKB writePkb;
//    ReadPKB readPkb;
//    PKB pkb;
//    FollowsStorage fs;
//    SP sourceProcessor;
//    pkb.followsStorage = &fs;
//    StmtStorage sts;
    sourceProcessor.processFile(sampleProgram, &writePKB);
//    writePkb.setFollows(1, 2);

    string input = "stmt ifs; Select ifs such that Follows(1, ifs)";
    vector<string> expected{ "2" };
    vector<string> res = qps.processQueries(input, readPKB);
    for (string r : res) {
        cout << r << "\n";
    }
    REQUIRE(true);
}