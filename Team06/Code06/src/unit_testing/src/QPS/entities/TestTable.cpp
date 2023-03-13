#include "catch.hpp"
#include "qps/entities/Table.h"
#include <string>
#include <vector>
#include <chrono>
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
            {"a", "1", "aa", "11"},
            {"b", "2", "bb", "22"},
            {"c", "3", "cc", "33"},
            {"d", "4", "dd", "44"},
            {"a", "5", "aa", "55"},
            {"b", "6", "bb", "66"},
            {"c", "7", "cc", "77"},
            {"d", "8", "dd", "88"},
            {"a", "11", "aa", "11"},
            {"b", "22", "bb", "22"},
            {"c", "33", "cc", "33"},
            {"d", "44", "dd", "44"},
            {"a", "55", "aa", "55"},
            {"b", "66", "bb", "66"},
            {"c", "77", "cc", "77"},
            {"d", "88", "dd", "88"},
            {"a", "1", "aaa", "11"},
            {"b", "2", "bbb", "22"},
            {"c", "3", "ccc", "33"},
            {"d", "4", "ddd", "44"},
            {"a", "5", "aaa", "55"},
            {"b", "6", "bbb", "66"},
            {"c", "7", "ccc", "77"},
            {"d", "8", "ddd", "88"},
            {"a", "11", "aaa", "11"},
            {"b", "22", "bbb", "22"},
            {"c", "33", "ccc", "33"},
            {"d", "44", "ddd", "44"},
            {"a", "55", "aaa", "55"},
            {"b", "66", "bbb", "66"},
            {"c", "77", "ccc", "77"},
            {"d", "88", "ddd", "88"},
    };
    vector<Parameter> h2 = {
            Parameter("v1", ParameterType::VARIABLE),
            Parameter("s2", ParameterType::STMT),
            Parameter("v", ParameterType::VARIABLE),
            Parameter("ss", ParameterType::STMT)
    };
    vector<vector<string>> c2 = {
            {"d", "1", "a", "11"},
            {"e", "2", "b", "22"},
            {"f", "3", "c", "33"},
            {"g", "4", "d", "44"},
            {"dd", "1", "a", "11"},
            {"ee", "2", "b", "22"},
            {"ff", "3", "c", "33"},
            {"gg", "4", "d", "44"},
            {"d", "1", "a", "111"},
            {"e", "2", "b", "222"},
            {"f", "3", "c", "333"},
            {"g", "4", "d", "444"},
            {"dd", "1", "a", "111"},
            {"ee", "2", "b", "222"},
            {"ff", "3", "c", "333"},
            {"gg", "4", "d", "444"},
            {"d", "11", "a", "11"},
            {"e", "22", "b", "22"},
            {"f", "33", "c", "33"},
            {"g", "44", "d", "44"},
            {"dd", "11", "a", "11"},
            {"ee", "22", "b", "22"},
            {"ff", "33", "c", "33"},
            {"gg", "44", "d", "44"},
            {"d", "11", "a", "111"},
            {"e", "22", "b", "222"},
            {"f", "33", "c", "333"},
            {"g", "44", "d", "444"},
            {"dd", "11", "a", "111"},
            {"ee", "22", "b", "222"},
            {"ff", "33", "c", "333"},
            {"gg", "44", "d", "444"},
    };
    auto start = high_resolution_clock::now();
    Table t1(h1, c1);
    Table t2(h2,c2);
    Table t3 = t1.intersectTable(t2);
    vector<Parameter> h3 = t3.getHeaders();
    vector<vector<string>> c3 = t3.getContent();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time to intersect table: " << duration.count() << endl;
    // Content size should be 8 * 8 * 4 = 256
    bool correctSize = h3.size() == 7 && c3.size() == 256;
    REQUIRE(correctSize);
}

TEST_CASE("intersectTable / intersection of one parameter is working / return true") {
    vector<Parameter> h1 = {Parameter("v", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT)};
    vector<vector<string>> c1 = {
            {"a", "1"},
            {"b", "2"},
            {"c", "3"}
    };
    vector<Parameter> h2 = {Parameter("v1", ParameterType::VARIABLE), Parameter("s", ParameterType::STMT)};
    vector<vector<string>> c2 = {
            {"d", "1"},
            {"e", "2"},
            {"f", "4"}
    };
    Table t1(h1, c1);
    Table t2(h2,c2);
    Table t3 = t1.intersectTable(t2);
    vector<Parameter> h3 = t3.getHeaders();
    vector<vector<string>> c3 = t3.getContent();
    bool rightSize = h3.size() == 3 && c3.size() == 2;
    bool rightHeaders = h3[0].getValue() == "v" && h3[1].getValue() == "v1" && h3[2].getValue() == "s";
    REQUIRE((rightSize && rightHeaders));
}

TEST_CASE("getHeader / returns the correct vector of headers / return true") {

  vector<Parameter> headers = {Parameter("v", AppConstants::VARIABLE),
                               Parameter("_", AppConstants::WILDCARD)};
  vector<vector<string>> content = {{"x", "5"}};
  Table table(headers, content);
  REQUIRE(headers == table.getHeaders());
}

TEST_CASE("getContent / returns the correct vector of contents / return true") {

  vector<Parameter> headers = {Parameter("v", AppConstants::VARIABLE),
                               Parameter("_", AppConstants::WILDCARD)};
  vector<vector<string>> content = {{"x", "5"}};
  Table table(headers, content);
  REQUIRE(content == table.getContent());
}

//TEST_CASE("selectColumns / the selection of single column will reduce the "
//          "table to chosen column / return true") {
//
//  vector<Parameter> headers = {Parameter("v", AppConstants::VARIABLE),
//                               Parameter("_", AppConstants::WILDCARD)};
//  vector<vector<string>> content = {{"x", "5"}};
//  Table table(headers, content);
//  vector<int> indexes = {0};
//  Table t = table.selectColumns(indexes);
//  int length = t.getContent().size();
//  string value = t.getContent()[0][0];
//  REQUIRE(((length == 1) && (value == "x")));
//}
//
//TEST_CASE("selectColumns / select nothing / return true") {
//
//  vector<Parameter> headers = {Parameter("v", AppConstants::VARIABLE),
//                               Parameter("_", AppConstants::WILDCARD)};
//  vector<vector<string>> content = {{"x", "5"}};
//  Table table(headers, content);
//  vector<int> indexes = {};
//  Table t = table.selectColumns(indexes);
//  REQUIRE(t.getHeaders().size() == 0);
//}

TEST_CASE("extractDesignEntities / test for variable and wild card / return "
          "only variable") {

  vector<Parameter> headers = {Parameter("v", AppConstants::VARIABLE),
                               Parameter("_", AppConstants::WILDCARD)};
  vector<vector<string>> content = {{"x", "5"}};
  Table table(headers, content);
  vector<int> indexes = {};
  Table t = table.extractDesignEntities();
  REQUIRE((t.getHeaders().size() == 1 && t.getHeaders()[0].getValue() == "v"));
}

TEST_CASE(
    "extractDesignEntities / test for stmt and fixed_int / return only stmt") {

  vector<Parameter> headers = {Parameter("s", AppConstants::STMT),
                               Parameter("321", AppConstants::FIXED_INT)};
  vector<vector<string>> content = {{"4", "321"}};
  Table table(headers, content);
  vector<int> indexes = {};
  Table t = table.extractDesignEntities();
  REQUIRE((t.getHeaders().size() == 1 && t.getHeaders()[0].getValue() == "s"));
}

TEST_CASE("extractDesignEntities / test for fixed_string, read and stmt / "
          "returns read and stmt") {

  vector<Parameter> headers = {
      Parameter("abc", AppConstants::FIXED_STRING),
      Parameter("rd", AppConstants::READ),
      Parameter("s", AppConstants::STMT),
  };
  vector<vector<string>> content = {{"abc", "6", "7"}};
  Table table(headers, content);
  Table t = table.extractDesignEntities();
  REQUIRE((t.getHeaders().size() == 2 && t.getHeaders()[0].getValue() == "rd" &&
           t.getHeaders()[1].getValue() == "s"));
}

TEST_CASE(
    "extractDesignEntities / test for call and fixed_string / returns call") {

  vector<Parameter> headers = {Parameter("abc", AppConstants::FIXED_STRING),
                               Parameter("cl", AppConstants::CALL)};
  vector<vector<string>> content = {{"abc", "5"}};
  Table table(headers, content);
  vector<int> indexes = {};
  Table t = table.extractDesignEntities();
  REQUIRE((t.getHeaders().size() == 1 && t.getHeaders()[0].getValue() == "cl"));
}

TEST_CASE("extractDesignEntities / test constant, procedure and") {

  vector<Parameter> headers = {
      Parameter("c", AppConstants::CONSTANT),
      Parameter("proc", AppConstants::PROCEDURE)};
  vector<vector<string>> content = {{"999", "main"}};
  Table table(headers, content);
  Table t = table.extractDesignEntities();
  REQUIRE((t.getHeaders().size() == 2 && t.getHeaders()[0].getValue() == "c" &&
           t.getHeaders()[1].getValue() == "proc"));
}

TEST_CASE(
    "extractDesignEntities / test synonym ifs assign / return ifs, assign") {

  vector<Parameter> headers = {
      Parameter("hello", AppConstants::SYNONYM),
      Parameter("ifs", AppConstants::IF),
      Parameter("a", AppConstants::ASSIGN),
  };
  vector<vector<string>> content = {{"hello", "5", "321"}};
  Table table(headers, content);
  vector<int> indexes = {};
  Table t = table.extractDesignEntities();
  REQUIRE((t.getHeaders().size() == 2 &&
           t.getHeaders()[0].getValue() == "ifs" &&
           t.getHeaders()[1].getValue() == "a"));
}
