#include <iostream>
#include "catch.hpp"
#include "../SP/Tokenizer.h"
#include "../SP/Parser.h"
#include "../SP/AST/Node/Procedure.h"
#include "../SP/SPExceptions.h"

// recursive helper functions
bool checkIfSameExpression(std::unique_ptr<Expression> expectedExpression, std::unique_ptr<Expression> actualExpression);
bool checkIfSameCondition(std::unique_ptr<ConditionalExpression> expectedExpression, std::unique_ptr<ConditionalExpression> actualExpression);
bool checkIfSameStatement(std::unique_ptr<Statement> expectedStatement, std::unique_ptr<Statement> actualStatement);
bool checkIfSameStatementList(std::unique_ptr<StatementList> expectedList, std::unique_ptr<StatementList> actualList);
bool checkIfSameTree(std::unique_ptr<Program> expectedRoot, std::unique_ptr<Program> actualRoot);

bool checkIfSameExpression(std::unique_ptr<Expression> expectedExpression, std::unique_ptr<Expression> actualExpression) {
	auto expected = expectedExpression.get();
	auto actual = actualExpression.get();

	if (dynamic_cast<Constant*>(expected) && dynamic_cast<Constant*>(actual)) {
		std::cout << "LOG: Comparing Constants" << std::endl;
		auto expectedValue = dynamic_cast<Constant*>(expected)->value;
		auto actualValue = dynamic_cast<Constant*>(actual)->value;
		return expectedValue == actualValue;
	}
	else if (dynamic_cast<Variable*>(expected) && dynamic_cast<Variable*> (actual)) {
		std::cout << "LOG: Comparing Variables" << std::endl;
		auto expectedName = dynamic_cast<Variable*>(expected)->name;
		auto actualName = dynamic_cast<Variable*>(actual)->name;
		return expectedName == actualName;
	}
	else if (dynamic_cast<MathExpression*>(expected) && dynamic_cast<MathExpression*>(actual)) {
		std::cout << "LOG: Comparing Math Expressions" << std::endl;
		auto expectedExpression = dynamic_cast<MathExpression*>(expected);
		auto actualExpression = dynamic_cast<MathExpression*>(actual);
		bool isSameLeftExpression=  checkIfSameExpression(std::move(expectedExpression->lhs), std::move(actualExpression->lhs));
		bool isSameRightExpression = checkIfSameExpression(std::move(expectedExpression->rhs), std::move(actualExpression->rhs));
		bool isSameOp = (expectedExpression->mathOperator == actualExpression->mathOperator);
		return (isSameLeftExpression && isSameRightExpression && isSameOp);
	}
	else {
		std::cout << "LOG: Not a valid Expression Type!" << std::endl;
	}
	return false;
}

bool checkIfSameCondition(std::unique_ptr<ConditionalExpression> expectedExpression, std::unique_ptr<ConditionalExpression> actualExpression) {
	auto expected = expectedExpression.get();
	auto actual = actualExpression.get();

	if (dynamic_cast<NotConditionalExpression*>(expected) && dynamic_cast<NotConditionalExpression*>(actual)) {
		std::cout << "LOG: Comparing Not Conditionals" << std::endl;
		auto expectedCondition = std::move(dynamic_cast<NotConditionalExpression*>(expected)->condExpr);
		auto actualCondition = std::move(dynamic_cast<NotConditionalExpression*>(actual)->condExpr);
		return checkIfSameCondition(std::move(expectedCondition), std::move(actualCondition));
	}
	else if (dynamic_cast<BinaryConditionalExpression*>(expected) && dynamic_cast<BinaryConditionalExpression*>(actual)) {
		std::cout << "LOG: Comparing Binary Conditionals" << std::endl;
		auto expectedCondition = dynamic_cast<BinaryConditionalExpression*>(expected);
		auto actualCondition = dynamic_cast<BinaryConditionalExpression*>(actual);
		bool isSameLeftConditional =  checkIfSameCondition(std::move(expectedCondition->lhs), std::move(actualCondition->lhs));
		bool isSameRightConditional = checkIfSameCondition(std::move(expectedCondition->rhs), std::move(actualCondition->rhs));
		bool isSameOp = (expectedCondition->condOperator == actualCondition->condOperator);
		return (isSameLeftConditional && isSameRightConditional && isSameOp);
		
	}
	else if (dynamic_cast<RelationalExpression*>(expected) && dynamic_cast<RelationalExpression*>(actual)) {
		std::cout << "LOG: Comparing Relational Expressions" << std::endl;
		auto expectedCondition = dynamic_cast<RelationalExpression*>(expected);
		auto actualCondition = dynamic_cast<RelationalExpression*>(actual);
		bool isSameLeftExpression =  checkIfSameExpression(std::move(expectedCondition->lhs), std::move(actualCondition->lhs));
		bool isSameRightExpression = checkIfSameExpression(std::move(expectedCondition->rhs), std::move(actualCondition->rhs));
		bool isSameOp = (expectedCondition->relationalOperator == actualCondition->relationalOperator);
		return (isSameLeftExpression && isSameRightExpression && isSameOp);
	}
	else {
		std::cout << "LOG: Not a valid Conditional Type!" << std::endl;
	}
	return false;
}

bool checkIfSameStatement(std::unique_ptr<Statement> expectedStatement, std::unique_ptr<Statement> actualStatement) {
	auto expected = expectedStatement.get();
	auto actual = actualStatement.get();
	
	if (dynamic_cast<PrintStatement*>(expected) && dynamic_cast<PrintStatement*>(actual)) {
		std::cout << "LOG: Comparing Print Statements" << std::endl;
		return dynamic_cast<PrintStatement*>(expected)->varName == dynamic_cast<PrintStatement*>(actual)->varName;
	}
	else if (dynamic_cast<ReadStatement*>(expected) && dynamic_cast<ReadStatement*>(actual)) {
		std::cout << "LOG: Comparing Read Statements" << std::endl;
		return dynamic_cast<ReadStatement*>(expected)->varName == dynamic_cast<ReadStatement*>(actual)->varName;
	}
	else if (dynamic_cast<CallStatement*>(expected) && dynamic_cast<CallStatement*>(actual)) {
		std::cout << "LOG: Comparing Call Statements" << std::endl;
		return dynamic_cast<CallStatement*>(expected)->procName == dynamic_cast<CallStatement*>(actual)->procName;
	}
	else if (dynamic_cast<AssignStatement*>(expected) && dynamic_cast<AssignStatement*>(actual)) {
		std::cout << "LOG: Comparing Assign Statements" << std::endl;
		auto expectedAssignStatement = dynamic_cast<AssignStatement*>(expected);
		auto actualAssignStatement = dynamic_cast<AssignStatement*>(actual);
		bool isSameVariable = (expectedAssignStatement->varName == actualAssignStatement->varName);
		bool isSameExpression = checkIfSameExpression(std::move(expectedAssignStatement->expr), std::move(actualAssignStatement->expr));
		std::cout << "LOG: Comparing Assign Statements Same expr " << isSameExpression << std::endl;
		return isSameVariable && isSameExpression;
	}
	else if (dynamic_cast<IfStatement*>(expected) && dynamic_cast<IfStatement*>(actual)) {
		std::cout << "LOG: Comparing If Statements" << std::endl;
		auto expectedIfObject = dynamic_cast<IfStatement*>(expected);
		auto actualIfObject = dynamic_cast<IfStatement*>(actual);
		bool isSameCondition = checkIfSameCondition(std::move(expectedIfObject->condExpr), std::move(actualIfObject->condExpr));
		bool isSameThenStatements = checkIfSameStatementList(std::move(expectedIfObject->thenStmtList), std::move(actualIfObject->thenStmtList));
		bool isSameElseStatements = checkIfSameStatementList(std::move(expectedIfObject->elseStmtList), std::move(actualIfObject->elseStmtList));
		return isSameCondition && isSameThenStatements && isSameElseStatements;
	}
	else if (dynamic_cast<WhileStatement*>(expected) && dynamic_cast<WhileStatement*>(actual)) {
		std::cout << "LOG: Comparing While Statements" << std::endl;
		auto expectedWhileObject = dynamic_cast<WhileStatement*>(expected);
		auto actualWhileObject = dynamic_cast<WhileStatement*>(actual);
		bool isSameCondition = checkIfSameCondition(std::move(expectedWhileObject->condExpr), std::move(actualWhileObject->condExpr));
		bool isSameStatements = checkIfSameStatementList(std::move(expectedWhileObject->stmtList), std::move(actualWhileObject->stmtList));
		return isSameCondition && isSameStatements;
	}
	else {
		std::cout << "LOG: Not a valid Statement Type!" << std::endl;
		return false;
	}
}

bool checkIfSameStatementList(std::unique_ptr<StatementList> expectedList, std::unique_ptr<StatementList> actualList) {
	// preliminary check for statement list
	if (expectedList->statements.size() != actualList->statements.size()) {
		std::cout << "LOG: Trees differ in number of statements" << std::endl;
		return false;
	}

	for (int i = 0; i < expectedList->statements.size(); i++) {
		if (!checkIfSameStatement(std::move(expectedList->statements[i]), std::move(actualList->statements[i]))) {
			std::cout << "LOG: Mismatch in statements!" << std::endl;
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
		std::cout << "LOG: Trees differ in number of procedures" << std::endl;
		return false;
	}

	// loop thru procedures
	for (int i = 0; i < expectedProcedures.size(); i++) {
		auto expectedStatements = std::move(expectedProcedures[i]->statementList);
		auto actualStatements = std::move(actualProcedures[i]->statementList);
		std::cout << "LOG: Checking Procedure " << expectedProcedures[i]->procedureName << std::endl;
		if (!checkIfSameStatementList(std::move(expectedStatements), std::move(actualStatements))) {
			return false;
		}
	}
	return true;
}


TEST_CASE("Valid Source Program") {
	std::stringstream strStream;
	Tokenizer testTokenizer;
	Parser testParser;
	std::deque<Token> tokenQueue;
	std::string testDirectory = "../../../../../../Tests06/sp/parser/";

	SECTION("Simple Valid Program") {
		std::ifstream testFile(testDirectory + "valid1.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		auto rootNode = testParser.parseProgram(tokenQueue);
		
		// creating expected tree
		std::unique_ptr<Statement> printNode = std::make_unique<PrintStatement>("z");
		std::unique_ptr<Expression> expressionNode = std::make_unique<Constant>(1);
		std::unique_ptr<Statement> assignNode = std::make_unique<AssignStatement>("x", std::move(expressionNode));
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
		// proc A
		std::unique_ptr<Expression> mathNodeA = std::make_unique<MathExpression>("+", std::make_unique<Variable>("x"), std::make_unique<Constant>(1));
		std::unique_ptr<Statement> assignNodeA = std::make_unique<AssignStatement>("x", std::move(mathNodeA));
		std::vector<std::unique_ptr<Statement>> statementsA;
		statementsA.push_back(std::move(assignNodeA));
		auto statementListNodeA = std::make_unique<StatementList>(std::move(statementsA));
		auto procedureNodeA = std::make_unique<Procedure>("A", std::move(statementListNodeA));

		// proc B
		std::unique_ptr<Expression> mathNodeB = std::make_unique<MathExpression>("*", std::make_unique<Variable>("y"), std::make_unique<Constant>(2));
		std::unique_ptr<Statement> assignNodeB = std::make_unique<AssignStatement>("y", std::move(mathNodeB));
		std::unique_ptr<Statement> readNodeB = std::make_unique<ReadStatement>("y");
		std::vector<std::unique_ptr<Statement>> statementsB;    
		statementsB.push_back(std::move(assignNodeB));
		statementsB.push_back(std::move(readNodeB));
		auto statementListNodeB = std::make_unique<StatementList>(std::move(statementsB));
		auto procedureNodeB = std::make_unique<Procedure>("B", std::move(statementListNodeB));

		// proc C
		std::unique_ptr<Expression> mathNodeC = std::make_unique<MathExpression>("-", std::make_unique<Variable>("z"), std::make_unique<Constant>(3));
		std::unique_ptr<Statement> assignNodeC = std::make_unique<AssignStatement>("z", std::move(mathNodeC));
		std::unique_ptr<Statement> printNodeC = std::make_unique<PrintStatement>("z");
		std::unique_ptr<Statement> callNodeC = std::make_unique<CallStatement>("B");
		std::vector<std::unique_ptr<Statement>> statementsC;
		statementsC.push_back(std::move(printNodeC));
		statementsC.push_back(std::move(callNodeC));
		statementsC.push_back(std::move(assignNodeC));
		auto statementListNodeC = std::make_unique<StatementList>(std::move(statementsC));
		auto procedureNodeC = std::make_unique<Procedure>("C", std::move(statementListNodeC));

		// program
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
		std::unique_ptr<Statement> assignNodeX = std::make_unique<AssignStatement>("x", std::make_unique<Constant>(6));
		std::unique_ptr<Statement> assignNodeY = std::make_unique<AssignStatement>("y", std::make_unique<Constant>(1));
		std::unique_ptr<Statement> assignNodeZ = std::make_unique<AssignStatement>("z", std::make_unique<Variable>("y"));


		// outer scope
		REQUIRE(rootNode->procedureList[0]->statementList->statements.size() == 5);
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