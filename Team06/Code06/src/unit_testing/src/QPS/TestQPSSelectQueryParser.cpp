#include "QPS/parser/SelectQueryParser.h"
#include "catch.hpp"
#include "exceptions/SyntaxException.h"
#include <string>
using namespace std;

TEST_CASE("parse / sample select query string / returns query object") {
  string input = "Select s     such    that    Follows(sass,11234)";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE("parse / Follows query does not have enough params / throws error") {
  string input = "Select s     such    that    Follows(sass)";
  SelectQueryParser sqp;
  CHECK_THROWS(sqp.parse(input));
}

TEST_CASE("parse / Follows query has too many params / throws error") {
  string input = "Select s     such    that    Follows(sass, asd, dad, doo)";
  SelectQueryParser sqp;
  CHECK_THROWS(sqp.parse(input));
}

TEST_CASE("parse / Rel ref is nonsense / throws error") {
  string input = "Select s     such    that  Teaches(sass, asd, dad, doo)";
  SelectQueryParser sqp;
  CHECK_THROWS(sqp.parse(input));
}

TEST_CASE("parse / empty string select clause / throws exception") {
  string input = " ";
  SelectQueryParser sqp;
  CHECK_THROWS(sqp.parse(input));
}

TEST_CASE("parse / nonsense string select clause / throws exception") {
  string input = "Hello World";
  SelectQueryParser sqp;
  CHECK_THROWS(sqp.parse(input));
}

TEST_CASE("parse / small s select clause / throws exception") {
  string input = "select v";
  SelectQueryParser sqp;
  CHECK_THROWS(sqp.parse(input));
}

TEST_CASE("parse / multiple such that clauses / returns query object") {
  string input = "Select s such that Follows(a, b) such that Follows(b, c)";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE("parse / multiple pattern clauses / returns query object") {
  string input = "Select s pattern a(a, _) pattern a(c, _)";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE(
    "parse / single pattern clause, multiple ands / return query object") {
  string input = "Select s pattern a(a, _) and a(c, _)";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE(
    "parse / single pattern clause, with dummy and / return query object") {
  string input = "Select s pattern a(and, _)";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE("parse / single ifs pattern clause, with dummy variable v / return "
          "ifs pattern query object") {
  string input = "Select s pattern ifs(v, _, _)";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE("parse / single ifs pattern clause, with third parameter not wild "
          "card / expect syntax error") {
  // The third parameter MUST be a wild card if it exists.
  string input = "Select s pattern ifs(v, _, sheeHuiBestTA)";
  // error because of SheeHui
  SelectQueryParser sqp;
  CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / missing third  / expect syntax error") {
  // The third parameter MUST be a wild card if it exists.
  string input = "Select s pattern ifs(v, _,)";
  SelectQueryParser sqp;
  CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / single ifs pattern clause, with second parameter not wild "
          "card / expect syntax error") {
  // The second parameter can either be an ExprSpec or a wild card
  string input = "Select s pattern ifs(v, sheeHuiBestTA, _)";
  SelectQueryParser sqp;
  CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / single pattern clause, with multiple dummy ands / return "
          "query object") {
  string input = "Select s pattern and(and, \"(and) * and\")";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE("parse / single pattern clause, with multiple dummy ands and true "
          "ands / return query object") {
  string input =
      "Select s pattern and(and, \"(and) * and\") and c(b, \"(and)\")";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE("parse / multiple pattern clause, with multiple dummy ands and true "
          "ands / return query object") {
  string input = "Select s pattern and(and, \"(and) * and\") and c(b, "
                 "\"(and)\") pattern and(a, \"b\") and yes(a, _)";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE("parse / single such that clauses with and/ returns query object") {
  string input = "Select s such that Follows(a, b) and Follows(b, c)";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE("parse / single such that clauses with dummy ands and true ands/ "
          "returns query object") {
  string input =
      "Select s such that Follows(and, and) and Modifies(c, \"and\")";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE("parse / multiple such that clauses with dummy ands and true ands/ "
          "returns query object") {
  string input = "Select s such that Follows(and, and) and Modifies(c, "
                 "\"and\") such that Modifies(d, \"and\") and Follows(a, b)";
  SelectQueryParser sqp;
  Query q = sqp.parse(input);
  CHECK(true);
}

TEST_CASE(
    "parseParameter / correct synonym, no attribute / returns Parameter") {
  SelectQueryParser sqp;
  string inputString = "thisIsIdent123";
  Parameter expected = Parameter(inputString, ParameterType::SYNONYM);
  Parameter output = sqp.parseParameter(inputString);

  CHECK(expected == output);
}

TEST_CASE(
    "parseParameter / correct synonym, with attribute / returns Parameter") {
  SelectQueryParser sqp;
  string inputName = "thisIsIdent123";
  string inputAttribute = AppConstants::PROCNAME;
  string inputString = inputName + "." + inputAttribute;

  Parameter expected =
      Parameter(inputName, ParameterType::SYNONYM, AppConstants::PROCNAME);
  Parameter output = sqp.parseParameter(inputString);

  CHECK(expected == output);
}

TEST_CASE("parseParameter / correct synonym, ends with ., no attribute / "
          "throws error") {
  SelectQueryParser sqp;
  string inputName = "thisIsIdent123";
  string inputAttribute = "";
  string inputString = inputName + "." + inputAttribute;

  CHECK_THROWS_AS(sqp.parseParameter(inputString), SyntaxException);
}

TEST_CASE(
    "parseParameter / correct synonym, invalid attribute string / "
    "throws error") {
  SelectQueryParser sqp;
  string inputName = "thisIsIdent123";
  string inputAttribute = "invalid";
  string inputString = inputName + "." + inputAttribute;

  CHECK_THROWS_AS(sqp.parseParameter(inputString), SyntaxException);
}

TEST_CASE(
    "parseParameter / incorrect synonym, valid attribute / "
    "throws error") {
  SelectQueryParser sqp;
  string inputName = "*notIdent123";
  string inputAttribute = AppConstants::PROCNAME;
  string inputString = inputName + "." + inputAttribute;

  CHECK_THROWS_AS(sqp.parseParameter(inputString), SyntaxException);
}