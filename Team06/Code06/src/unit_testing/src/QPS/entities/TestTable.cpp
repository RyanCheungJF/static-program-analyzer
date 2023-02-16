#include "catch.hpp"
#include "qps/entities/Table.h"
#include <vector>
#include <string>
using namespace std;

TEST_CASE("getHeader / returns the correct vector of headers / return true") {
    vector<Parameter> headers = {Parameter("v", "variable"), Parameter("_", "wildcard")};
    vector<vector<string>> content = {{"x", "5"}};
    Table table(headers, content);
    REQUIRE(headers == table.getHeaders());
}

TEST_CASE("getContent / returns the correct vector of contents / return true") {
    vector<Parameter> headers = {Parameter("v", "variable"), Parameter("_", "wildcard")};
    vector<vector<string>> content = {{"x", "5"}};
    Table table(headers, content);
    REQUIRE(content == table.getContent());
}

TEST_CASE("selectColumns / the selection of single column will reduce the table to chosen column / return true") {
    vector<Parameter> headers = {Parameter("v", "variable"), Parameter("_", "wildcard")};
    vector<vector<string>> content = {{"x", "5"}};
    Table table(headers, content);
    vector<int> indexes = {0};
    Table t = table.selectColumns(indexes);
    int length = t.getContent().size();
    string value = t.getContent()[0][0];
    REQUIRE(((length == 1) && (value == "x")));
}

TEST_CASE("selectColumns / select nothing / return true") {
    vector<Parameter> headers = {Parameter("v", "variable"), Parameter("_", "wildcard")};
    vector<vector<string>> content = {{"x", "5"}};
    Table table(headers, content);
    vector<int> indexes = {};
    Table t = table.selectColumns(indexes);
    REQUIRE(t.getHeaders().size() == 0);
}

TEST_CASE("extractDesignEntities / test for variable and wild card / return only variable") {
    vector<Parameter> headers = {Parameter("v", "variable"),
                                 Parameter("_", "wildcard")
                                 };
    vector<vector<string>> content = {{"x", "5"}};
    Table table(headers, content);
    vector<int> indexes = {};
    Table t = table.extractDesignEntities();
    REQUIRE((t.getHeaders().size() == 1 && t.getHeaders()[0].getValue() == "v"));
}

TEST_CASE("extractDesignEntities / test for stmt and fixed_int / return only stmt") {
    vector<Parameter> headers = {Parameter("s", "stmt"),
                                 Parameter("321", "fixed_int")
                                };
    vector<vector<string>> content = {{"4", "321"}};
    Table table(headers, content);
    vector<int> indexes = {};
    Table t = table.extractDesignEntities();
    REQUIRE((t.getHeaders().size() == 1 && t.getHeaders()[0].getValue() == "s"));
}

TEST_CASE("extractDesignEntities / test for fixed_string, read and stmt / returns read and stmt") {
    vector<Parameter> headers = {Parameter("abc", "fixed_string"),
                                 Parameter("rd", "read"),
                                 Parameter("s", "stmt"),
    };
    vector<vector<string>> content = {{"abc", "6", "7"}};
    Table table(headers, content);
    Table t = table.extractDesignEntities();
    REQUIRE((t.getHeaders().size() == 2 && t.getHeaders()[0].getValue() == "rd" && t.getHeaders()[1].getValue() == "s"));
}

TEST_CASE("extractDesignEntities / test for call and fixed_string / returns call") {
    vector<Parameter> headers = {Parameter("abc", "fixed_string"),
                                 Parameter("cl", "call")
    };
    vector<vector<string>> content = {{"abc", "5"}};
    Table table(headers, content);
    vector<int> indexes = {};
    Table t = table.extractDesignEntities();
    REQUIRE((t.getHeaders().size() == 1 && t.getHeaders()[0].getValue() == "cl"));
}

TEST_CASE("extractDesignEntities / test constant, procedure and fixed_string_with_wildcard") {
    vector<Parameter> headers = {Parameter("c", "constant"),
                                 Parameter("proc", "procedure"),
                                 Parameter("_\"a\"_", "fixed_string_with_wildcard")
    };
    vector<vector<string>> content = {{"999", "main", "x+a"}};
    Table table(headers, content);
    Table t = table.extractDesignEntities();
    REQUIRE((t.getHeaders().size() == 2 && t.getHeaders()[0].getValue() == "c" && t.getHeaders()[1].getValue() == "proc"));
}

TEST_CASE("extractDesignEntities / test synonym ifs assign / return ifs, assign") {
    vector<Parameter> headers = {Parameter("hello", "synonym"),
                                 Parameter("ifs", "if"),
                                 Parameter("a", "assign"),
    };
    vector<vector<string>> content = {{"hello", "5", "321"}};
    Table table(headers, content);
    vector<int> indexes = {};
    Table t = table.extractDesignEntities();
    REQUIRE((t.getHeaders().size() == 2 && t.getHeaders()[0].getValue() == "ifs" && t.getHeaders()[1].getValue() == "a"));
}


