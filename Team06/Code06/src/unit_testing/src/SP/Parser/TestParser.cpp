#include <iostream>
#include "catch.hpp"
#include "../SP/Tokenizer.h"
#include "../SP/Parser.h"
#include "../SP/AST/Node/Procedure.h"
#include "../SP/SPExceptions.h"

bool checkIfSameExpression(std::unique_ptr<Expression> expectedExpression, std::unique_ptr<Expression> actualExpression) {
	auto expected = expectedExpression.get();
	auto actual = actualExpression.get();

	if (dynamic_cast<Constant*>(expected) && dynamic_cast<Constant*>(actual)) {
		std::cout << "LOG: Comparing constants" << std::endl;
		auto expectedValue = dynamic_cast<Constant*>(expected)->value;
		auto actualValue = dynamic_cast<Constant*>(actual)->value;
		return expectedValue == actualValue;
	}
	else if (dynamic_cast<Variable*>(expected) && dynamic_cast<Variable*> (actual)) {
		std::cout << "LOG: Comparing variables" << std::endl;
		auto expectedName = dynamic_cast<Variable*>(expected)->name;
		auto actualName = dynamic_cast<Variable*>(actual)->name;
		return expectedName == actualName;
	}
	else if (dynamic_cast<MathExpression*>(expected) && dynamic_cast<MathExpression*>(actual)) {
		std::cout << "LOG: Comparing Math Expressions" << std::endl;
		/*auto expectedExpression = dynamic_cast<MathExpression*>(expected);
		auto actualCondition = dynamic_cast<MathExpression*>(actual);
		// put into unique pointers
		bool isSameLeftExpression=  checkIfSameExpression(, );
		bool isSameRightExpression =  checkIfSameExpression(, );
		bool isSameOp = // check ops
		return (isSameLeftConditional && isSameRightConditional && isSameOp);
		*/
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
		/*auto expectedCondition = dynamic_cast<NotConditionalExpression*>(expected)->condExpr;
		auto actualCondition = dynamic_cast<NotConditionalExpression*>(actual)->condExpr;
		// put into unique pointers
		return checkIfSameCondition(, );
		*/
	}
	else if (dynamic_cast<BinaryConditionalExpression*>(expected) && dynamic_cast<BinaryConditionalExpression*>(actual)) {
		std::cout << "LOG: Comparing Binary Conditionals" << std::endl;
		/*auto expectedCondition = dynamic_cast<BinaryConditionalExpression*>(expected);
		auto actualCondition = dynamic_cast<BinaryConditionalExpressiont*>(actual);
		// put into unique pointers
		bool isSameLeftConditional =  checkIfSameCondition(, );
		bool isSameRightConditional =  checkIfSameCondition(, );
		bool isSameOp = // check ops
		return (isSameLeftConditional && isSameRightConditional && isSameOp);
		*/
	}
	else if (dynamic_cast<RelationalExpression*>(expected) && dynamic_cast<RelationalExpression*>(actual)) {
		std::cout << "LOG: Comparing Relational Expressions" << std::endl;
		/*auto expectedCondition = dynamic_cast<RelationalExpression*>(expected);
		auto actualCondition = dynamic_cast<RelationalExpressiont*>(actual);
		// put into unique pointers
		bool isSameLeftExpression =  checkIfSameExpression(, );
		bool isSameRightExpression =  checkIfSameExpression(, );
		bool isSameOp = // check ops
		return (isSameLeftConditional && isSameRightConditional && isSameOp);
		*/
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
		/*auto expectedVar = dynamic_cast<PrintStatement*>(expected)->varName;
		auto actualVar = dynamic_cast<PrintStatement*>(actual)->varName;
		std::cout << "Expected: " << expectedVar << std::endl;
		std::cout << "Actual: " << actualVar << std::endl;
		return expectedVar == actualVar;*/
		return false;
	}
	else if (dynamic_cast<ReadStatement*>(expected) && dynamic_cast<ReadStatement*>(actual)) {
		std::cout << "LOG: Comparing Read Statements" << std::endl;
		return static_cast<ReadStatement*>(expected)->varName == static_cast<ReadStatement*>(actual)->varName;
	}
	else if (dynamic_cast<CallStatement*>(expected) && dynamic_cast<CallStatement*>(actual)) {
		std::cout << "LOG: Comparing Call Statements" << std::endl;
		return static_cast<CallStatement*>(expected)->procName == static_cast<CallStatement*>(actual)->procName;
	}
	else if (dynamic_cast<AssignStatement*>(expected) && dynamic_cast<AssignStatement*>(actual)) {
		std::cout << "LOG: Comparing Assign Statements" << std::endl;
		return (static_cast<AssignStatement*>(expected)->varName == static_cast<AssignStatement*>(actual)->varName);
	}
	else if (dynamic_cast<IfStatement*>(expected) && dynamic_cast<IfStatement*>(actual)) {
		std::cout << "LOG: Comparing If Statements" << std::endl;
		auto expectedIfObject = static_cast<IfStatement*>(expected);
		auto actualIfObject = static_cast<IfStatement*>(actual);
		//bool isSameCondition = checkIfSameCondition(, );
	}
	else if (dynamic_cast<WhileStatement*>(expected) && dynamic_cast<WhileStatement*>(actual)) {
		std::cout << "LOG: Comparing While Statements" << std::endl;
		auto expectedIfObject = static_cast<IfStatement*>(expected);
		auto actualIfObject = static_cast<IfStatement*>(actual);
		//bool isSameCondition = checkIfSameCondition(, );
	}
	else {
		std::cout << "LOG: Not a valid Statement Type!" << std::endl;
		return false;
	}
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
		auto expectedStatements = std::move(expectedProcedures[i]->statementList->statements);
		auto actualStatements = std::move(actualProcedures[i]->statementList->statements);

	// preliminary check for statementlist
		if (expectedStatements.size() != actualStatements.size()) {
			std::cout << "LOG: Trees differ in number of statements" << std::endl;
			return false;
		}

	// recurse each statement tree to check if they are the same
		for (int j = 0; j < expectedStatements.size(); j++) {
			if (!checkIfSameStatement(std::move(expectedStatements[j]), std::move(actualStatements[j]))) {
				std::cout << "LOG: Mismatch in statements!" << std::endl;
				return false;
			}
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

		//REQUIRE(checkIfSameTree(std::move(rootNode), std::move(programNode)));
		REQUIRE(rootNode->procedureList.size() == 1);
		REQUIRE(rootNode->procedureList.front()->procedureName == "A");
		REQUIRE(rootNode->procedureList.front()->statementList->statements.size() == 2);
	}

	SECTION("Valid Program With Multiple Procedures") {
		std::ifstream testFile(testDirectory + "valid2.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		auto rootNode = testParser.parseProgram(tokenQueue);

		// creating actual tree

		
		REQUIRE(rootNode->procedureList.size() == 3);
		// check proc A
		REQUIRE(rootNode->procedureList[0]->procedureName == "A");
		REQUIRE(rootNode->procedureList[0]->statementList->statements.size() == 1);
		// check proc C
		REQUIRE(rootNode->procedureList[2]->statementList->statements.size() == 3);
	}

	SECTION("Valid Program With Conditionals") {
		std::ifstream testFile(testDirectory + "valid3.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		auto rootNode = testParser.parseProgram(tokenQueue);

		// creating actual tree

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