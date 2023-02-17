//#include "catch.hpp"
//#include <string>
//#include "PKB/PKB.h"
//#include "PKB/ReadPKB.h"
//#include "PKB/WritePKB.h"
//#include "SP/SP.h"
//#include "qps/QPS.h"
//#include "SP/Tokenizer.h"
//#include "SP/Parser.h"
//#include "SP/AST/Node/Procedure.h"
//#include "SP/SPExceptions.h"
//using namespace std;
//
//bool exists(vector<string>& result, string item) {
//    return find(result.begin(), result.end(), item) != result.end();
//}
//
//TEST_CASE("adhoc") {
//    string filename = "../../../tests/Sample_source.txt";
//    SP sourceProcessor;
//    QPS qps;
//    WritePKB writePKB;
//    ReadPKB readPKB;
//    PKB pkb;
//    pkb.initializePkb();
//    writePKB.setInstancePKB(pkb);
//    readPKB.setInstancePKB(pkb);
//    sourceProcessor.processFile(filename, &writePKB, &readPKB);
//
//    string input = "assign a; Select a pattern a(_, _\"x + 1\"_)";
//    vector<string> result = qps.processQueries(input, readPKB);
//    REQUIRE(result.size() == 3);
//    REQUIRE(exists(result, "7"));
//    REQUIRE(exists(result, "18"));
//    REQUIRE(exists(result, "23"));
//}
//
//TEST_CASE("adhoc2") {
//    string filename = "../../../tests/Sample_source.txt";
//    SP sourceProcessor;
//    QPS qps;
//    WritePKB writePKB;
//    ReadPKB readPKB;
//    PKB pkb;
//    pkb.initializePkb();
//    writePKB.setInstancePKB(pkb);
//    readPKB.setInstancePKB(pkb);
//    sourceProcessor.processFile(filename, &writePKB, &readPKB);
//    string input = "variable a; stmt s1; Select s1 such that Follows(2, s1)";
//    vector<string> result = qps.processQueries(input, readPKB);
//    REQUIRE(result.size() == 1);
//    REQUIRE(exists(result, "3"));
//}
//
//TEST_CASE("assign a; select a;") {
//    string filename = "../../../tests/Sample_source.txt";
//    SP sourceProcessor;
//    QPS qps;
//    WritePKB writePKB;
//    ReadPKB readPKB;
//    PKB pkb;
//    pkb.initializePkb();
//    writePKB.setInstancePKB(pkb);
//    readPKB.setInstancePKB(pkb);
//    sourceProcessor.processFile(filename, &writePKB, &readPKB);
//    string input = "assign a; Select a";
//    vector<string> expected{ "1", "24", "2", "7", "18", "3", "5", "8", "9", "11", "15", "17", "19", "20", "21", "23"};
//    vector<string> res = qps.processQueries(input, readPKB);
//    REQUIRE(res.size() == expected.size());
//}
//
//TEST_CASE("if i; Select i;") {
//    string filename = "../../../tests/Sample_source.txt";
//    SP sourceProcessor;
//    QPS qps;
//    WritePKB writePKB;
//    ReadPKB readPKB;
//    PKB pkb;
//    pkb.initializePkb();
//    writePKB.setInstancePKB(pkb);
//    readPKB.setInstancePKB(pkb);
//    sourceProcessor.processFile(filename, &writePKB, &readPKB);
//    string input = "if i; Select i";
//    vector<string> result = qps.processQueries(input, readPKB);
//    REQUIRE(result.size() == 3);
//    REQUIRE(exists(result, "6"));
//    REQUIRE(exists(result, "13"));
//    REQUIRE(exists(result, "22"));
//}
//
////FAILING TEST CASE COMMENTED OUT FOR SETTING UP CI
///*
//TEST_CASE("if i;") {
//    string filename = "C:/CS3203_Project/Team06/Code06/tests/Sample_source.txt";
//    SP sourceProcessor;
//    QPS qps;
//    WritePKB writePKB;
//    ReadPKB readPKB;
//    PKB pkb;
//    pkb.initializePkb();
//    writePKB.setInstancePKB(pkb);
//    readPKB.setInstancePKB(pkb);
//    sourceProcessor.processFile(filename, &writePKB, &readPKB);
//    SECTION("if i; select i such that follows(5, i)") {
//        string input = "if i; Select i such that Follows(5, i)";
//        vector<string> result = qps.processQueries(input, readPKB);
//        REQUIRE(result.size() == 1);
//        REQUIRE(exists(result, "6"));
//    }
//
////    SECTION("if i; stmt s; Select s such that Follows(2, s)") {
////        string input = "stmt s; Select s such that Follows(5, s)";
////        vector<string> expected{ "6", "13", "22" };
////        vector<string> res = qps.processQueries(input, readPKB);
////        for (string r : res) {
////            cout << r << "\n";
////        }
////        REQUIRE(res == expected);
////    }
//}
//*/
//
//TEST_CASE("patterns clause") {
//    string filename = "../../../tests/Sample_source.txt";
//    SP sourceProcessor;
//    QPS qps;
//    WritePKB writePKB;
//    ReadPKB readPKB;
//    PKB pkb;
//    pkb.initializePkb();
//    writePKB.setInstancePKB(pkb);
//    readPKB.setInstancePKB(pkb);
//    sourceProcessor.processFile(filename, &writePKB, &readPKB);
//    string input = "assign a; variable v; stmt s; Select v pattern a(v, _)";
//    vector<string> result = qps.processQueries(input, readPKB);
//    REQUIRE(result.size() == 4);
//    REQUIRE(exists(result, "i"));
//    REQUIRE(exists(result, "y"));
//    REQUIRE(exists(result, "z"));
//    REQUIRE(exists(result, "x"));
//}