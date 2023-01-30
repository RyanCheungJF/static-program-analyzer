#include "catch.hpp"
#include "qps/QPSGrammarUtils.h"
using namespace std;

// isName Test Cases

TEST_CASE("IsName / Single letter is valid name / return true") {
    string name = "a";
    REQUIRE(isName(name));
}

TEST_CASE("IsName / Multiple letter name / return true") {
    string name = "apple";
    REQUIRE(isName(name));
}

TEST_CASE("IsName / Name is combination of upper and lower case / return true") {
    string name = "APpLe";
    REQUIRE(isName(name));
}

TEST_CASE("IsName / name is combination of letters and numbers / return true") {
    string name = "Br0Th4";
    REQUIRE(isName(name));
}

TEST_CASE("IsName / name cannot start with a number / return false") {
    string name = "4pple";
    REQUIRE(!isName(name));
}

TEST_CASE("IsName / name cannot contain non letters and numbers / return false") {
    string name = "app_le";
    REQUIRE(!isName(name));
}

TEST_CASE("IsName / empty string is not a name / return false") {
    string name = "";
    REQUIRE(!isName(name));
}

// isIdent Test Cases

TEST_CASE("IsIdent / Single letter is valid ident / return true") {
    string name = "a";
    REQUIRE(isIdent(name));
}

TEST_CASE("IsIdent / Multiple letter ident / return true") {
    string name = "apple";
    REQUIRE(isIdent(name));
}

TEST_CASE("IsIdent / ident is combination of upper and lower case / return true") {
    string name = "APpLe";
    REQUIRE(isIdent(name));
}

TEST_CASE("IsIdent / ident is combination of letters and numbers / return true") {
    string name = "Br0Th4";
    REQUIRE(isIdent(name));
}

TEST_CASE("IsIdent / ident cannot start with a number / return false") {
    string name = "4pple";
    REQUIRE(!isIdent(name));
}

TEST_CASE("IsIdent / ident cannot contain non letters and numbers / return false") {
    string name = "app_le";
    REQUIRE(!isIdent(name));
}

TEST_CASE("IsIdent / empty string is not an ident / return false") {
    string name = "";
    REQUIRE(!isIdent(name));
}

// isSynonym Test Cases

TEST_CASE("isSynonym / Single letter is valid synonym / return true") {
    string name = "a";
    REQUIRE(isSynonym(name));
}

TEST_CASE("isSynonym / Multiple letter synonym / return true") {
    string name = "apple";
    REQUIRE(isSynonym(name));
}

TEST_CASE("isSynonym / synonym is combination of upper and lower case / return true") {
    string name = "APpLe";
    REQUIRE(isSynonym(name));
}

TEST_CASE("isSynonym / synonym is combination of letters and numbers / return true") {
    string name = "Br0Th4";
    REQUIRE(isSynonym(name));
}

TEST_CASE("isSynonym / synonym cannot start with a number / return false") {
    string name = "4pple";
    REQUIRE(!isSynonym(name));
}

TEST_CASE("isSynonym / synonym cannot contain non letters and numbers / return false") {
    string name = "app_le";
    REQUIRE(!isSynonym(name));
}

TEST_CASE("isSynonym / empty string is not a synonym / return false") {
    string name = "";
    REQUIRE(!isSynonym(name));
}

// isInteger Test Cases

TEST_CASE("isInteger / single digit is an integer / return true") {
    string integer = "1";
    REQUIRE(isInteger(integer));
}

TEST_CASE("isInteger / multiple digit is an integer / return true") {
    string integer = "11111";
    REQUIRE(isInteger(integer));
}

TEST_CASE("isInteger / numbers and letters is not an integer / return false") {
    string integer = "1LLL";
    REQUIRE(!isInteger(integer));
}

TEST_CASE("isInteger / emtpy string is not an integer / return false") {
    string integer = "";
    REQUIRE(!isInteger(integer));
}

TEST_CASE("isInteger / integer does not start with zero / return false") {
    string integer = "012";
    REQUIRE(!isInteger(integer));
}

// isSelect Test Cases

TEST_CASE("isSelect / Select is a select statement / return true") {
    string s = "Select";
    REQUIRE(isSelect(s));
}

TEST_CASE("isSelect / empty string is not a select statement / return false") {
    string s = "";
    REQUIRE(!isSelect(s));
}

// Check with LY on why select is strictly ^Select$

//TEST_CASE("isSelect / Select ___ is a select statement / return true") {
//    string s = "Select aaaaaaaaaaa";
//    REQUIRE(isSelect(s));
//}

TEST_CASE("isSelect / ___ Select is not a select statement / return false") {
    string s = "aaa Select";
    REQUIRE(!isSelect(s));
}

TEST_CASE("isSelect / select with small s is NOT a select statement / return false") {
    string s = "select";
    REQUIRE(!isSelect(s));
}

// isPattern Test Cases

TEST_CASE("isPattern / pattern is a valid pattern / return true") {
    string s = "pattern";
    REQUIRE(isPattern(s));
}

TEST_CASE("isPattern / Pattern is an invalid pattern / return false") {
    string s = "Pattern";
    REQUIRE(!isPattern(s));
}

TEST_CASE("isPattern / empty string is not a valid pattern / return false") {
    string s = "";
    REQUIRE(!isPattern(s));
}

// isDesignEntity Test Cases

TEST_CASE("isDesignEntity / empty string is not a design entity / return false") {
    string s = "";
    REQUIRE(!isDesignEntity(s));
}

TEST_CASE("isDesignEntity / stmt is a design entity / return true") {
    string s = "stmt s";
    REQUIRE(isDesignEntity(s));
}

// isEntRef Test Cases

TEST_CASE("isEntRef /  / return true") {
    string s = "Whatthe0fuck";
    REQUIRE(isEntRef(s));
}
