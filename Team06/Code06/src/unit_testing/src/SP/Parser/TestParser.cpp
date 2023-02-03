#include "catch.hpp"
#include "../SP/Tokenizer.h"
#include "../SP/Parser.h"
#include "../SP/AST/Node/Procedure.h"
#include "../SP/SPExceptions.h"


bool checkIfSameStatement(std::unique_ptr<Statement> expectedStatement, std::unique_ptr<Statement> actualStatement) {
	auto expected = expectedStatement.get();
	auto actual = actualStatement.get();
	
	if (dynamic_cast<PrintStatement*>(expected) && dynamic_cast<PrintStatement*>(actual)) {
		return static_cast<PrintStatement*>(expected)->varName == static_cast<PrintStatement*>(actual)->varName;
	}
	else if (dynamic_cast<ReadStatement*>(expected) && dynamic_cast<ReadStatement*>(actual)) {
		return static_cast<ReadStatement*>(expected)->varName == static_cast<ReadStatement*>(actual)->varName;
	}
	else if (dynamic_cast<CallStatement*>(expected) && dynamic_cast<CallStatement*>(actual)) {
		return static_cast<CallStatement*>(expected)->procName == static_cast<CallStatement*>(actual)->procName;
	}
	else {
		return false;
	}
	return false;
}

bool checkIfSameTree(std::unique_ptr<Program> expectedRoot, std::unique_ptr<Program> actualRoot) {
	auto actualProcedures = std::move(actualRoot.get()->procedureList);
	auto expectedProcedures = std::move(expectedRoot.get()->procedureList);

	// preliminary check for procedurelist
	if (expectedProcedures.size() != actualProcedures.size()) {
		std::cout << "LOG: Trees differ in number of procedures" << std::endl;
		return false;
	}

	// loop thru procedures
	for (int i = 0; i < expectedProcedures.size(); i++) {
		auto expectedStatements = std::move(expectedProcedures[i].get()->statementList.get()->statements);
		auto actualStatements = std::move(actualProcedures[i].get()->statementList.get()->statements);

	// preliminary check for statementlist
		if (expectedStatements.size() != actualStatements.size()) {
			std::cout << "LOG: Trees differ in number of statements" << std::endl;
			return false;
		}

	// recurse each statement tree to check if they are the same
		for (int j = 0; j < expectedStatements.size(); j++) {
			if (!checkIfSameStatement(std::move(expectedStatements[j]), std::move(actualStatements[j]))) {
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

	SECTION("Sample Valid Program") {
		std::ifstream testFile(testDirectory + "valid1.txt");
		strStream << testFile.rdbuf();
		tokenQueue = testTokenizer.tokenize(strStream);

		auto rootNode = testParser.parseProgram(tokenQueue);
		
		auto printNode = std::make_unique<PrintStatement>();

		REQUIRE(rootNode.get()->procedureList.size() == 1);
		REQUIRE(rootNode.get()->procedureList.front().get()->procedureName == "A");
		REQUIRE(rootNode.get()->procedureList.front().get()->statementList.get()->statements.size() == 2);
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

		REQUIRE_THROWS_AS(testParser.parseProgram(tokenQueue), SyntaxErrorException);
	}

}