#include <iostream>
#include "catch.hpp"
#include "../../spa/src/SP/Tokenizer.h"
#include "../../spa/src/SP/Parser.h"

#define CAST_TO(ASTNodeType, value) dynamic_cast<ASTNodeType*>(value)

// Helper functions
bool checkIfSameExpression(std::unique_ptr<Expression> expectedExpression, std::unique_ptr<Expression> actualExpression);
bool checkIfSameCondition(std::unique_ptr<ConditionalExpression> expectedExpression, std::unique_ptr<ConditionalExpression> actualExpression);
bool checkIfSameStatement(std::unique_ptr<Statement> expectedStatement, std::unique_ptr<Statement> actualStatement);
bool checkIfSameStatementList(std::unique_ptr<StatementList> expectedList, std::unique_ptr<StatementList> actualList);
bool checkIfSameTree(std::unique_ptr<Program> expectedRoot, std::unique_ptr<Program> actualRoot);

TEST_CASE("Valid Source Program") {
	std::stringstream strStream;
	Tokenizer testTokenizer;
	Parser testParser;
	std::deque<Token> tokenQueue;
	std::string testDirectory = "../../../../../../Tests06/sp/parser/";

    //MacOS CLion requires full pathname for some reason
    //std::string testDirectory = "/Users/admin/Downloads/this sem/cs3203project/Team06/Tests06/sp/parser/";

	SECTION("Simple Valid Program") {
		std::ifstream testFile(testDirectory + "valid1.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		auto rootNode = testParser.parseProgram(tokenQueue);
		
		// creating expected tree
		auto printNode = std::make_unique<PrintStatement>("z");
		auto expressionNode = std::make_unique<Constant>(1);
		auto assignNode = std::make_unique<AssignStatement>("x", std::move(expressionNode));

		std::vector<std::unique_ptr<Statement>> statements;
		statements.push_back(std::move(assignNode));
		statements.push_back(std::move(printNode));

		auto statementListNode = std::make_unique<StatementList>(std::move(statements));
		auto procedureNode = std::make_unique<Procedure>("A", std::move(statementListNode));
		std::vector<std::unique_ptr<Procedure>> procedures;
		procedures.push_back(std::move(procedureNode));

		auto programNode = std::make_unique<Program>(std::move(procedures));

		REQUIRE(checkIfSameTree(std::move(rootNode), std::move(programNode)));
	}


	SECTION("Valid Program With Multiple Procedures") {
		std::ifstream testFile(testDirectory + "valid2.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		auto rootNode = testParser.parseProgram(tokenQueue);

		// creating actual tree
		auto mathNodeA = std::make_unique<MathExpression>("+", std::make_unique<Variable>("x"), std::make_unique<Constant>(1));
		auto assignNodeA = std::make_unique<AssignStatement>("x", std::move(mathNodeA));
		std::vector<std::unique_ptr<Statement>> statementsA;
		statementsA.push_back(std::move(assignNodeA));
		auto statementListNodeA = std::make_unique<StatementList>(std::move(statementsA));
		auto procedureNodeA = std::make_unique<Procedure>("A", std::move(statementListNodeA));

		auto mathNodeB = std::make_unique<MathExpression>("*", std::make_unique<Variable>("y"), std::make_unique<Constant>(2));
		auto assignNodeB = std::make_unique<AssignStatement>("y", std::move(mathNodeB));
		auto readNodeB = std::make_unique<ReadStatement>("y");
		std::vector<std::unique_ptr<Statement>> statementsB;    
		statementsB.push_back(std::move(assignNodeB));
		statementsB.push_back(std::move(readNodeB));
		auto statementListNodeB = std::make_unique<StatementList>(std::move(statementsB));
		auto procedureNodeB = std::make_unique<Procedure>("B", std::move(statementListNodeB));

		auto mathNodeC = std::make_unique<MathExpression>("-", std::make_unique<Variable>("z"), std::make_unique<Constant>(3));
		auto assignNodeC = std::make_unique<AssignStatement>("z", std::move(mathNodeC));
		auto printNodeC = std::make_unique<PrintStatement>("z");
		auto callNodeC = std::make_unique<CallStatement>("B");
		std::vector<std::unique_ptr<Statement>> statementsC;
		statementsC.push_back(std::move(printNodeC));
		statementsC.push_back(std::move(callNodeC));
		statementsC.push_back(std::move(assignNodeC));
		auto statementListNodeC = std::make_unique<StatementList>(std::move(statementsC));
		auto procedureNodeC = std::make_unique<Procedure>("C", std::move(statementListNodeC));

		std::vector<std::unique_ptr<Procedure>> procedures;
		procedures.push_back(std::move(procedureNodeA));
		procedures.push_back(std::move(procedureNodeB));
		procedures.push_back(std::move(procedureNodeC));
		auto programNode = std::make_unique<Program>(std::move(procedures));

		REQUIRE(checkIfSameTree(std::move(rootNode), std::move(programNode)));
	}

	SECTION("Valid Program With Conditionals") {
		std::ifstream testFile(testDirectory + "valid3.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		auto rootNode = testParser.parseProgram(tokenQueue);

		// creating actual tree
		auto assignNodeX = std::make_unique<AssignStatement>("x", std::make_unique<Constant>(6));
		auto assignNodeZ = std::make_unique<AssignStatement>("z", std::make_unique<Variable>("x"));
		auto printNodeZ = std::make_unique<PrintStatement>("z");

		std::vector<std::unique_ptr<Statement>> thenStatements;
		auto thenStmtNode = std::make_unique<AssignStatement>("z", std::make_unique<Constant>(2));
		thenStatements.push_back(std::move(thenStmtNode));
		auto thenStmtList = std::make_unique<StatementList>(std::move(thenStatements));

		std::vector<std::unique_ptr<Statement>> elseStatements;
		auto elseStmtNode = std::make_unique<AssignStatement>("z", std::make_unique<Constant>(3));
		elseStatements.push_back(std::move(elseStmtNode));
		auto elseStmtList = std::make_unique<StatementList>(std::move(elseStatements));

		auto lhsIfExpr = std::make_unique<MathExpression>("%", std::make_unique<Variable>("z"), std::make_unique<Constant>(2));
		auto rhsIfExpr = std::make_unique<Constant>(0);
		auto ifRelExprNode = std::make_unique<RelationalExpression>("==", std::move(lhsIfExpr), std::move(rhsIfExpr));
		auto ifStatementNode = std::make_unique<IfStatement>(std::move(ifRelExprNode), std::move(thenStmtList), std::move(elseStmtList));
		
		auto leftRelExpr = std::make_unique<RelationalExpression>("<", std::make_unique<Variable>("x"), std::make_unique<Constant>(7));
		auto leftCondExpr = std::make_unique<NotConditionalExpression>(std::move(leftRelExpr));
		auto rightCondExpr = std::make_unique<RelationalExpression>("<=", std::make_unique<Variable>("z"), std::make_unique<Constant>(12));
		auto binaryCondNode = std::make_unique<BinaryConditionalExpression>("&&", std::move(leftCondExpr), std::move(rightCondExpr));
		
		std::vector<std::unique_ptr<Statement>> whileStatements;
		whileStatements.push_back(std::move(ifStatementNode));
		auto whileStmtList = std::make_unique<StatementList>(std::move(whileStatements));
		auto whileNode = std::make_unique<WhileStatement>(std::move(binaryCondNode), std::move(whileStmtList));

		std::vector<std::unique_ptr<Statement>> statementsA;
		statementsA.push_back(std::move(assignNodeX));
		statementsA.push_back(std::move(assignNodeZ));
		statementsA.push_back(std::move(whileNode));
		statementsA.push_back(std::move(printNodeZ));

		auto statementListNodeA = std::make_unique<StatementList>(std::move(statementsA));
		auto procedureNodeA = std::make_unique<Procedure>("A", std::move(statementListNodeA));	
		std::vector<std::unique_ptr<Procedure>> procedures;
		procedures.push_back(std::move(procedureNodeA));
		auto programNode = std::make_unique<Program>(std::move(procedures));

		REQUIRE(checkIfSameTree(std::move(rootNode), std::move(programNode)));
	}

	SECTION("Valid Program using keywords as names") {
		std::ifstream testFile(testDirectory + "valid4.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		auto rootNode = testParser.parseProgram(tokenQueue);

		// creating expected tree
		auto printNode = std::make_unique<PrintStatement>("procedure");
		std::vector<std::unique_ptr<Statement>> statements;
		statements.push_back(std::move(printNode));

		auto statementListNode = std::make_unique<StatementList>(std::move(statements));
		auto procedureNode = std::make_unique<Procedure>("procedure", std::move(statementListNode));

		std::vector<std::unique_ptr<Procedure>> procedures;
		procedures.push_back(std::move(procedureNode));

		auto programNode = std::make_unique<Program>(std::move(procedures));

		REQUIRE(checkIfSameTree(std::move(rootNode), std::move(programNode)));
	}

	SECTION("Valid Program to test distinguish between relExpr & condExpr") {
		std::ifstream testFile(testDirectory + "valid5.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		auto rootNode = testParser.parseProgram(tokenQueue);

		// creating expected tree
		auto assignNodeX = std::make_unique<AssignStatement>("x", std::make_unique<Constant>(1));
		std::vector<std::unique_ptr<Statement>> whileStatements;
		whileStatements.push_back(std::move(assignNodeX));

		auto whileStmtList = std::make_unique<StatementList>(std::move(whileStatements));
		auto lhsExpr = std::make_unique<MathExpression>("+", std::make_unique<Constant>(1), std::make_unique<Constant>(3));
		auto relExpr = std::make_unique<RelationalExpression>("<", std::move(lhsExpr), std::make_unique<Constant>(2));
		auto whileNode = std::make_unique<WhileStatement>(std::move(relExpr), std::move(whileStmtList));

		std::vector<std::unique_ptr<Statement>> statementsA;
		statementsA.push_back(std::move(whileNode));

		auto statementListNodeA = std::make_unique<StatementList>(std::move(statementsA));
		auto procedureNodeA = std::make_unique<Procedure>("A", std::move(statementListNodeA));
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
	std::string testDirectory = "../../../../../../Tests06/sp/parser/";

	SECTION("Program has no procedure") {
		std::ifstream testFile(testDirectory + "invalid1.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		//REQUIRE_THROWS_MATCHES(testParser.parseProgram(tokenQueue), SyntaxErrorException, Catch::Message("SyntaxErrorException::what"));
		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Procedure has invalid syntax") {
		std::ifstream testFile(testDirectory + "invalid2.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Statement List has no statement") {
		std::ifstream testFile(testDirectory + "invalid3.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid read statement") {
		std::ifstream testFile(testDirectory + "invalid4.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid print statement") {
		std::ifstream testFile(testDirectory + "invalid5.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid call statement") {
		std::ifstream testFile(testDirectory + "invalid6.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid while statement") {
		std::ifstream testFile(testDirectory + "invalid7.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid if statement") {
		std::ifstream testFile(testDirectory + "invalid8.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid assign statement") {
		std::ifstream testFile(testDirectory + "invalid9.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid conditional") {
		std::ifstream testFile(testDirectory + "invalid10.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid statement type") {
		std::ifstream testFile(testDirectory + "invalid11.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid relational expression") {
		std::ifstream testFile(testDirectory + "invalid12.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid expression") {
		std::ifstream testFile(testDirectory + "invalid13.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid term") {
		std::ifstream testFile(testDirectory + "invalid14.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

	SECTION("Invalid factor") {
		std::ifstream testFile(testDirectory + "invalid15.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}
}

bool checkIfSameExpression(std::unique_ptr<Expression> expectedExpression, std::unique_ptr<Expression> actualExpression) {
	auto expected = expectedExpression.get();
	auto actual = actualExpression.get();

	if (CAST_TO(Constant, expected) && CAST_TO(Constant, actual)) {
		return CAST_TO(Constant, expected)->value == CAST_TO(Constant, actual)->value;
	}
	else if (CAST_TO(Variable, expected) && CAST_TO(Variable, actual)) {
		return CAST_TO(Variable, expected)->name == CAST_TO(Variable, actual)->name;
	}
	else if (CAST_TO(MathExpression, expected) && CAST_TO(MathExpression, actual)) {
		auto expectedExpression = CAST_TO(MathExpression, expected);
		auto actualExpression = CAST_TO(MathExpression, actual);
		bool isSameLeftExpression = checkIfSameExpression(std::move(expectedExpression->lhs), std::move(actualExpression->lhs));
		bool isSameRightExpression = checkIfSameExpression(std::move(expectedExpression->rhs), std::move(actualExpression->rhs));
		bool isSameOp = (expectedExpression->mathOperator == actualExpression->mathOperator);
		return (isSameLeftExpression && isSameRightExpression && isSameOp);
	}
	else {
		return false; // Invalid expression type
	}
}

bool checkIfSameCondition(std::unique_ptr<ConditionalExpression> expectedExpression, std::unique_ptr<ConditionalExpression> actualExpression) {
	auto expected = expectedExpression.get();
	auto actual = actualExpression.get();

	if (CAST_TO(NotConditionalExpression, expected) && CAST_TO(NotConditionalExpression, actual)) {
		return checkIfSameCondition(std::move(CAST_TO(NotConditionalExpression, expected)->condExpr),
			std::move(CAST_TO(NotConditionalExpression, actual)->condExpr));
	}
	else if (CAST_TO(BinaryConditionalExpression, expected) && CAST_TO(BinaryConditionalExpression, actual)) {
		auto expectedCondition = CAST_TO(BinaryConditionalExpression, expected);
		auto actualCondition = CAST_TO(BinaryConditionalExpression, actual);
		bool isSameLeftConditional = checkIfSameCondition(std::move(expectedCondition->lhs), std::move(actualCondition->lhs));
		bool isSameRightConditional = checkIfSameCondition(std::move(expectedCondition->rhs), std::move(actualCondition->rhs));
		bool isSameOp = (expectedCondition->condOperator == actualCondition->condOperator);
		return (isSameLeftConditional && isSameRightConditional && isSameOp);
	}
	else if (CAST_TO(RelationalExpression, expected) && CAST_TO(RelationalExpression, actual)) {
		auto expectedCondition = CAST_TO(RelationalExpression, expected);
		auto actualCondition = CAST_TO(RelationalExpression, actual);
		bool isSameLeftExpression = checkIfSameExpression(std::move(expectedCondition->lhs), std::move(actualCondition->lhs));
		bool isSameRightExpression = checkIfSameExpression(std::move(expectedCondition->rhs), std::move(actualCondition->rhs));
		bool isSameOp = (expectedCondition->relationalOperator == actualCondition->relationalOperator);
		return (isSameLeftExpression && isSameRightExpression && isSameOp);
	}
	else {
		return false;
	}
}

bool checkIfSameStatement(std::unique_ptr<Statement> expectedStatement, std::unique_ptr<Statement> actualStatement) {
	auto expected = expectedStatement.get();
	auto actual = actualStatement.get();

	if (CAST_TO(PrintStatement, expected) && CAST_TO(PrintStatement, actual)) {
		return CAST_TO(PrintStatement, expected)->varName == CAST_TO(PrintStatement, actual)->varName;
	}
	else if (CAST_TO(ReadStatement, expected) && CAST_TO(ReadStatement, actual)) {
		return CAST_TO(ReadStatement, expected)->varName == CAST_TO(ReadStatement, actual)->varName;
	}
	else if (CAST_TO(CallStatement, expected) && CAST_TO(CallStatement, actual)) {
		return CAST_TO(CallStatement, expected)->procName == CAST_TO(CallStatement, actual)->procName;
	}
	else if (CAST_TO(AssignStatement, expected) && CAST_TO(AssignStatement, actual)) {
		auto expectedAssignStatement = CAST_TO(AssignStatement, expected);
		auto actualAssignStatement = CAST_TO(AssignStatement, actual);
		bool isSameVariable = (expectedAssignStatement->varName == actualAssignStatement->varName);
		bool isSameExpression = checkIfSameExpression(std::move(expectedAssignStatement->expr), std::move(actualAssignStatement->expr));
		return isSameVariable && isSameExpression;
	}
	else if (CAST_TO(IfStatement, expected) && CAST_TO(IfStatement, actual)) {
		auto expectedIfObject = CAST_TO(IfStatement, expected);
		auto actualIfObject = CAST_TO(IfStatement, actual);
		bool isSameCondition = checkIfSameCondition(std::move(expectedIfObject->condExpr), std::move(actualIfObject->condExpr));
		bool isSameThenStatements = checkIfSameStatementList(std::move(expectedIfObject->thenStmtList), std::move(actualIfObject->thenStmtList));
		bool isSameElseStatements = checkIfSameStatementList(std::move(expectedIfObject->elseStmtList), std::move(actualIfObject->elseStmtList));
		return isSameCondition && isSameThenStatements && isSameElseStatements;
	}
	else if (CAST_TO(WhileStatement, expected) && CAST_TO(WhileStatement, actual)) {
		auto expectedWhileObject = CAST_TO(WhileStatement, expected);
		auto actualWhileObject = CAST_TO(WhileStatement, actual);
		bool isSameCondition = checkIfSameCondition(std::move(expectedWhileObject->condExpr), std::move(actualWhileObject->condExpr));
		bool isSameStatements = checkIfSameStatementList(std::move(expectedWhileObject->stmtList), std::move(actualWhileObject->stmtList));
		return isSameCondition && isSameStatements;
	}
	else {
		return false;
	}
}

bool checkIfSameStatementList(std::unique_ptr<StatementList> expectedList, std::unique_ptr<StatementList> actualList) {
	// preliminary check for statement list
	if (expectedList->statements.size() != actualList->statements.size()) {
		return false;
	}

	for (int i = 0; i < expectedList->statements.size(); i++) {
		if (!checkIfSameStatement(std::move(expectedList->statements[i]), std::move(actualList->statements[i]))) {
			return false;
		}
	}
	return true;
}

bool checkIfSameTree(std::unique_ptr<Program> expectedRoot, std::unique_ptr<Program> actualRoot) {
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
		if (expectedProcedures[i]->procedureName != actualProcedures[i]->procedureName) {
			return false;
		}
		if (!checkIfSameStatementList(std::move(expectedStatements), std::move(actualStatements))) {
			return false;
		}
	}
	return true;
}