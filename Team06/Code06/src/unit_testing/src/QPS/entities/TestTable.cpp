#include <chrono>
#include <string>
#include <vector>

#include "catch.hpp"
#include "qps/entities/Table.h"
using namespace std::chrono;
using namespace std;

TEST_CASE("intersectTable / intersecting large tables / return true") {
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
    auto start = high_resolution_clock::now();
    Table t1(h1, c1);
    Table t2(h2, c2);
    t1.intersectTable(t2);
    vector<Parameter> h3 = t1.getHeaders();
    vector<vector<string>> c3 = t1.getContent();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    // Content size should be 8 * 8 * 4 = 256
    bool correctSize = h3.size() == 7 && c3.size() == 256;
    REQUIRE(correctSize);
}

TEST_CASE("intersectTable / intersection of one parameter is working / return true") {
    vector<Parameter> h1 = {Parameter("v", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT)};
    vector<vector<string>> c1 = {{"a", "1"}, {"b", "2"}, {"c", "3"}};
    vector<Parameter> h2 = {Parameter("v1", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT)};
    vector<vector<string>> c2 = {{"d", "1"}, {"e", "2"}, {"f", "4"}};
    Table t1(h1, c1);
    Table t2(h2, c2);
    t1.intersectTable(t2);
    vector<Parameter> h3 = t1.getHeaders();
    vector<vector<string>> c3 = t1.getContent();
    bool rightSize = h3.size() == 3 && c3.size() == 2;
    bool rightHeaders = h3[0].getValue() == "v" && h3[1].getValue() == "v1" && h3[2].getValue() == "s";
    REQUIRE((rightSize && rightHeaders));
}

TEST_CASE("getHeader / returns the correct vector of headers / return true") {

    vector<Parameter> headers = {Parameter("v", ParameterType::VARIABLE), Parameter("_", ParameterType::WILDCARD)};
    vector<vector<string>> content = {{"x", "5"}};
    Table table(headers, content);
    REQUIRE(headers == table.getHeaders());
}

TEST_CASE("getContent / returns the correct vector of contents / return true") {

    vector<Parameter> headers = {Parameter("v", ParameterType::VARIABLE), Parameter("_", ParameterType::WILDCARD)};
    vector<vector<string>> content = {{"x", "5"}};
    Table table(headers, content);
    REQUIRE(content == table.getContent());
}

TEST_CASE("selectColumns / the selection of single column will reduce the "
          "table to chosen column / return true") {

    vector<Parameter> headers = {Parameter("v", ParameterType::VARIABLE), Parameter("_", ParameterType::WILDCARD)};
    vector<vector<string>> content = {{"x", "5"}};
    Table table(headers, content);
    vector<int> indexes = {0};
    table.extractColumns(indexes);
    int length = table.getContent().size();
    string value = table.getContent()[0][0];
    REQUIRE(((length == 1) && (value == "x")));
}

TEST_CASE("selectColumns / select nothing / return true") {

    vector<Parameter> headers = {Parameter("v", ParameterType::VARIABLE), Parameter("_", ParameterType::WILDCARD)};
    vector<vector<string>> content = {{"x", "5"}};
    Table table(headers, content);
    vector<int> indexes = {};
    table.extractColumns(indexes);
    REQUIRE(table.getHeaders().size() == 0);
}

TEST_CASE("extractDesignEntities / test for variable and wild card / return "
          "only variable") {

    vector<Parameter> headers = {Parameter("v", ParameterType::VARIABLE), Parameter("_", ParameterType::WILDCARD)};
    vector<vector<string>> content = {{"x", "5"}};
    Table table(headers, content);
    vector<int> indexes = {};
    table.extractDesignEntities();
    REQUIRE((table.getHeaders().size() == 1 && table.getHeaders()[0].getValue() == "v"));
}

TEST_CASE("extractDesignEntities / test for stmt and fixed_int / return only stmt") {

    vector<Parameter> headers = {Parameter("s", ParameterType::STMT), Parameter("321", ParameterType::FIXED_INT)};
    vector<vector<string>> content = {{"4", "321"}};
    Table table(headers, content);
    vector<int> indexes = {};
    table.extractDesignEntities();
    REQUIRE((table.getHeaders().size() == 1 && table.getHeaders()[0].getValue() == "s"));
}

TEST_CASE("extractDesignEntities / test for fixed_string, read and stmt / "
          "returns read and stmt") {

    vector<Parameter> headers = {
        Parameter("abc", ParameterType::FIXED_STRING),
        Parameter("rd", ParameterType::READ),
        Parameter("s", ParameterType::STMT),
    };
    vector<vector<string>> content = {{"abc", "6", "7"}};
    Table table(headers, content);
    table.extractDesignEntities();
    REQUIRE(
        (table.getHeaders().size() == 2 && table.getHeaders()[0].getValue() == "rd" && table.getHeaders()[1].getValue() == "s"));
}

TEST_CASE("extractDesignEntities / test for call and fixed_string / returns call") {

    vector<Parameter> headers = {Parameter("abc", ParameterType::FIXED_STRING), Parameter("cl", ParameterType::CALL)};
    vector<vector<string>> content = {{"abc", "5"}};
    Table table(headers, content);
    vector<int> indexes = {};
    table.extractDesignEntities();
    REQUIRE((table.getHeaders().size() == 1 && table.getHeaders()[0].getValue() == "cl"));
}

TEST_CASE("extractDesignEntities / test constant, procedure and") {

    vector<Parameter> headers = {Parameter("c", ParameterType::CONSTANT), Parameter("proc", ParameterType::PROCEDURE)};
    vector<vector<string>> content = {{"999", "main"}};
    Table table(headers, content);
    table.extractDesignEntities();
    REQUIRE(
        (table.getHeaders().size() == 2 && table.getHeaders()[0].getValue() == "c" && table.getHeaders()[1].getValue() == "proc"));
}

TEST_CASE("extractDesignEntities / test synonym ifs assign / return ifs, assign") {

    vector<Parameter> headers = {
        Parameter("hello", ParameterType::SYNONYM),
        Parameter("ifs", ParameterType::IF),
        Parameter("a", ParameterType::ASSIGN),
    };
    vector<vector<string>> content = {{"hello", "5", "321"}};
    Table table(headers, content);
    vector<int> indexes = {};
    table.extractDesignEntities();
    REQUIRE(
        (table.getHeaders().size() == 2 && table.getHeaders()[0].getValue() == "ifs" && table.getHeaders()[1].getValue() == "a"));
}

TEST_CASE("cartesianProduct / simple 2 element tables / result in total 4 element table") {
    vector<Parameter> h1 = {Parameter("v1", ParameterType::VARIABLE), Parameter("s1", ParameterType::STMT)};
    vector<Parameter> h2 = {Parameter("v2", ParameterType::VARIABLE), Parameter("s2", ParameterType::STMT)};
    vector<vector<string>> c1 = {{"a", "111"}, {"b", "222"}};
    vector<vector<string>> c2 = {{"c", "333"}, {"d", "444"}};
    Table t1 = Table(h1, c1);
    Table t2 = Table(h2, c2);
    t1.cartesianProduct(t2);
    vector<Parameter> h3 = t1.getHeaders();
    vector<vector<string>> c3 = t1.getContent();
    REQUIRE(c3.size() == 4);
}

TEST_CASE("cartesianProduct / product with second table with empty content / should return an empty table") {
    vector<Parameter> h1 = {Parameter("v1", ParameterType::VARIABLE), Parameter("s1", ParameterType::STMT)};
    vector<vector<string>> c1 = {{"a", "111"}, {"b", "222"}};
    vector<Parameter> h2 = {Parameter("v2", ParameterType::VARIABLE), Parameter("s2", ParameterType::STMT)};
    vector<vector<string>> c2 = {};
    Table t1 = Table(h1, c1);
    Table t2 = Table(h2, c2);
    t1.cartesianProduct(t2);
    vector<vector<string>> c3 = t1.getContent();
    vector<Parameter> h3 = t1.getHeaders();
    REQUIRE(h3.size() == 4);
    REQUIRE(c3.empty());
}

TEST_CASE("cartesianProduct / product with first table with empty content / should return an empty table") {
    vector<Parameter> h1 = {Parameter("v1", ParameterType::VARIABLE), Parameter("s1", ParameterType::STMT)};
    vector<vector<string>> c1 = {{"a", "111"}, {"b", "222"}};
    vector<Parameter> h2 = {Parameter("v2", ParameterType::VARIABLE), Parameter("s2", ParameterType::STMT)};
    vector<vector<string>> c2 = {};
    Table t1 = Table(h1, c1);
    Table t2 = Table(h2, c2);
    t2.cartesianProduct(t1);
    vector<vector<string>> c3 = t2.getContent();
    vector<Parameter> h3 = t2.getHeaders();
    REQUIRE(h3.size() == 4);
    REQUIRE(c3.empty());
}

TEST_CASE("cartesianProduct / massive 26 element tables / result in total 4 element table") {
    vector<Parameter> h1 = {Parameter("v1", ParameterType::VARIABLE), Parameter("s1", ParameterType::STMT)};
    vector<Parameter> h2 = {Parameter("v2", ParameterType::VARIABLE), Parameter("s2", ParameterType::STMT)};
    vector<Parameter> h3 = {Parameter("v3", ParameterType::VARIABLE), Parameter("s3", ParameterType::STMT)};
    vector<vector<string>> c1 = {
        {"a", "1"}, {"b", "1"}, {"c", "1"}, {"d", "1"}, {"e", "1"}, {"f", "1"}, {"g", "1"}, {"h", "1"}, {"i", "1"},
        {"j", "1"}, {"k", "1"}, {"l", "1"}, {"m", "1"}, {"n", "1"}, {"o", "1"}, {"p", "1"}, {"q", "1"}, {"r", "1"},
        {"s", "1"}, {"t", "1"}, {"u", "1"}, {"v", "1"}, {"w", "1"}, {"x", "1"}, {"y", "1"}, {"z", "1"},
    };
    vector<vector<string>> c2 = {
        {"a", "2"}, {"b", "2"}, {"c", "2"}, {"d", "2"}, {"e", "2"}, {"f", "2"}, {"g", "2"}, {"h", "2"}, {"i", "2"},
        {"j", "2"}, {"k", "2"}, {"l", "2"}, {"m", "2"}, {"n", "2"}, {"o", "2"}, {"p", "2"}, {"q", "2"}, {"r", "2"},
        {"s", "2"}, {"t", "2"}, {"u", "2"}, {"v", "2"}, {"w", "2"}, {"x", "2"}, {"y", "2"}, {"z", "2"},
    };
    vector<vector<string>> c3 = {
        {"a", "3"}, {"b", "3"}, {"c", "3"}, {"d", "3"}, {"e", "3"}, {"f", "3"}, {"g", "3"}, {"h", "3"}, {"i", "3"},
        {"j", "3"}, {"k", "3"}, {"l", "3"}, {"m", "3"}, {"n", "3"}, {"o", "3"}, {"p", "3"}, {"q", "3"}, {"r", "3"},
        {"s", "3"}, {"t", "3"}, {"u", "3"}, {"v", "3"}, {"w", "3"}, {"x", "3"}, {"y", "3"}, {"z", "3"},
    };
    Table t1 = Table(h1, c1);
    Table t2 = Table(h2, c2);
    Table t3 = Table(h3, c3);
    t1.cartesianProduct(t2);
    t1.cartesianProduct(t3);
    vector<Parameter> h5 = t1.getHeaders();
    vector<vector<string>> c5 = t1.getContent();
    REQUIRE(h5.size() == 6);
    REQUIRE(c5.size() == 26 * 26 * 26);
}

TEST_CASE("extractColumns / extracting through using indexes / extract normally") {
    vector<Parameter> h1 = {Parameter("v", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT),
                            Parameter("a", ParameterType::ASSIGN)};
    vector<vector<string>> c1 = {{"a", "1", "11"}, {"b", "2", "22"}, {"c", "3", "33"}};
    Table t1 = Table(h1, c1);
    vector<int> indexes = {1, 2};
    t1.extractColumns(indexes);
    REQUIRE(t1.getHeaders().size() == 2);
    REQUIRE(t1.getContent().size() == 3);
}

TEST_CASE("extractColumns / extracting through using indexes with duplicates / extracted table should not have dupes") {
    vector<Parameter> h1 = {Parameter("v", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT),
                            Parameter("a", ParameterType::ASSIGN)};
    vector<vector<string>> c1 = {
        {"a", "1", "11"}, {"a", "2", "22"}, {"a", "1", "33"}, {"a", "2", "44"}, {"a", "1", "55"},
        {"a", "2", "66"}, {"a", "1", "77"}, {"a", "2", "88"}, {"a", "1", "99"}, {"a", "2", "00"},
    };
    Table t1 = Table(h1, c1);
    vector<int> indexes = {0, 1};
    t1.extractColumns(indexes);
    REQUIRE(t1.getHeaders().size() == 2);
    REQUIRE(t1.getContent().size() == 2);
}

TEST_CASE("extractColumns / extracting through using params / extract normally") {
    vector<Parameter> h1 = {Parameter("v", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT),
                            Parameter("a", ParameterType::ASSIGN)};
    vector<Parameter> extractHeaders = {Parameter("v", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT)};
    vector<vector<string>> c1 = {{"a", "1", "11"}, {"b", "2", "22"}, {"c", "3", "33"}};
    Table t1 = Table(h1, c1);
    t1.extractColumns(extractHeaders);
    REQUIRE(t1.getHeaders().size() == 2);
    REQUIRE(t1.getContent().size() == 3);
}

TEST_CASE("extractColumns / extracting through using params with duplicates / extracted table should not have dupes") {
    vector<Parameter> h1 = {Parameter("v", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT),
                            Parameter("a", ParameterType::ASSIGN)};
    vector<Parameter> extractHeaders = {Parameter("v", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT)};
    vector<vector<string>> c1 = {
        {"a", "1", "11"}, {"a", "2", "22"}, {"a", "1", "33"}, {"a", "2", "44"}, {"a", "1", "55"},
        {"a", "2", "66"}, {"a", "1", "77"}, {"a", "2", "88"}, {"a", "1", "99"}, {"a", "2", "00"},
    };
    Table t1 = Table(h1, c1);
    t1.extractColumns(extractHeaders);
    REQUIRE(t1.getHeaders().size() == 2);
    REQUIRE(t1.getContent().size() == 2);
}

TEST_CASE("updateValues / single header updates correctly the values of the table") {
    Table table({Parameter("v", ParameterType::VARIABLE)}, {{"1"}, {"2"}, {"3"}});
    unordered_map<string, string> map;
    map.insert({{"1", "2"}, {"2", "4"}, {"3", "6"}});
    Parameter p = Parameter("v", ParameterType::VARIABLE);
    table.updateValues(p, map);
    vector<vector<string>> resContent = table.getContent();
    vector<vector<string>> expected = {{"2"}, {"4"}, {"6"}};
    REQUIRE(find(resContent.begin(), resContent.end(), expected[0]) != resContent.end());
    REQUIRE(find(resContent.begin(), resContent.end(), expected[1]) != resContent.end());
    REQUIRE(find(resContent.begin(), resContent.end(), expected[2]) != resContent.end());
}

TEST_CASE("updateValues / multiple header updates correctly the values of the table") {
    Table table({Parameter("v", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT)},
                {{"1", "11"}, {"2", "22"}, {"3", "33"}});
    unordered_map<string, string> map;
    map.insert({{"1", "2"}, {"2", "4"}, {"3", "6"}});
    Parameter p = Parameter("v", ParameterType::VARIABLE);
    table.updateValues(p, map);
    vector<vector<string>> resContent = table.getContent();
    vector<vector<string>> expected = {{"2", "11"}, {"4", "22"}, {"6", "33"}};
    REQUIRE(find(resContent.begin(), resContent.end(), expected[0]) != resContent.end());
    REQUIRE(find(resContent.begin(), resContent.end(), expected[1]) != resContent.end());
    REQUIRE(find(resContent.begin(), resContent.end(), expected[2]) != resContent.end());
}