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
#include "./AST/Node/ReadStatement.h"
#include "./AST/Node/PrintStatement.h"
#include "./AST/Node/CallStatement.h"
#include "./AST/Node/WhileStatement.h"
#include "./AST/Node/IfStatement.h"
#include "./AST/Node/AssignStatement.h"
#include "./AST/Node/Expression.h"
#include "./AST/Node/ConditionalExpression.h"

class Parser {
private:
	std::unique_ptr<Procedure> parseProcedure(std::deque<Token>& tokens);
	std::unique_ptr<StatementList> parseStatementList(std::deque<Token>& tokens);
	std::unique_ptr<Statement> parseStatement(std::deque<Token>& tokens);
	std::unique_ptr<ReadStatement> parseReadStatement(std::deque<Token>& tokens);
	std::unique_ptr<PrintStatement> parsePrintStatement(std::deque<Token>& tokens);
	std::unique_ptr<CallStatement> parseCallStatement(std::deque<Token>& tokens);
	std::unique_ptr<WhileStatement> parseWhileStatement(std::deque<Token>& tokens);
	std::unique_ptr<IfStatement> parseIfStatement(std::deque<Token>& tokens);
	std::unique_ptr<AssignStatement> parseAssignStatement(std::deque<Token>& tokens);
	std::unique_ptr<ConditionalExpression> parseConditionalExpression(std::deque<Token>& tokens);
	std::unique_ptr<Expression> parseExpression(std::deque<Token>& tokens);

public:
	Parser();
	std::unique_ptr<Program> parseProgram(std::deque<Token> tokens);
};