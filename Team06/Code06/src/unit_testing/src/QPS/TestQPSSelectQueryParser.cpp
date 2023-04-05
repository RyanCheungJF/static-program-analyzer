#include <string>

#include "QPS/parser/SelectQueryParser.h"
#include "catch.hpp"
#include "exceptions/SyntaxException.h"
using namespace std;

TEST_CASE("parse / sample select query string / returns query object") {
    string input = "Select s     such    that    Follows(sass,11234)";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    Parameter rel1Param1("sass", ParameterType::SYNONYM);
    Parameter rel1Param2("11234", ParameterType::FIXED_INT);
    vector<Parameter> rel1Params{rel1Param1, rel1Param2};
    vector<shared_ptr<Relationship>> relationships{Relationship::makeRelationship(AppConstants::FOLLOWS, rel1Params)};
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
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
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    Parameter rel1Param1("a", ParameterType::SYNONYM);
    Parameter rel1Param2("b", ParameterType::SYNONYM);
    vector<Parameter> rel1Params{rel1Param1, rel1Param2};
    Parameter rel2Param1("b", ParameterType::SYNONYM);
    Parameter rel2Param2("c", ParameterType::SYNONYM);
    vector<Parameter> rel2Params{rel2Param1, rel2Param2};
    vector<shared_ptr<Relationship>> relationships{Relationship::makeRelationship(AppConstants::FOLLOWS, rel1Params),
                                                   Relationship::makeRelationship(AppConstants::FOLLOWS, rel2Params)};
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / multiple pattern clauses / returns query object") {
    string input = "Select s pattern a(a, _) pattern a(c, _)";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    Parameter pat1Syn("a", ParameterType::SYNONYM);
    Parameter pat1Ent("a", ParameterType::SYNONYM);
    vector<string> pat1Expr{"_"};
    Parameter pat2Syn("a", ParameterType::SYNONYM);
    Parameter pat2Ent("c", ParameterType::SYNONYM);
    vector<string> pat2Expr{"_"};
    vector<Pattern> patterns{Pattern(pat1Syn, pat1Ent, pat1Expr), Pattern(pat2Syn, pat2Ent, pat2Expr)};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / single pattern clause, multiple ands / return query object") {
    string input = "Select s pattern a(a, _) and a(c, _)";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    Parameter pat1Syn("a", ParameterType::SYNONYM);
    Parameter pat1Ent("a", ParameterType::SYNONYM);
    vector<string> pat1Expr{"_"};
    Parameter pat2Syn("a", ParameterType::SYNONYM);
    Parameter pat2Ent("c", ParameterType::SYNONYM);
    vector<string> pat2Expr{"_"};
    vector<Pattern> patterns{Pattern(pat1Syn, pat1Ent, pat1Expr), Pattern(pat2Syn, pat2Ent, pat2Expr)};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / single pattern clause, with dummy and / return query object") {
    string input = "Select s pattern a(and, _)";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    Parameter pat1Syn("a", ParameterType::SYNONYM);
    Parameter pat1Ent("and", ParameterType::SYNONYM);
    vector<string> pat1Expr{"_"};
    vector<Pattern> patterns{Pattern(pat1Syn, pat1Ent, pat1Expr)};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / single ifs pattern clause, with dummy variable v / return "
          "ifs pattern query object") {
    string input = "Select s pattern ifs(v, _, _)";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    Parameter pat1Syn("ifs", ParameterType::SYNONYM);
    Parameter pat1Ent("v", ParameterType::SYNONYM);
    vector<string> pat1Expr{"_", "_"};
    vector<Pattern> patterns{Pattern(pat1Syn, pat1Ent, pat1Expr)};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / single ifs pattern clause, with third parameter not wild "
          "card / expect syntax error") {
    // The third parameter MUST be a wild card if it exists.
    string input = "Select s pattern ifs(v, _, sheeHuiBestTA)";
    // error because of SheeHui
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / missing third  / syntax Error") {
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
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    Parameter pat1Syn("and", ParameterType::SYNONYM);
    Parameter pat1Ent("and", ParameterType::SYNONYM);
    vector<string> pat1Expr{"(and)*and"};
    vector<Pattern> patterns{Pattern(pat1Syn, pat1Ent, pat1Expr)};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / single pattern clause, with multiple dummy ands and true "
          "ands / return query object") {
    string input = "Select s pattern and(and, \"(and) * and\") and c(b, \"(and)\")";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    Parameter pat1Syn("and", ParameterType::SYNONYM);
    Parameter pat1Ent("and", ParameterType::SYNONYM);
    vector<string> pat1Expr{"(and)*and"};
    Parameter pat2Syn("c", ParameterType::SYNONYM);
    Parameter pat2Ent("b", ParameterType::SYNONYM);
    vector<string> pat2Expr{"(and)"};
    vector<Pattern> patterns{Pattern(pat1Syn, pat1Ent, pat1Expr), Pattern(pat2Syn, pat2Ent, pat2Expr)};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / multiple pattern clause, with multiple dummy ands and true "
          "ands / return query object") {
    string input = "Select s pattern and(and, \"(and) * and\") and c(b, "
                   "\"(and)\") pattern and(a, \"b\") and yes(a, _)";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    Parameter pat1Syn("and", ParameterType::SYNONYM);
    Parameter pat1Ent("and", ParameterType::SYNONYM);
    vector<string> pat1Expr{"(and)*and"};
    Parameter pat2Syn("c", ParameterType::SYNONYM);
    Parameter pat2Ent("b", ParameterType::SYNONYM);
    vector<string> pat2Expr{"(and)"};
    Parameter pat3Syn("and", ParameterType::SYNONYM);
    Parameter pat3Ent("a", ParameterType::SYNONYM);
    vector<string> pat3Expr{"b"};
    Parameter pat4Syn("yes", ParameterType::SYNONYM);
    Parameter pat4Ent("a", ParameterType::SYNONYM);
    vector<string> pat4Expr{"_"};
    vector<Pattern> patterns{Pattern(pat1Syn, pat1Ent, pat1Expr), Pattern(pat2Syn, pat2Ent, pat2Expr),
                             Pattern(pat3Syn, pat3Ent, pat3Expr), Pattern(pat4Syn, pat4Ent, pat4Expr)};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / single such that clauses with and/ returns query object") {
    string input = "Select s such that Follows(a, b) and Follows(b, c)";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    Parameter rel1Param1("a", ParameterType::SYNONYM);
    Parameter rel1Param2("b", ParameterType::SYNONYM);
    vector<Parameter> rel1Params{rel1Param1, rel1Param2};
    Parameter rel2Param1("b", ParameterType::SYNONYM);
    Parameter rel2Param2("c", ParameterType::SYNONYM);
    vector<Parameter> rel2Params{rel2Param1, rel2Param2};
    vector<shared_ptr<Relationship>> relationships{Relationship::makeRelationship(AppConstants::FOLLOWS, rel1Params),
                                                   Relationship::makeRelationship(AppConstants::FOLLOWS, rel2Params)};
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / single such that clauses with dummy ands and true ands/ "
          "returns query object") {
    string input = "Select s such that Follows(and, and) and Modifies(c, \"and\")";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    Parameter rel1Param1("and", ParameterType::SYNONYM);
    Parameter rel1Param2("and", ParameterType::SYNONYM);
    vector<Parameter> rel1Params{rel1Param1, rel1Param2};
    Parameter rel2Param1("c", ParameterType::SYNONYM);
    Parameter rel2Param2("and", ParameterType::FIXED_STRING);
    vector<Parameter> rel2Params{rel2Param1, rel2Param2};
    vector<shared_ptr<Relationship>> relationships{Relationship::makeRelationship(AppConstants::FOLLOWS, rel1Params),
                                                   Relationship::makeRelationship(AppConstants::MODIFIES, rel2Params)};
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / multiple such that clauses with dummy ands and true ands/ "
          "returns query object") {
    string input = "Select s such that Follows(and, and) and Modifies(c, "
                   "\"and\") such that Modifies(d, \"and\") and Follows(a, b)";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    Parameter rel1Param1("and", ParameterType::SYNONYM);
    Parameter rel1Param2("and", ParameterType::SYNONYM);
    vector<Parameter> rel1Params{rel1Param1, rel1Param2};
    Parameter rel2Param1("c", ParameterType::SYNONYM);
    Parameter rel2Param2("and", ParameterType::FIXED_STRING);
    vector<Parameter> rel2Params{rel2Param1, rel2Param2};
    Parameter rel3Param1("d", ParameterType::SYNONYM);
    Parameter rel3Param2("and", ParameterType::FIXED_STRING);
    vector<Parameter> rel3Params{rel3Param1, rel3Param2};
    Parameter rel4Param1("a", ParameterType::SYNONYM);
    Parameter rel4Param2("b", ParameterType::SYNONYM);
    vector<Parameter> rel4Params{rel4Param1, rel4Param2};
    vector<shared_ptr<Relationship>> relationships{Relationship::makeRelationship(AppConstants::FOLLOWS, rel1Params),
                                                   Relationship::makeRelationship(AppConstants::MODIFIES, rel2Params),
                                                   Relationship::makeRelationship(AppConstants::MODIFIES, rel3Params),
                                                   Relationship::makeRelationship(AppConstants::FOLLOWS, rel4Params)};
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / pattern pattern pattern clause/ "
          "returns query object") {
    string input = "Select pattern pattern pattern( pattern, \"pattern\")";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("pattern", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    Parameter pat1Syn("pattern", ParameterType::SYNONYM);
    Parameter pat1Ent("pattern", ParameterType::SYNONYM);
    vector<string> pat1Expr{"pattern"};
    vector<Pattern> patterns{Pattern(pat1Syn, pat1Ent, pat1Expr)};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / with with with clause/ "
          "returns query object") {
    string input = "Select with with \"with\" = \"with\"";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("with", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    vector<Pattern> patterns{};
    Parameter comp1Left("with", ParameterType::FIXED_STRING);
    Parameter comp1Right("with", ParameterType::FIXED_STRING);
    vector<Comparison> comparisons{Comparison(ComparisonOperator::EQUALS, comp1Left, comp1Right)};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parseParameter / correct synonym, no attribute / returns Parameter") {
    SelectQueryParser sqp;
    string inputString = "thisIsIdent123";
    Parameter expected = Parameter(inputString, ParameterType::SYNONYM);
    Parameter output = sqp.parseParameter(inputString);

    CHECK(expected == output);
}

TEST_CASE("parseParameter / correct synonym, with attribute / returns Parameter") {
    SelectQueryParser sqp;
    string inputName = "thisIsIdent123";
    string inputAttribute = AppConstants::PROCNAME;
    string inputString = inputName + "." + inputAttribute;

    Parameter expected = Parameter(inputName, ParameterType::SYNONYM, AttributeType::PROCNAME);
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

TEST_CASE("parseParameter / only . / "
          "throws error") {
    SelectQueryParser sqp;
    string inputName = "";
    string inputAttribute = "";
    string inputString = inputName + "." + inputAttribute;

    CHECK_THROWS_AS(sqp.parseParameter(inputString), SyntaxException);
}

TEST_CASE("parseParameter / correct synonym, invalid attribute string / "
          "throws error") {
    SelectQueryParser sqp;
    string inputName = "thisIsIdent123";
    string inputAttribute = "invalid";
    string inputString = inputName + "." + inputAttribute;

    CHECK_THROWS_AS(sqp.parseParameter(inputString), SyntaxException);
}

TEST_CASE("parseParameter / incorrect synonym, valid attribute / "
          "throws error") {
    SelectQueryParser sqp;
    string inputName = "*notIdent123";
    string inputAttribute = AppConstants::PROCNAME;
    string inputString = inputName + "." + inputAttribute;

    CHECK_THROWS_AS(sqp.parseParameter(inputString), SyntaxException);
}

TEST_CASE("parse / select clause parameter with valid attributes / return query") {
    string input = "Select s.stmt#";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM, AttributeType::STMTNO)};
    vector<shared_ptr<Relationship>> relationships{};
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / select clause parameter with valid attributes and valid spaces / return query") {
    string input = "Select s . stmt#";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM, AttributeType::STMTNO)};
    vector<shared_ptr<Relationship>> relationships{};
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / select clause parameter with valid attributes and invalid spaces / throw syntax error") {
    string input = "Select s s. stmt#";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / select clause parameter with invalid attributes / throw syntax error") {
    string input = "Select s.invalidAttribute";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / select clause parameter with . but no attributes / throw syntax error") {
    string input = "Select s.";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / select clause only . / throw syntax error") {
    string input = "Select .";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / space in between parameter name / throws syntax error") {
    string input = "Select s s";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / space in between parameter name in such that clause / throws syntax error") {
    string input = "Select s such that Follows(s s, d)";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / space in between relationship name in such that clause / throws syntax error") {
    string input = "Select s such that F o l l ows(s s, d)";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / space in between parameter name in pattern clause / throws syntax error") {
    string input = "Select s pattern a(v s, \"expr\")";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / space in between pattern-syn in pattern clause / throws syntax error") {
    string input = "Select s pattern a s(v, _)";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE(
    "parse / valid space in between expr spec in pattern clause / expr_spec in parsed query doesn't contain space") {
    string input = "Select s pattern a(v, \"a + v + z \")";
    string expectedExprSpec = "a+v+z";
    SelectQueryParser sqp;
    Query q = sqp.parse(input);
    CHECK(q.patterns[0].exprSpecs[0] == expectedExprSpec);
}

TEST_CASE("parse / select tuple parameter with valid attributes and valid spaces / return query") {
    string input = "Select <s . stmt# >";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM, AttributeType::STMTNO)};
    vector<shared_ptr<Relationship>> relationships{};
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = true;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / select tuple multiple parameter with valid attributes and valid spaces / return query") {
    string input = "Select < s . stmt# , s1 . stmt# >";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM, AttributeType::STMTNO),
                                   Parameter("s1", ParameterType::SYNONYM, AttributeType::STMTNO)};
    vector<shared_ptr<Relationship>> relationships{};
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = true;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / select tuple multiple parameter with valid attributes and invalid spaces / return query") {
    string input = "Select < s 2. stmt# , s1 . stmt# >";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / select such that with valid spaces in fixed string / return query") {
    string input = "Select s such that Uses( \" valid \", \"hello \" ) ";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    Parameter rel1Param1("valid", ParameterType::FIXED_STRING);
    Parameter rel1Param2("hello", ParameterType::FIXED_STRING);
    vector<Parameter> rel1Params{rel1Param1, rel1Param2};
    vector<shared_ptr<Relationship>> relationships{Relationship::makeRelationship(AppConstants::USES, rel1Params)};
    vector<Pattern> patterns{};
    vector<Comparison> comparisons{};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / select such that with invalid spaces in fixed string / throws syntax error") {
    string input = "Select s such that Uses( \"in valid\", \"hello\" )";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / select tuple but only one item in the tuple / throws syntax error") {
    string input = "Select <s>";
    SelectQueryParser sqp;
    Query q = sqp.parse(input);
    REQUIRE(q.selectParameters.size() == 1);
}

TEST_CASE("parse / select pattern with valid spaces in exprSpec / return query") {
    SelectQueryParser sqp;
    SECTION("exprSpec without wild cards") {
        string input1 = "Select s pattern a (d, \" valid\")";
        Query outputQuery = sqp.parse(input1);

        vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
        vector<shared_ptr<Relationship>> relationships{};
        Parameter pat1Syn("a", ParameterType::SYNONYM);
        Parameter pat1Ent("d", ParameterType::SYNONYM);
        vector<string> pat1Expr{"valid"};
        vector<Pattern> patterns{Pattern(pat1Syn, pat1Ent, pat1Expr)};
        vector<Comparison> comparisons{};
        bool isSelectTuple = false;
        Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
        REQUIRE(q == outputQuery);
    }

    SECTION("exprSpec with wild cards") {
        string input2 = "Select s pattern a (d, _\" valid \" _)";
        Query outputQuery = sqp.parse(input2);

        vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
        vector<shared_ptr<Relationship>> relationships{};
        Parameter pat1Syn("a", ParameterType::SYNONYM);
        Parameter pat1Ent("d", ParameterType::SYNONYM);
        vector<string> pat1Expr{"_valid_"};
        vector<Pattern> patterns{Pattern(pat1Syn, pat1Ent, pat1Expr)};
        vector<Comparison> comparisons{};
        bool isSelectTuple = false;
        Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
        REQUIRE(q == outputQuery);
    }
}

TEST_CASE("parse / select with clause with valid form / returns query") {
    string input = "Select s with \"a\" = b.procName";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    vector<Pattern> patterns{};
    Parameter comp1Left("a", ParameterType::FIXED_STRING);
    Parameter comp1Right("b", ParameterType::SYNONYM, AttributeType::PROCNAME);
    vector<Comparison> comparisons{Comparison(ComparisonOperator::EQUALS, comp1Left, comp1Right)};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / select with clause with invalid form / throws syntax error") {
    string input = "Select s with \"a\" = b";
    SelectQueryParser sqp;
    CHECK_THROWS_AS(sqp.parse(input), SyntaxException);
}

TEST_CASE("parse / select with clause with with / returns query") {
    string input = "Select s with \" issue \" = with.procName";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    vector<Pattern> patterns{};
    Parameter comp1Left("issue", ParameterType::FIXED_STRING);
    Parameter comp1Right("with", ParameterType::SYNONYM, AttributeType::PROCNAME);
    vector<Comparison> comparisons{Comparison(ComparisonOperator::EQUALS, comp1Left, comp1Right)};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}

TEST_CASE("parse / select with clause with and / returns query") {
    string input = "Select s with \"a\" = b.procName and 1 = c.stmt# and d.varName = e.procName";
    SelectQueryParser sqp;
    Query outputQuery = sqp.parse(input);

    vector<Parameter> selectParams{Parameter("s", ParameterType::SYNONYM)};
    vector<shared_ptr<Relationship>> relationships{};
    vector<Pattern> patterns{};
    Parameter comp1Left("a", ParameterType::FIXED_STRING);
    Parameter comp1Right("b", ParameterType::SYNONYM, AttributeType::PROCNAME);
    Parameter comp2Left("1", ParameterType::FIXED_INT);
    Parameter comp2Right("c", ParameterType::SYNONYM, AttributeType::STMTNO);
    Parameter comp3Left("d", ParameterType::SYNONYM, AttributeType::VARNAME);
    Parameter comp3Right("e", ParameterType::SYNONYM, AttributeType::PROCNAME);
    vector<Comparison> comparisons{Comparison(ComparisonOperator::EQUALS, comp1Left, comp1Right),
                                   Comparison(ComparisonOperator::EQUALS, comp2Left, comp2Right),
                                   Comparison(ComparisonOperator::EQUALS, comp3Left, comp3Right)};
    bool isSelectTuple = false;
    Query q(selectParams, relationships, patterns, comparisons, isSelectTuple);
    REQUIRE(q == outputQuery);
}