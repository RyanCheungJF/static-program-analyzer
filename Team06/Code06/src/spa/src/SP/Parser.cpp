#include "Parser.h"

Parser::Parser() {

}

std::unique_ptr<Program> Parser::parseProgram(std::deque<Token> tokens) {
	// Rule: procedure+
	std::unique_ptr<Program> program = std::make_unique<Program>();

	while (tokens.front().type != TokenType::ENDOFFILE) {
		program->procedureList.push_back(parseProcedure(tokens));
	}

	return program;
}

std::unique_ptr<Procedure> Parser::parseProcedure(std::deque<Token>& tokens) {
	// Rule: 'procedure' proc_name '{' stmtLst '}'
	std::unique_ptr<Procedure> procedure = std::make_unique<Procedure>();

	if (tokens.front().type != TokenType::NAME && tokens.front().value != "procedure") {
		std::cerr << "Expected 'procedure' keyword, but got: " << tokens.front().value << std::endl;
	}
	tokens.pop_front();

	if (tokens.front().type != TokenType::NAME) {
		std::cerr << "Expected 'proc_name', but got: " << tokens.front().value << std::endl;
	}
	std::string procedureName = tokens.front().value;
	std::cout << procedureName << std::endl;
	tokens.pop_front();

	if (tokens.front().type != TokenType::LEFT_BRACE) {
		std::cerr << "Expected '{', but got: " << tokens.front().value << std::endl;
	}
	tokens.pop_front();

	procedure->statementList = parseStatementList(tokens);

	if (tokens.front().type != TokenType::RIGHT_BRACE) {
		std::cerr << "Expected '}', but got: " << tokens.front().value << std::endl;
	}
	tokens.pop_front();

	return procedure;
}

std::unique_ptr<StatementList> Parser::parseStatementList(std::deque<Token>& tokens) {
	// Rule: stmt+
	std::unique_ptr<StatementList> statementList = std::make_unique<StatementList>();

	while (tokens.front().type != TokenType::RIGHT_BRACE) {
		statementList->statements.push_back(parseStatement(tokens));
	}

	return statementList;
}

std::unique_ptr<Statement> Parser::parseStatement(std::deque<Token>& tokens) {

	std::unique_ptr<Statement> statement = std::make_unique<Statement>();

	while (tokens.front().type != TokenType::RIGHT_BRACE) {
		tokens.pop_front();
	}
	tokens.pop_front();

	return statement;
}
