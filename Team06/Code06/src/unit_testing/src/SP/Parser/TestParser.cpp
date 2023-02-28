#include "../../spa/src/SP/Parser.h"
#include "../../spa/src/SP/Tokenizer.h"
#include "catch.hpp"
#include <filesystem>
#include <iostream>

#define CAST_TO(ASTNodeType, value) dynamic_cast<ASTNodeType *>(value)

// Helper functions
bool checkIfSameExpression(std::unique_ptr<Expression> expectedExpression,
                           std::unique_ptr<Expression> actualExpression);
bool checkIfSameCondition(
    std::unique_ptr<ConditionalExpression> expectedExpression,
    std::unique_ptr<ConditionalExpression> actualExpression);
bool checkIfSameStatement(std::unique_ptr<Statement> expectedStatement,
                          std::unique_ptr<Statement> actualStatement);
bool checkIfSameStatementList(std::unique_ptr<StatementList> expectedList,
                              std::unique_ptr<StatementList> actualList);
bool checkIfSameTree(std::unique_ptr<Program> expectedRoot,
                     std::unique_ptr<Program> actualRoot);

TEST_CASE("Valid Source Program") {
  std::stringstream strStream;
  Tokenizer testTokenizer;
  Parser testParser;
  std::deque<Token> tokenQueue;
  auto testDirectory = std::filesystem::path(UNIT_TESTING_DIR);
  for (int i = 0; i < 3; i++) {
    testDirectory = testDirectory.parent_path();
  }
  testDirectory /= "Tests06/sp/parser/";

  SECTION("Simple Valid Program") {
    std::ifstream testFile(testDirectory.string() + "valid1.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    auto rootNode = testParser.parseProgram(tokenQueue);

    // creating expected tree
    auto printNode = std::make_unique<PrintStatement>(2, "z");
    auto expressionNode = std::make_unique<Constant>(1);
    auto assignNode =
        std::make_unique<AssignStatement>(1, "x", std::move(expressionNode));

    std::vector<std::unique_ptr<Statement>> statements;
    statements.push_back(std::move(assignNode));
    statements.push_back(std::move(printNode));

    auto statementListNode =
        std::make_unique<StatementList>(std::move(statements));
    auto procedureNode =
        std::make_unique<Procedure>("A", std::move(statementListNode));
    std::vector<std::unique_ptr<Procedure>> procedures;
    procedures.push_back(std::move(procedureNode));

    auto programNode = std::make_unique<Program>(std::move(procedures));

    REQUIRE(checkIfSameTree(std::move(rootNode), std::move(programNode)));
  }

  SECTION("Valid Program With Multiple Procedures") {
    std::ifstream testFile(testDirectory.string() + "valid2.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    auto rootNode = testParser.parseProgram(tokenQueue);

    // creating actual tree
    auto mathNodeA = std::make_unique<MathExpression>(
        "+", std::make_unique<Variable>("x"), std::make_unique<Constant>(1));
    auto assignNodeA =
        std::make_unique<AssignStatement>(1, "x", std::move(mathNodeA));
    std::vector<std::unique_ptr<Statement>> statementsA;
    statementsA.push_back(std::move(assignNodeA));
    auto statementListNodeA =
        std::make_unique<StatementList>(std::move(statementsA));
    auto procedureNodeA =
        std::make_unique<Procedure>("A", std::move(statementListNodeA));

    auto mathNodeB = std::make_unique<MathExpression>(
        "*", std::make_unique<Variable>("y"), std::make_unique<Constant>(2));
    auto assignNodeB =
        std::make_unique<AssignStatement>(2, "y", std::move(mathNodeB));
    auto readNodeB = std::make_unique<ReadStatement>(3, "y");
    std::vector<std::unique_ptr<Statement>> statementsB;
    statementsB.push_back(std::move(assignNodeB));
    statementsB.push_back(std::move(readNodeB));
    auto statementListNodeB =
        std::make_unique<StatementList>(std::move(statementsB));
    auto procedureNodeB =
        std::make_unique<Procedure>("B", std::move(statementListNodeB));

    auto mathNodeC = std::make_unique<MathExpression>(
        "-", std::make_unique<Variable>("z"), std::make_unique<Constant>(3));
    auto assignNodeC =
        std::make_unique<AssignStatement>(6, "z", std::move(mathNodeC));
    auto printNodeC = std::make_unique<PrintStatement>(4, "z");
    auto callNodeC = std::make_unique<CallStatement>(5, "B");
    std::vector<std::unique_ptr<Statement>> statementsC;
    statementsC.push_back(std::move(printNodeC));
    statementsC.push_back(std::move(callNodeC));
    statementsC.push_back(std::move(assignNodeC));
    auto statementListNodeC =
        std::make_unique<StatementList>(std::move(statementsC));
    auto procedureNodeC =
        std::make_unique<Procedure>("C", std::move(statementListNodeC));

    std::vector<std::unique_ptr<Procedure>> procedures;
    procedures.push_back(std::move(procedureNodeA));
    procedures.push_back(std::move(procedureNodeB));
    procedures.push_back(std::move(procedureNodeC));
    auto programNode = std::make_unique<Program>(std::move(procedures));

    REQUIRE(checkIfSameTree(std::move(rootNode), std::move(programNode)));
  }

  SECTION("Valid Program With Conditionals") {
    std::ifstream testFile(testDirectory.string() + "valid3.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    auto rootNode = testParser.parseProgram(tokenQueue);

    // creating actual tree
    auto assignNodeX = std::make_unique<AssignStatement>(
        1, "x", std::make_unique<Constant>(6));
    auto assignNodeZ = std::make_unique<AssignStatement>(
        2, "z", std::make_unique<Variable>("x"));
    auto printNodeZ = std::make_unique<PrintStatement>(7, "z");

    std::vector<std::unique_ptr<Statement>> thenStatements;
    auto thenStmtNode = std::make_unique<AssignStatement>(
        5, "z", std::make_unique<Constant>(2));
    thenStatements.push_back(std::move(thenStmtNode));
    auto thenStmtList =
        std::make_unique<StatementList>(std::move(thenStatements));

    std::vector<std::unique_ptr<Statement>> elseStatements;
    auto elseStmtNode = std::make_unique<AssignStatement>(
        6, "z", std::make_unique<Constant>(3));
    elseStatements.push_back(std::move(elseStmtNode));
    auto elseStmtList =
        std::make_unique<StatementList>(std::move(elseStatements));

    auto lhsIfExpr = std::make_unique<MathExpression>(
        "%", std::make_unique<Variable>("z"), std::make_unique<Constant>(2));
    auto rhsIfExpr = std::make_unique<Constant>(0);
    auto ifRelExprNode = std::make_unique<RelationalExpression>(
        "==", std::move(lhsIfExpr), std::move(rhsIfExpr));
    auto ifStatementNode = std::make_unique<IfStatement>(
        4, std::move(ifRelExprNode), std::move(thenStmtList),
        std::move(elseStmtList));

    auto leftRelExpr = std::make_unique<RelationalExpression>(
        "<", std::make_unique<Variable>("x"), std::make_unique<Constant>(7));
    auto leftCondExpr =
        std::make_unique<NotConditionalExpression>(std::move(leftRelExpr));
    auto rightCondExpr = std::make_unique<RelationalExpression>(
        "<=", std::make_unique<Variable>("z"), std::make_unique<Constant>(12));
    auto binaryCondNode = std::make_unique<BinaryConditionalExpression>(
        "&&", std::move(leftCondExpr), std::move(rightCondExpr));

    std::vector<std::unique_ptr<Statement>> whileStatements;
    whileStatements.push_back(std::move(ifStatementNode));
    auto whileStmtList =
        std::make_unique<StatementList>(std::move(whileStatements));
    auto whileNode = std::make_unique<WhileStatement>(
        3, std::move(binaryCondNode), std::move(whileStmtList));

    std::vector<std::unique_ptr<Statement>> statementsA;
    statementsA.push_back(std::move(assignNodeX));
    statementsA.push_back(std::move(assignNodeZ));
    statementsA.push_back(std::move(whileNode));
    statementsA.push_back(std::move(printNodeZ));

    auto statementListNodeA =
        std::make_unique<StatementList>(std::move(statementsA));
    auto procedureNodeA =
        std::make_unique<Procedure>("A", std::move(statementListNodeA));
    std::vector<std::unique_ptr<Procedure>> procedures;
    procedures.push_back(std::move(procedureNodeA));
    auto programNode = std::make_unique<Program>(std::move(procedures));

    REQUIRE(checkIfSameTree(std::move(rootNode), std::move(programNode)));
  }

  SECTION("Valid Program using keywords as names") {
    std::ifstream testFile(testDirectory.string() + "valid4.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    auto rootNode = testParser.parseProgram(tokenQueue);

    // creating expected tree
    auto printNode = std::make_unique<PrintStatement>(1, "procedure");
    std::vector<std::unique_ptr<Statement>> statements;
    statements.push_back(std::move(printNode));

    auto statementListNode =
        std::make_unique<StatementList>(std::move(statements));
    auto procedureNode = std::make_unique<Procedure>(
        AppConstants::PROCEDURE, std::move(statementListNode));

    std::vector<std::unique_ptr<Procedure>> procedures;
    procedures.push_back(std::move(procedureNode));

    auto programNode = std::make_unique<Program>(std::move(procedures));

    REQUIRE(checkIfSameTree(std::move(rootNode), std::move(programNode)));
  }

  SECTION("Valid Program to test distinguish between relExpr & condExpr") {
    std::ifstream testFile(testDirectory.string() + "valid5.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    auto rootNode = testParser.parseProgram(tokenQueue);

    // creating expected tree
    auto assignNodeX = std::make_unique<AssignStatement>(
        2, "x", std::make_unique<Constant>(1));
    std::vector<std::unique_ptr<Statement>> whileStatements;
    whileStatements.push_back(std::move(assignNodeX));

    auto whileStmtList =
        std::make_unique<StatementList>(std::move(whileStatements));
    auto lhsExpr = std::make_unique<MathExpression>(
        "+", std::make_unique<Constant>(1), std::make_unique<Constant>(3));
    auto relExpr = std::make_unique<RelationalExpression>(
        "<", std::move(lhsExpr), std::make_unique<Constant>(2));
    auto whileNode = std::make_unique<WhileStatement>(1, std::move(relExpr),
                                                      std::move(whileStmtList));

    std::vector<std::unique_ptr<Statement>> statementsA;
    statementsA.push_back(std::move(whileNode));

    auto statementListNodeA =
        std::make_unique<StatementList>(std::move(statementsA));
    auto procedureNodeA =
        std::make_unique<Procedure>("A", std::move(statementListNodeA));
    std::vector<std::unique_ptr<Procedure>> procedures;
    procedures.push_back(std::move(procedureNodeA));

    auto programNode = std::make_unique<Program>(std::move(procedures));

    REQUIRE(checkIfSameTree(std::move(rootNode), std::move(programNode)));
  }

  SECTION("Valid Program with procedure/variable names as keywords") {
    std::ifstream testFile(testDirectory.string() + "valid6.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    auto rootNode = testParser.parseProgram(tokenQueue);

    // creating expected tree
    auto readStmt = std::make_unique<ReadStatement>(1, AppConstants::CALL);
    auto callStmt = std::make_unique<CallStatement>(2, AppConstants::PRINT);
    auto printStmt = std::make_unique<PrintStatement>(3, AppConstants::READ);
    auto readAssignStmt = std::make_unique<AssignStatement>(
        4, AppConstants::READ, std::make_unique<Constant>(2));
    auto callAssignStmt = std::make_unique<AssignStatement>(
        5, AppConstants::CALL, std::make_unique<Constant>(3));
    auto whileAssignStmt = std::make_unique<AssignStatement>(
        6, AppConstants::WHILE, std::make_unique<Constant>(5));
    auto ifAssignStmt = std::make_unique<AssignStatement>(
        7, AppConstants::IF, std::make_unique<Constant>(4));
    auto printAssignStmt = std::make_unique<AssignStatement>(
        8, AppConstants::PRINT, std::make_unique<Constant>(8));

    std::vector<std::unique_ptr<Statement>> statementsA;
    statementsA.push_back(std::move(readStmt));
    statementsA.push_back(std::move(callStmt));
    statementsA.push_back(std::move(printStmt));
    statementsA.push_back(std::move(readAssignStmt));
    statementsA.push_back(std::move(callAssignStmt));
    statementsA.push_back(std::move(whileAssignStmt));
    statementsA.push_back(std::move(ifAssignStmt));
    statementsA.push_back(std::move(printAssignStmt));

    auto statementListNodeA =
        std::make_unique<StatementList>(std::move(statementsA));
    auto procedureNodeA = std::make_unique<Procedure>(
        AppConstants::WHILE, std::move(statementListNodeA));
    std::vector<std::unique_ptr<Procedure>> procedures;
    procedures.push_back(std::move(procedureNodeA));

    auto programNode = std::make_unique<Program>(std::move(procedures));

    REQUIRE(checkIfSameTree(std::move(rootNode), std::move(programNode)));
  }
}

TEST_CASE("Invalid Source Program") {
  std::stringstream strStream;
  Tokenizer testTokenizer;
  Parser testParser;
  std::deque<Token> tokenQueue;
  std::string errorMessage = "";
  auto testDirectory = std::filesystem::path(UNIT_TESTING_DIR);
  for (int i = 0; i < 3; i++) {
    testDirectory = testDirectory.parent_path();
  }
  testDirectory /= "Tests06/sp/parser/";

  SECTION("Program has no procedure") {
    std::ifstream testFile(testDirectory.string() + "invalid1.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(
        errorMessage.find("Program should contain at least one procedure") !=
        std::string::npos);
  }

  SECTION("No procedure keyword") {
    std::ifstream testFile(testDirectory.string() + "invalid2.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected 'procedure' keyword") !=
            std::string::npos);
  }

  SECTION("Invalid procedure name") {
    std::ifstream testFile(testDirectory.string() + "invalid3.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected valid 'proc_name'") !=
            std::string::npos);
  }

  SECTION("Missing left brace in procedure") {
    std::ifstream testFile(testDirectory.string() + "invalid4.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected '{'") != std::string::npos);
  }

  SECTION("Missing right brace in procedure") {
    std::ifstream testFile(testDirectory.string() + "invalid5.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected '}'") != std::string::npos);
  }

  SECTION("Statement list no statements") {
    std::ifstream testFile(testDirectory.string() + "invalid6.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find(
                "Statement List should contain at least one statement") !=
            std::string::npos);
  }

  SECTION("Invalid statement type") {
    std::ifstream testFile(testDirectory.string() + "invalid7.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Unexpected token when parsing statement") !=
            std::string::npos);
  }

  SECTION("Invalid statement keyword") {
    std::ifstream testFile(testDirectory.string() + "invalid8.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Unexpected token when parsing statement") !=
            std::string::npos);
  }

  SECTION("Invalid read statement variable name") {
    std::ifstream testFile(testDirectory.string() + "invalid9.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected var_name in read statement") !=
            std::string::npos);
  }

  SECTION("Missing semicolon for read statement") {
    std::ifstream testFile(testDirectory.string() + "invalid10.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected ; at end of read statement") !=
            std::string::npos);
  }

  SECTION("Invalid print statement variable name") {
    std::ifstream testFile(testDirectory.string() + "invalid11.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected var_name in print statement") !=
            std::string::npos);
  }

  SECTION("Missing semicolon for read statement") {
    std::ifstream testFile(testDirectory.string() + "invalid12.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected ; at end of print statement") !=
            std::string::npos);
  }

  SECTION("Invalid call statement procedure name") {
    std::ifstream testFile(testDirectory.string() + "invalid13.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected proc_name in call statement") !=
            std::string::npos);
  }

  SECTION("Missing semicolon for call statement") {
    std::ifstream testFile(testDirectory.string() + "invalid14.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected ; at end of call statement") !=
            std::string::npos);
  }

  SECTION("Missing ( in while") {
    std::ifstream testFile(testDirectory.string() + "invalid15.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected '(' in while statement") !=
            std::string::npos);
  }

  SECTION("Missing ) in while") {
    std::ifstream testFile(testDirectory.string() + "invalid16.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected ')' in while statement") !=
            std::string::npos);
  }

  SECTION("Missing { in while") {
    std::ifstream testFile(testDirectory.string() + "invalid17.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected '{' in while statement") !=
            std::string::npos);
  }

  SECTION("Missing } in while") {
    std::ifstream testFile(testDirectory.string() + "invalid18.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected '}'") != std::string::npos);
  }

  SECTION("Missing ( in if") {
    std::ifstream testFile(testDirectory.string() + "invalid19.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected '(' in if statement") !=
            std::string::npos);
  }

  SECTION("Missing ) in if") {
    std::ifstream testFile(testDirectory.string() + "invalid20.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected ')' in if statement") !=
            std::string::npos);
  }

  SECTION("Missing then in if") {
    std::ifstream testFile(testDirectory.string() + "invalid21.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected 'then' in if statement") !=
            std::string::npos);
  }

  SECTION("Missing { in if") {
    std::ifstream testFile(testDirectory.string() + "invalid22.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected '{' in if statement") !=
            std::string::npos);
  }

  SECTION("Missing } in if") {
    std::ifstream testFile(testDirectory.string() + "invalid23.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Unexpected token") != std::string::npos);
  }

  SECTION("Missing else in if") {
    std::ifstream testFile(testDirectory.string() + "invalid24.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected 'else' in if statement") !=
            std::string::npos);
  }

  SECTION("Missing { in if after else") {
    std::ifstream testFile(testDirectory.string() + "invalid25.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected '{' in if statement") !=
            std::string::npos);
  }

  SECTION("Missing } in if after else") {
    std::ifstream testFile(testDirectory.string() + "invalid26.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected '}'") != std::string::npos);
  }

  SECTION("Invalid = in assign statement") {
    std::ifstream testFile(testDirectory.string() + "invalid27.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Unexpected token") != std::string::npos);
  }

  SECTION("Missing semicolon for assign statement") {
    std::ifstream testFile(testDirectory.string() + "invalid28.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected ';' at end of assign statement") !=
            std::string::npos);
  }

  SECTION("Missing semicolon for assign statement") {
    std::ifstream testFile(testDirectory.string() + "invalid28.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected ';' at end of assign statement") !=
            std::string::npos);
  }

  SECTION("Missing ( for not conditional expression") {
    std::ifstream testFile(testDirectory.string() + "invalid29.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected '(' in not conditional expression") !=
            std::string::npos);
  }

  SECTION("Missing ( for not conditional expression") {
    std::ifstream testFile(testDirectory.string() + "invalid30.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected ')'") != std::string::npos);
  }

  SECTION("Missing ) in binary conditional") {
    std::ifstream testFile(testDirectory.string() + "invalid31.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(
        errorMessage.find("Expected ')' in binary conditional expression") !=
        std::string::npos);
  }

  SECTION("Missing && or || in binary conditional") {
    std::ifstream testFile(testDirectory.string() + "invalid32.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find(
                "Expected '&&' or '||' in binary conditional expression") !=
            std::string::npos);
  }

  SECTION("Missing '(' in binary conditional") {
    std::ifstream testFile(testDirectory.string() + "invalid33.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(
        errorMessage.find("Expected '(' in binary conditional expression") !=
        std::string::npos);
  }

  SECTION("Missing ')' in binary conditional") {
    std::ifstream testFile(testDirectory.string() + "invalid34.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected ')'") != std::string::npos);
  }

  SECTION("Invalid comparator in relational expression") {
    std::ifstream testFile(testDirectory.string() + "invalid35.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(
        errorMessage.find("Expected a comparator in relational expression") !=
        std::string::npos);
  }

  SECTION("Invalid expression") {
    std::ifstream testFile(testDirectory.string() + "invalid36.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Parsing factor failed") != std::string::npos);
  }

  SECTION("Invalid term") {
    std::ifstream testFile(testDirectory.string() + "invalid37.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Parsing factor failed") != std::string::npos);
  }

  SECTION("Invalid expression missing ')'") {
    std::ifstream testFile(testDirectory.string() + "invalid38.txt");
    strStream << testFile.rdbuf();
    tokenQueue = testTokenizer.tokenize(strStream);

    try {
      testParser.parseProgram(tokenQueue);
    } catch (SyntaxErrorException e) {
      errorMessage = e.what();
    }

    REQUIRE(errorMessage.find("Expected ')' in factor") != std::string::npos);
  }
}

bool checkIfSameExpression(std::unique_ptr<Expression> expectedExpression,
                           std::unique_ptr<Expression> actualExpression) {
  auto expected = expectedExpression.get();
  auto actual = actualExpression.get();

  if (CAST_TO(Constant, expected) && CAST_TO(Constant, actual)) {
    return CAST_TO(Constant, expected)->value ==
           CAST_TO(Constant, actual)->value;
  } else if (CAST_TO(Variable, expected) && CAST_TO(Variable, actual)) {
    return CAST_TO(Variable, expected)->name == CAST_TO(Variable, actual)->name;
  } else if (CAST_TO(MathExpression, expected) &&
             CAST_TO(MathExpression, actual)) {
    auto expectedExpression = CAST_TO(MathExpression, expected);
    auto actualExpression = CAST_TO(MathExpression, actual);
    bool isSameLeftExpression = checkIfSameExpression(
        std::move(expectedExpression->lhs), std::move(actualExpression->lhs));
    bool isSameRightExpression = checkIfSameExpression(
        std::move(expectedExpression->rhs), std::move(actualExpression->rhs));
    bool isSameOp =
        (expectedExpression->mathOperator == actualExpression->mathOperator);
    return (isSameLeftExpression && isSameRightExpression && isSameOp);
  } else {
    return false; // Invalid expression type
  }
}

bool checkIfSameCondition(
    std::unique_ptr<ConditionalExpression> expectedExpression,
    std::unique_ptr<ConditionalExpression> actualExpression) {
  auto expected = expectedExpression.get();
  auto actual = actualExpression.get();

  if (CAST_TO(NotConditionalExpression, expected) &&
      CAST_TO(NotConditionalExpression, actual)) {
    return checkIfSameCondition(
        std::move(CAST_TO(NotConditionalExpression, expected)->condExpr),
        std::move(CAST_TO(NotConditionalExpression, actual)->condExpr));
  } else if (CAST_TO(BinaryConditionalExpression, expected) &&
             CAST_TO(BinaryConditionalExpression, actual)) {
    auto expectedCondition = CAST_TO(BinaryConditionalExpression, expected);
    auto actualCondition = CAST_TO(BinaryConditionalExpression, actual);
    bool isSameLeftConditional = checkIfSameCondition(
        std::move(expectedCondition->lhs), std::move(actualCondition->lhs));
    bool isSameRightConditional = checkIfSameCondition(
        std::move(expectedCondition->rhs), std::move(actualCondition->rhs));
    bool isSameOp =
        (expectedCondition->condOperator == actualCondition->condOperator);
    return (isSameLeftConditional && isSameRightConditional && isSameOp);
  } else if (CAST_TO(RelationalExpression, expected) &&
             CAST_TO(RelationalExpression, actual)) {
    auto expectedCondition = CAST_TO(RelationalExpression, expected);
    auto actualCondition = CAST_TO(RelationalExpression, actual);
    bool isSameLeftExpression = checkIfSameExpression(
        std::move(expectedCondition->lhs), std::move(actualCondition->lhs));
    bool isSameRightExpression = checkIfSameExpression(
        std::move(expectedCondition->rhs), std::move(actualCondition->rhs));
    bool isSameOp = (expectedCondition->relationalOperator ==
                     actualCondition->relationalOperator);
    return (isSameLeftExpression && isSameRightExpression && isSameOp);
  } else {
    return false;
  }
}

bool checkIfSameStatement(std::unique_ptr<Statement> expectedStatement,
                          std::unique_ptr<Statement> actualStatement) {
  auto expected = expectedStatement.get();
  auto actual = actualStatement.get();

  if (CAST_TO(PrintStatement, expected) && CAST_TO(PrintStatement, actual)) {
    return CAST_TO(PrintStatement, expected)->varName ==
               CAST_TO(PrintStatement, actual)->varName &&
           CAST_TO(PrintStatement, expected)->statementNumber ==
               CAST_TO(PrintStatement, actual)->statementNumber;
  } else if (CAST_TO(ReadStatement, expected) &&
             CAST_TO(ReadStatement, actual)) {
    return CAST_TO(ReadStatement, expected)->varName ==
               CAST_TO(ReadStatement, actual)->varName &&
           CAST_TO(ReadStatement, expected)->statementNumber ==
               CAST_TO(ReadStatement, actual)->statementNumber;
  } else if (CAST_TO(CallStatement, expected) &&
             CAST_TO(CallStatement, actual)) {
    return CAST_TO(CallStatement, expected)->procName ==
               CAST_TO(CallStatement, actual)->procName &&
           CAST_TO(CallStatement, expected)->statementNumber ==
               CAST_TO(CallStatement, actual)->statementNumber;
  } else if (CAST_TO(AssignStatement, expected) &&
             CAST_TO(AssignStatement, actual)) {
    auto expectedAssignStatement = CAST_TO(AssignStatement, expected);
    auto actualAssignStatement = CAST_TO(AssignStatement, actual);
    bool isSameStatementNumber = (expectedAssignStatement->statementNumber ==
                                  actualAssignStatement->statementNumber);
    bool isSameVariable =
        (expectedAssignStatement->varName == actualAssignStatement->varName);
    bool isSameExpression =
        checkIfSameExpression(std::move(expectedAssignStatement->expr),
                              std::move(actualAssignStatement->expr));
    return isSameVariable && isSameExpression && isSameStatementNumber;
  } else if (CAST_TO(IfStatement, expected) && CAST_TO(IfStatement, actual)) {
    auto expectedIfObject = CAST_TO(IfStatement, expected);
    auto actualIfObject = CAST_TO(IfStatement, actual);
    bool isSameStatementNumber =
        (expectedIfObject->statementNumber == actualIfObject->statementNumber);
    bool isSameCondition =
        checkIfSameCondition(std::move(expectedIfObject->condExpr),
                             std::move(actualIfObject->condExpr));
    bool isSameThenStatements =
        checkIfSameStatementList(std::move(expectedIfObject->thenStmtList),
                                 std::move(actualIfObject->thenStmtList));
    bool isSameElseStatements =
        checkIfSameStatementList(std::move(expectedIfObject->elseStmtList),
                                 std::move(actualIfObject->elseStmtList));
    return isSameCondition && isSameThenStatements && isSameElseStatements &&
           isSameStatementNumber;
  } else if (CAST_TO(WhileStatement, expected) &&
             CAST_TO(WhileStatement, actual)) {
    auto expectedWhileObject = CAST_TO(WhileStatement, expected);
    auto actualWhileObject = CAST_TO(WhileStatement, actual);
    bool isSameStatementNumber = (expectedWhileObject->statementNumber ==
                                  actualWhileObject->statementNumber);
    bool isSameCondition =
        checkIfSameCondition(std::move(expectedWhileObject->condExpr),
                             std::move(actualWhileObject->condExpr));
    bool isSameStatements =
        checkIfSameStatementList(std::move(expectedWhileObject->stmtList),
                                 std::move(actualWhileObject->stmtList));
    return isSameCondition && isSameStatements && isSameStatementNumber;
  } else {
    return false;
  }
}

bool checkIfSameStatementList(std::unique_ptr<StatementList> expectedList,
                              std::unique_ptr<StatementList> actualList) {
  // preliminary check for statement list
  if (expectedList->statements.size() != actualList->statements.size()) {
    return false;
  }

  for (int i = 0; i < expectedList->statements.size(); i++) {
    if (!checkIfSameStatement(std::move(expectedList->statements[i]),
                              std::move(actualList->statements[i]))) {
      return false;
    }
  }
  return true;
}

bool checkIfSameTree(std::unique_ptr<Program> expectedRoot,
                     std::unique_ptr<Program> actualRoot) {
  auto actualProcedures = std::move(actualRoot->procedureList);
  auto expectedProcedures = std::move(expectedRoot->procedureList);

  // preliminary check for procedurelist
  if (expectedProcedures.size() != actualProcedures.size()) {
    return false;
  }

  // loop thru procedures
  for (int i = 0; i < expectedProcedures.size(); i++) {
    auto expectedStatements = std::move(expectedProcedures[i]->statementList);
    auto actualStatements = std::move(actualProcedures[i]->statementList);
    if (expectedProcedures[i]->procedureName !=
        actualProcedures[i]->procedureName) {
      return false;
    }
    if (!checkIfSameStatementList(std::move(expectedStatements),
                                  std::move(actualStatements))) {
      return false;
    }
  }
  return true;
}