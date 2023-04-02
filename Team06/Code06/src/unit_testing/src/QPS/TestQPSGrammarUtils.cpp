#include "QPS/QPSGrammarUtils.h"
#include "catch.hpp"
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

TEST_CASE("isSynonym / synonym is combination of upper and lower case / return "
          "true") {
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

TEST_CASE("isSynonym / synonym cannot contain non letters and numbers / return "
          "false") {
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
    string s = "stmt";
    REQUIRE(isDesignEntity(s));
}

TEST_CASE("isDesignEntity / stmts is NOT a design entity / return true") {
    string s = "stmts";
    REQUIRE(!isDesignEntity(s));
}

// isEntRef Test Cases

TEST_CASE("isEntRef /  / return true") {
    string s = "Whatthe0fuck";
    REQUIRE(isEntRef(s));
}

// isDeclaration

TEST_CASE("isDeclaration / variable v / is valid declaration") {
    string s = "variable v";
    REQUIRE(isDeclaration(s));
}

TEST_CASE("isDeclaration / variable a, b, c, d / is valid declaration") {
    string s = "variable a, b, c, d";
    REQUIRE(isDeclaration(s));
}

TEST_CASE("isDeclaration / variable 00 / is NOT valid declaration") {
    string s = "variable 00";
    REQUIRE(!isDeclaration(s));
}

TEST_CASE("isDeclaration / variable v, 00 / is NOT valid declaration") {
    string s = "variable v, 00";
    REQUIRE(!isDeclaration(s));
}

TEST_CASE("isDeclaration / vari able v / is NOT valid declaration") {
    string s = "vari able v";
    REQUIRE(!isDeclaration(s));
}

TEST_CASE("isDeclaration / variablee v / is NOT valid declaration") {
    string s = "variablee v";
    REQUIRE(!isDeclaration(s));
}

TEST_CASE("isDeclaration / variable v a / is NOT valid declaration") {
    string s = "variable v a";
    REQUIRE(!isDeclaration(s));
}

TEST_CASE("hasCorrectAttrCompForm / correct form / return true") {
    string s = "asd . stmt# = 123";
    REQUIRE(hasCorrectAttrCompForm(s));
}

TEST_CASE("hasCorrectAttrCompForm / missing right hand param / return false") {
    string s = "asd . stmt# = ";
    REQUIRE(!hasCorrectAttrCompForm(s));
}

TEST_CASE("hasCorrectAttrCompForm / missing left hand param / return false") {
    string s = " = \"ks\"";
    REQUIRE(!hasCorrectAttrCompForm(s));
}

TEST_CASE("hasCorrectAttrCompForm / missing comparator / return false") {
    string s = "asd . stmt#";
    REQUIRE(!hasCorrectAttrCompForm(s));
}

TEST_CASE("hasCorrectAttrCompForm / invalid whitespace in right param / return false") {
    string s = "asd . stmt# = k s";
    REQUIRE(!hasCorrectAttrCompForm(s));
}