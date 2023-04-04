#include <string>
#include <vector>

#include "QPS/entities/QueryDB.h"
#include "QPS/entities/Table.h"
#include "catch.hpp"
using namespace std;

TEST_CASE("insertTable / inserting large table / return true") {
    vector<Parameter> h1 = {
        Parameter("v", ParameterType::VARIABLE),
        Parameter("s", ParameterType::STMT),
        Parameter("vv", ParameterType::VARIABLE),
        Parameter("s1", ParameterType::STMT),
    };
    vector<vector<string>> c1 = {
        {"a", "1", "aa", "11"},   {"b", "2", "bb", "22"},   {"c", "3", "cc", "33"},   {"d", "4", "dd", "44"},
        {"a", "5", "aa", "55"},   {"b", "6", "bb", "66"},   {"c", "7", "cc", "77"},   {"d", "8", "dd", "88"},
        {"a", "11", "aa", "11"},  {"b", "22", "bb", "22"},  {"c", "33", "cc", "33"},  {"d", "44", "dd", "44"},
        {"a", "55", "aa", "55"},  {"b", "66", "bb", "66"},  {"c", "77", "cc", "77"},  {"d", "88", "dd", "88"},
        {"a", "1", "aaa", "11"},  {"b", "2", "bbb", "22"},  {"c", "3", "ccc", "33"},  {"d", "4", "ddd", "44"},
        {"a", "5", "aaa", "55"},  {"b", "6", "bbb", "66"},  {"c", "7", "ccc", "77"},  {"d", "8", "ddd", "88"},
        {"a", "11", "aaa", "11"}, {"b", "22", "bbb", "22"}, {"c", "33", "ccc", "33"}, {"d", "44", "ddd", "44"},
        {"a", "55", "aaa", "55"}, {"b", "66", "bbb", "66"}, {"c", "77", "ccc", "77"}, {"d", "88", "ddd", "88"},
    };
    vector<Parameter> h2 = {Parameter("v1", ParameterType::VARIABLE), Parameter("s2", ParameterType::STMT),
                            Parameter("v", ParameterType::VARIABLE), Parameter("ss", ParameterType::STMT)};
    vector<vector<string>> c2 = {
        {"d", "1", "a", "11"},    {"e", "2", "b", "22"},    {"f", "3", "c", "33"},    {"g", "4", "d", "44"},
        {"dd", "1", "a", "11"},   {"ee", "2", "b", "22"},   {"ff", "3", "c", "33"},   {"gg", "4", "d", "44"},
        {"d", "1", "a", "111"},   {"e", "2", "b", "222"},   {"f", "3", "c", "333"},   {"g", "4", "d", "444"},
        {"dd", "1", "a", "111"},  {"ee", "2", "b", "222"},  {"ff", "3", "c", "333"},  {"gg", "4", "d", "444"},
        {"d", "11", "a", "11"},   {"e", "22", "b", "22"},   {"f", "33", "c", "33"},   {"g", "44", "d", "44"},
        {"dd", "11", "a", "11"},  {"ee", "22", "b", "22"},  {"ff", "33", "c", "33"},  {"gg", "44", "d", "44"},
        {"d", "11", "a", "111"},  {"e", "22", "b", "222"},  {"f", "33", "c", "333"},  {"g", "44", "d", "444"},
        {"dd", "11", "a", "111"}, {"ee", "22", "b", "222"}, {"ff", "33", "c", "333"}, {"gg", "44", "d", "444"},
    };
    Table t1(h1, c1);
    Table t2(h2, c2);
    QueryDB qdb;
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    ReadPKB readPKB;
    vector<string> content = qdb.fetch({Parameter("v", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT),
                                        Parameter("vv", ParameterType::VARIABLE), Parameter("s1", ParameterType::STMT),
                                        Parameter("v1", ParameterType::VARIABLE), Parameter("s2", ParameterType::STMT),
                                        Parameter("ss", ParameterType::STMT)},
                                       readPKB);
    REQUIRE(content.size() == 256);
}

TEST_CASE("insertTable / insertion of two tables with intersection / return "
          "one table with 2 values ") {

    QueryDB qdb;
    ReadPKB readPkb;
    vector<vector<string>> content1 = {{"1"}, {"2"}, {"3"}};
    vector<vector<string>> content2 = {{"1"}, {"2"}};
    vector<Parameter> params = {Parameter("s", ParameterType::STMT)};
    Table t1(params, content1);
    Table t2(params, content2);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    vector<string> finalContent = qdb.fetch(params, readPkb);
    REQUIRE(find(finalContent.begin(), finalContent.end(), "1") != finalContent.end());
    REQUIRE(find(finalContent.begin(), finalContent.end(), "2") != finalContent.end());
}

TEST_CASE("insertTable / insertion of two tables without intersection / return "
          "two tables") {

    QueryDB qdb;
    ReadPKB readPkb;
    vector<vector<string>> content1 = {{"1"}, {"2"}, {"3"}};
    vector<vector<string>> content2 = {{"4"}, {"5"}};
    vector<Parameter> params1 = {Parameter("s1", ParameterType::STMT)};
    vector<Parameter> params2 = {Parameter("s2", ParameterType::STMT)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    vector<string> finalContent = qdb.fetch(params1, readPkb);

    REQUIRE(find(finalContent.begin(), finalContent.end(), "1") != finalContent.end());
    REQUIRE(find(finalContent.begin(), finalContent.end(), "2") != finalContent.end());
    REQUIRE(find(finalContent.begin(), finalContent.end(), "3") != finalContent.end());
}

TEST_CASE("insertTable / insertion of three tables with intersection / return "
          "ONE table") {

    QueryDB qdb;
    ReadPKB readPkb;
    vector<vector<string>> content1 = {{"1", "x"}, {"2", "y"}, {"3", "y"}};
    vector<vector<string>> content2 = {{"4", "x"}, {"5", "y"}};
    vector<vector<string>> content3 = {{"4", "99"}, {"5", "99"}};
    vector<Parameter> params1 = {Parameter("s1", ParameterType::STMT), Parameter("v", ParameterType::VARIABLE)};
    vector<Parameter> params2 = {Parameter("s2", ParameterType::STMT), Parameter("v", ParameterType::VARIABLE)};
    vector<Parameter> params3 = {Parameter("s2", ParameterType::STMT), Parameter("a", ParameterType::ASSIGN)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    Table t3(params3, content3);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    qdb.insertTable(t3);
    vector<string> finalContent = qdb.fetch({
        Parameter("s1", ParameterType::STMT),
        Parameter("v", ParameterType::VARIABLE),
        Parameter("s2", ParameterType::STMT),
        Parameter("a", ParameterType::ASSIGN),
        }, readPkb);
    REQUIRE(find(finalContent.begin(), finalContent.end(), "1 x 4 99") != finalContent.end());
    REQUIRE(find(finalContent.begin(), finalContent.end(), "2 y 5 99") != finalContent.end());
    REQUIRE(find(finalContent.begin(), finalContent.end(), "3 y 5 99") != finalContent.end());

}

TEST_CASE("insertTable / intersecting headers but non intersecting content / "
          "return ONE empty table") {

    QueryDB qdb;
    ReadPKB readPKB;
    vector<vector<string>> content1 = {{"1", "x"}, {"2", "y"}, {"3", "y"}};
    vector<vector<string>> content2 = {{"4", "a"}, {"5", "b"}};
    vector<vector<string>> content3 = {{"4", "99"}, {"5", "99"}};
    vector<Parameter> params1 = {Parameter("s1", ParameterType::STMT), Parameter("v", ParameterType::VARIABLE)};
    vector<Parameter> params2 = {Parameter("s2", ParameterType::STMT), Parameter("v", ParameterType::VARIABLE)};
    vector<Parameter> params3 = {Parameter("s2", ParameterType::STMT), Parameter("a", ParameterType::ASSIGN)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    Table t3(params3, content3);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    qdb.insertTable(t3);
    vector<Parameter> fetchParams = {Parameter("s1", ParameterType::STMT), Parameter("v", ParameterType::VARIABLE),
                                        Parameter("s2", ParameterType::STMT), Parameter("a", ParameterType::ASSIGN)};
    vector<string> finalContent = qdb.fetch(fetchParams, readPKB);
    REQUIRE(finalContent.empty());
}

TEST_CASE("insertTable / third table intersects with first two tables / return "
          "correct intersection result") {

    QueryDB qdb;
    ReadPKB readPKB;
    vector<vector<string>> content1 = {{"1", "x"}, {"2", "y"}, {"3", "y"}};
    vector<vector<string>> content2 = {{"4", "a"}, {"5", "b"}};
    vector<vector<string>> content3 = {{"4", "1"}, {"5", "2"}, {"4", "3"}, {"5", "3"}};
    vector<Parameter> params1 = {Parameter("s1", ParameterType::STMT), Parameter("x", ParameterType::VARIABLE)};
    vector<Parameter> params2 = {Parameter("s2", ParameterType::STMT), Parameter("y", ParameterType::VARIABLE)};
    vector<Parameter> params3 = {Parameter("s2", ParameterType::STMT), Parameter("s1", ParameterType::STMT)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    Table t3(params3, content3);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    qdb.insertTable(t3);
    vector<Parameter> fetchParams = {
            Parameter("s1", ParameterType::STMT),
            Parameter("s2", ParameterType::STMT),
            Parameter("x", ParameterType::VARIABLE),
            Parameter("y", ParameterType::VARIABLE)
    };
    vector<string> finalContent = qdb.fetch(fetchParams, readPKB);
    REQUIRE(find(finalContent.begin(), finalContent.end(), "3 5 y b") != finalContent.end());
    REQUIRE(find(finalContent.begin(), finalContent.end(), "2 5 y b") != finalContent.end());
    REQUIRE(find(finalContent.begin(), finalContent.end(), "3 4 y a") != finalContent.end());
    REQUIRE(find(finalContent.begin(), finalContent.end(), "1 4 x a") != finalContent.end());
}

TEST_CASE("insertTable / insert tables with empty content / tables can still "
          "be inserted without content") {

    QueryDB qdb;
    ReadPKB readPKB;
    vector<vector<string>> content1 = {};
    vector<vector<string>> content2 = {};
    vector<vector<string>> content3 = {};
    vector<Parameter> params1 = {Parameter("s1", ParameterType::STMT), Parameter("x", ParameterType::VARIABLE)};
    vector<Parameter> params2 = {Parameter("s2", ParameterType::STMT), Parameter("y", ParameterType::VARIABLE)};
    vector<Parameter> params3 = {Parameter("s2", ParameterType::STMT), Parameter("s1", ParameterType::STMT)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    Table t3(params3, content3);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    qdb.insertTable(t3);
    bool a = qdb.fetch({Parameter("s1", ParameterType::STMT)}, readPKB).empty();
    bool b = qdb.fetch({Parameter("s2", ParameterType::STMT)}, readPKB).empty();
    bool c = qdb.fetch({Parameter("x", ParameterType::VARIABLE)}, readPKB).empty();
    bool d = qdb.fetch({Parameter("y", ParameterType::VARIABLE)}, readPKB).empty();
    REQUIRE((a && b && c && d));
}

TEST_CASE("insertTable / insert tables with with content / tables correctly insert and fetch "
          "be inserted without content") {
    // this is based of systemTest 1'
    // while w1, w2, w3;
    // Select <w1, w2, w3> such that Parent*(w1, w2) and Parent*(w2, w3)
    QueryDB qdb;
    ReadPKB readPKB;
    vector<vector<string>> content1 = {{"25", "27"}, {"25", "30"}, {"25", "34"},
                                       {"27", "30"}, {"27", "34"}, {"30", "34"}};
    vector<Parameter> params1 = {Parameter("w1", ParameterType::WHILE), Parameter("w2", ParameterType::WHILE)};
    vector<Parameter> params2 = {Parameter("w2", ParameterType::WHILE), Parameter("w3", ParameterType::WHILE)};
    Table t1(params1, content1);
    Table t2(params2, content1);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    vector<string> res = qdb.fetch({Parameter("w1", ParameterType::WHILE), Parameter("w2", ParameterType::WHILE),
                                    Parameter("w3", ParameterType::WHILE)},
                                   readPKB);
    REQUIRE(find(res.begin(), res.end(), "25 27 30") != res.end());
    REQUIRE(find(res.begin(), res.end(), "25 27 34") != res.end());
    REQUIRE(find(res.begin(), res.end(), "25 30 34") != res.end());
    REQUIRE(find(res.begin(), res.end(), "27 30 34") != res.end());
}

TEST_CASE("insertTable / insert tables with with content, non intersecting / tables correctly insert and fetch "
          "be inserted without content") {
    // this is based of systemTest 1'
    // while w1, w2, w3;
    // Select <w1, w2, w3> such that Parent*(w1, w2) and Parent*(w3, 34)
    QueryDB qdb;
    ReadPKB readPKB;
    vector<vector<string>> content1 = {{"25", "27"}, {"25", "30"}, {"25", "34"},
                                       {"27", "30"}, {"27", "34"}, {"30", "34"}};
    vector<vector<string>> content2 = {{"25"}, {"27"}, {"30"}};
    vector<Parameter> params1 = {Parameter("w1", ParameterType::WHILE), Parameter("w2", ParameterType::WHILE)};
    vector<Parameter> params2 = {Parameter("w3", ParameterType::WHILE)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    qdb.insertTable(t2);
    qdb.insertTable(t1);
    vector<string> res = qdb.fetch({Parameter("w1", ParameterType::WHILE), Parameter("w2", ParameterType::WHILE),
                                    Parameter("w3", ParameterType::WHILE)},
                                   readPKB);
    REQUIRE(find(res.begin(), res.end(), "25 27 25") != res.end());
    REQUIRE(find(res.begin(), res.end(), "25 30 25") != res.end());
    REQUIRE(find(res.begin(), res.end(), "25 34 25") != res.end());
    REQUIRE(find(res.begin(), res.end(), "27 30 25") != res.end());
    REQUIRE(find(res.begin(), res.end(), "27 34 25") != res.end());
    REQUIRE(find(res.begin(), res.end(), "30 34 25") != res.end());
    REQUIRE(find(res.begin(), res.end(), "25 27 27") != res.end());
    REQUIRE(find(res.begin(), res.end(), "25 30 27") != res.end());
    REQUIRE(find(res.begin(), res.end(), "25 34 27") != res.end());
    REQUIRE(find(res.begin(), res.end(), "27 30 27") != res.end());
    REQUIRE(find(res.begin(), res.end(), "27 34 27") != res.end());
    REQUIRE(find(res.begin(), res.end(), "30 34 27") != res.end());
    REQUIRE(find(res.begin(), res.end(), "25 27 30") != res.end());
    REQUIRE(find(res.begin(), res.end(), "25 30 30") != res.end());
    REQUIRE(find(res.begin(), res.end(), "25 34 30") != res.end());
    REQUIRE(find(res.begin(), res.end(), "27 30 30") != res.end());
    REQUIRE(find(res.begin(), res.end(), "27 34 30") != res.end());
    REQUIRE(find(res.begin(), res.end(), "30 34 30") != res.end());
}

TEST_CASE("fetch / fetching duplicate parameters / returns duplicate parameters "
          "be inserted without content") {
    QueryDB qdb;
    ReadPKB readPKB;
    vector<vector<string>> content1 = {{"a", "b"}, {"c", "d"}, {"e", "f"}};
    vector<Parameter> params1 = {Parameter("v", ParameterType::VARIABLE), Parameter("v2", ParameterType::VARIABLE)};
    Table t1(params1, content1);
    qdb.insertTable(t1);
    vector<string> res = qdb.fetch({Parameter("v", ParameterType::VARIABLE), Parameter("v", ParameterType::VARIABLE),
                                    Parameter("v2", ParameterType::VARIABLE)},
                                   readPKB);
    REQUIRE(find(res.begin(), res.end(), "a a b") != res.end());
    REQUIRE(find(res.begin(), res.end(), "c c d") != res.end());
    REQUIRE(find(res.begin(), res.end(), "e e f") != res.end());
}

TEST_CASE("fetch / insertion of table with duplicate params but only fetching first instance "
          "of that param / returns one instance of the param ") {
    QueryDB qdb;
    ReadPKB readPKB;
    vector<vector<string>> content1 = {{"a", "aa"}, {"c", "cc"}, {"e", "ee"}};
    vector<Parameter> params1 = {Parameter("v", ParameterType::VARIABLE), Parameter("v", ParameterType::VARIABLE)};
    Table t1(params1, content1);
    qdb.insertTable(t1);
    vector<string> res = qdb.fetch(
        {
            Parameter("v", ParameterType::VARIABLE),
        },
        readPKB);
    REQUIRE(find(res.begin(), res.end(), "a") != res.end());
    REQUIRE(find(res.begin(), res.end(), "c") != res.end());
    REQUIRE(find(res.begin(), res.end(), "e") != res.end());
}

TEST_CASE("fetch / insertion with intersection / fetch single parameter") {
    // This test case is from systemtest1 in tuple.txt
    QueryDB qdb;
    ReadPKB readPKB;
    vector<vector<string>> content1 = {
        {"arithmetic", "z"},      {"arithmetic", "y"},      {"arithmetic", "x"},
        {"arithmetic", "w"},      {"arithmetic", "o"},      {"arithmetic", "a"},
        {"arithmetic", "b"},      {"arithmetic", "k"},      {"arithmetic", "j"},
        {"arithmetic", "i"},      {"conditionalSpam", "i"}, {"conditionalSpam", "a"},
        {"conditionalSpam", "A"}, {"conditionalSpam", "b"}, {"conditionalSpam", "B"},
        {"conditionalSpam", "c"}, {"conditionalSpam", "N"}, {"conditionalSpam", "LongName"}};
    vector<vector<string>> content2 = {{"arithmetic", "conditionalSpam"}};
    vector<Parameter> params1 = {Parameter("p", ParameterType::PROCEDURE), Parameter("v", ParameterType::VARIABLE)};
    vector<Parameter> params2 = {Parameter("p", ParameterType::PROCEDURE), Parameter("q", ParameterType::PROCEDURE)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    vector<string> res = qdb.fetch(
        {
            Parameter("v", ParameterType::VARIABLE),
        },
        readPKB);
    REQUIRE(find(res.begin(), res.end(), "z") != res.end());
    REQUIRE(find(res.begin(), res.end(), "y") != res.end());
    REQUIRE(find(res.begin(), res.end(), "x") != res.end());
    REQUIRE(find(res.begin(), res.end(), "w") != res.end());
    REQUIRE(find(res.begin(), res.end(), "o") != res.end());
    REQUIRE(find(res.begin(), res.end(), "a") != res.end());
    REQUIRE(find(res.begin(), res.end(), "b") != res.end());
    REQUIRE(find(res.begin(), res.end(), "k") != res.end());
    REQUIRE(find(res.begin(), res.end(), "j") != res.end());
    REQUIRE(find(res.begin(), res.end(), "i") != res.end());
}

TEST_CASE("fetch / insertion with intersection / fetch vector of params") {
    // This test case is from systemtest1 in tuple.txt
    QueryDB qdb;
    ReadPKB readPKB;
    vector<vector<string>> content1 = {
        {"arithmetic", "z"},      {"arithmetic", "y"},      {"arithmetic", "x"},
        {"arithmetic", "w"},      {"arithmetic", "o"},      {"arithmetic", "a"},
        {"arithmetic", "b"},      {"arithmetic", "k"},      {"arithmetic", "j"},
        {"arithmetic", "i"},      {"conditionalSpam", "i"}, {"conditionalSpam", "a"},
        {"conditionalSpam", "A"}, {"conditionalSpam", "b"}, {"conditionalSpam", "B"},
        {"conditionalSpam", "c"}, {"conditionalSpam", "N"}, {"conditionalSpam", "LongName"}};
    vector<vector<string>> content2 = {{"arithmetic", "conditionalSpam"}};
    vector<Parameter> params1 = {Parameter("p", ParameterType::PROCEDURE), Parameter("v", ParameterType::VARIABLE)};
    vector<Parameter> params2 = {Parameter("p", ParameterType::PROCEDURE), Parameter("q", ParameterType::PROCEDURE)};
    Table t1(params1, content1);
    Table t2(params2, content2);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    vector<string> res = qdb.fetch({Parameter("v", ParameterType::VARIABLE), Parameter("p", ParameterType::PROCEDURE),
                                    Parameter("q", ParameterType::PROCEDURE)},
                                   readPKB);
    REQUIRE(find(res.begin(), res.end(), "z arithmetic conditionalSpam") != res.end());
    REQUIRE(find(res.begin(), res.end(), "y arithmetic conditionalSpam") != res.end());
    REQUIRE(find(res.begin(), res.end(), "x arithmetic conditionalSpam") != res.end());
    REQUIRE(find(res.begin(), res.end(), "w arithmetic conditionalSpam") != res.end());
    REQUIRE(find(res.begin(), res.end(), "o arithmetic conditionalSpam") != res.end());
    REQUIRE(find(res.begin(), res.end(), "a arithmetic conditionalSpam") != res.end());
    REQUIRE(find(res.begin(), res.end(), "b arithmetic conditionalSpam") != res.end());
    REQUIRE(find(res.begin(), res.end(), "k arithmetic conditionalSpam") != res.end());
    REQUIRE(find(res.begin(), res.end(), "j arithmetic conditionalSpam") != res.end());
    REQUIRE(find(res.begin(), res.end(), "i arithmetic conditionalSpam") != res.end());
}

TEST_CASE("fetch / table with 4 variables fetching 3 variables / return in correct order") {
    QueryDB qdb;
    ReadPKB readPKB;
    vector<Parameter> header1 = {
        Parameter("t", ParameterType::PRINT),
        Parameter("v", ParameterType::VARIABLE),
    };
    vector<Parameter> header2 = {
        Parameter("p", ParameterType::PROCEDURE),
        Parameter("v", ParameterType::VARIABLE),
    };
    vector<Parameter> header3 = {
        Parameter("c", ParameterType::CALL),
    };
    vector<vector<string>> content1 = {
        {"1", "a"},
        {"2", "b"},
        {"3", "c"},
    };
    vector<vector<string>> content2 = {
        {"4", "a"}, {"5", "a"}, {"6", "a"}, {"4", "b"}, {"5", "b"}, {"6", "b"}, {"4", "c"}, {"5", "c"}, {"6", "c"},
    };
    vector<vector<string>> content3 = {
        {"999"},
        {"888"},
    };
    Table t1(header1, content1);
    Table t2(header2, content2);
    Table t3(header3, content3);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    qdb.insertTable(t3);
    vector<string> res = qdb.fetch({Parameter("t", ParameterType::PRINT), Parameter("c", ParameterType::CALL),
                                    Parameter("v", ParameterType::VARIABLE)},
                                   readPKB);

    REQUIRE(find(res.begin(), res.end(), "1 999 a") != res.end());
    REQUIRE(find(res.begin(), res.end(), "1 888 a") != res.end());
    REQUIRE(find(res.begin(), res.end(), "2 999 b") != res.end());
    REQUIRE(find(res.begin(), res.end(), "2 888 b") != res.end());
    REQUIRE(find(res.begin(), res.end(), "3 999 c") != res.end());
    REQUIRE(find(res.begin(), res.end(), "3 888 c") != res.end());
}

TEST_CASE("queryDB fetch / case of (a,a) (a,b) (a,b) intersect tables and fetch a should work") {
    QueryDB qdb;
    ReadPKB readPKB;
    vector<Parameter> h1 = {
        Parameter("a", ParameterType::ASSIGN),
        Parameter("a", ParameterType::ASSIGN),
    };
    vector<Parameter> h2 = {
        Parameter("a", ParameterType::ASSIGN),
    };
    vector<Parameter> h3 = {
        Parameter("a", ParameterType::ASSIGN),
    };
    vector<vector<string>> c1 = {{"29", "29"}};
    vector<vector<string>> c2 = {
        {"28"},
        {"29"},
        {"4"},
    };
    vector<vector<string>> c3 = {
        {"29"},
        {"26"},
        {"3"},
    };
    Table t1 = Table(h1, c1);
    Table t2 = Table(h2, c2);
    Table t3 = Table(h3, c3);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    qdb.insertTable(t3);
    vector<string> res = qdb.fetch({Parameter("a", ParameterType::ASSIGN)}, readPKB);
    REQUIRE(find(res.begin(), res.end(), "29") != res.end());
}

TEST_CASE("queryDB fetch / multiple Next Next* and patterns") {
    QueryDB qdb;
    ReadPKB readPKB;
    vector<Parameter> h1 = {
            Parameter("w1", ParameterType::WHILE),
            Parameter("s", ParameterType::STMT),
    };
    vector<Parameter> h2 = {
            Parameter("s", ParameterType::STMT),
            Parameter("w2", ParameterType::WHILE),
    };
    vector<vector<string>> c1 = {
            {"36", "37"},
            {"36", "39"},
            {"36", "38"},
            {"36", "41"},
            {"36", "40"},
            {"36", "36"},
            {"18", "19"},
            {"18", "20"},
            {"18", "18"},
            {"18", "21"},
            {"18", "14"},
            {"18", "15"},
            {"18", "22"},
            {"18", "16"},
            {"18", "17"},
            {"20", "21"},
            {"20", "14"},
            {"20", "20"},
            {"20", "15"},
            {"20", "22"},
            {"20", "16"},
            {"20", "18"},
            {"20", "17"},
            {"20", "19"},
            {"14", "15"},
            {"14", "22"},
            {"14", "16"},
            {"14", "14"},
            {"14", "18"},
            {"14", "17"},
            {"14", "19"},
            {"14", "20"},
            {"14", "21"},
            {"24", "25"},
            {"24", "29"},
            {"24", "26"},
            {"24", "31"},
            {"24", "30"},
            {"24", "28"},
            {"24", "27"},
            {"24", "24"},
            {"9", "10"},
            {"9", "11"},
            {"9", "9"},
            {"9", "12"},
    };
    vector<vector<string>> c2 = {
            {"1", "14"},
            {"1", "9"},
            {"1", "24"},
            {"1", "18"},
            {"1", "20"},
            {"2", "14"},
            {"2," "9"},
            {"2", "24"},
            {"2", "18"},
            {"2", "20"},
            {"3", "14"},
            {"3", "9"},
            {"3", "24"},
            {"3", "18"},
            {"3", "20"},
            {"4", "14"},
            {"4", "9"},
            {"4", "24"},
            {"4," "18"},
            {"4", "20"},
            {"5", "14"},
            {"5", "9"},
            {"5", "24"},
            {"5", "18"},
            {"5", "20"},
            {"6", "14"},
            {"6", "9"},
            {"6", "24"},
            {"6", "18"},
            {"6", "20"},
            {"7", "14"},
            {"7", "9"},
            {"7", "24"},
            {"7", "18"},
            {"7", "20"},
            {"8", "9"},
            {"9", "9"},
            {"10", "9"},
            {"13", "14"},
            {"13", "24"},
            {"13", "18"},
            {"13", "20"},
            {"14", "14"},
            {"14", "18"},
            {"14", "20"},
            {"15", "14"},
            {"15", "18"},
            {"15", "20"},
            {"16", "18"},
            {"16", "20"},
            {"16", "14"},
            {"17", "14"},
            {"17", "18"},
            {"17", "20"},
            {"18", "20"},
            {"18", "18"},
            {"18", "14"},
            {"19", "18"},
            {"19", "20"},
            {"19", "14"},
            {"20", "14"},
            {"20", "20"},
            {"20", "18"},
            {"21", "20"},
            {"21", "14"},
            {"21", "18"},
            {"22", "14"},
            {"22", "18"},
            {"22", "20"},
            {"23", "24"},
            {"24", "24"},
            {"25", "24"},
            {"26", "24"},
            {"27", "24"},
            {"28", "24"},
            {"36", "36"},
            {"37", "36"},
            {"38", "36"},
    };
    Table t1 = Table(h1, c1);
    Table t2 = Table(h2, c2);
    qdb.insertTable(t1);
    qdb.insertTable(t2);
    vector<string> res = qdb.fetch({
        Parameter("s", ParameterType::STMT),
        Parameter("w1", ParameterType::WHILE),
        Parameter("w2", ParameterType::WHILE),
        }, readPKB);
    REQUIRE(res.size()==91);
}