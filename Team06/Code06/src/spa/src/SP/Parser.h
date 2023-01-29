#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <deque>
#include <Token.h>
#include "./AST/Node/Program.h"
#include "./AST/Node/Procedure.h"
#include "./AST/Node/StatementList.h"
#include "./AST/Node/Statement.h"

class Parser {
private:
	std::unique_ptr<Procedure> parseProcedure(std::deque<Token>& tokens);
	std::unique_ptr<StatementList> parseStatementList(std::deque<Token>& tokens);
	std::unique_ptr<Statement> parseStatement(std::deque<Token>& tokens);

public:
	Parser();

	std::unique_ptr<Program> parseProgram(std::deque<Token> tokens);
};