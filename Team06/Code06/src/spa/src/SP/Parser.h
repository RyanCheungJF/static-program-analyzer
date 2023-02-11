#pragma once

#include <deque>
#include <Token.h>
#include "SPExceptions.h"
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
#include "./AST/Node/NotConditionalExpression.h"
#include "./AST/Node/BinaryConditionalExpression.h"
#include "./AST/Node/RelationalExpression.h"
#include "./AST/Node/MathExpression.h"
#include "./AST/Node/Variable.h"
#include "./AST/Node/Constant.h"

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
	std::unique_ptr<ConditionalExpression> parseRelationalExpression(std::deque<Token>& tokens);
	std::unique_ptr<Expression> parseRelationalFactor(std::deque<Token>& tokens);
	std::unique_ptr<Expression> parseTerm(std::deque<Token>& tokens);
	std::unique_ptr<Expression> parseFactor(std::deque<Token>& tokens);
	std::unique_ptr<Expression> parseConstant(std::deque<Token>& tokens);
	std::unique_ptr<Expression> parseVariable(std::deque<Token>& tokens);
	bool isRelationalOperator(TokenType tt);

public:
	std::unique_ptr<Program> parseProgram(std::deque<Token> tokens);
	std::unique_ptr<Expression> parseExpression(std::deque<Token>& tokens); // Used by Pattern Storage
};