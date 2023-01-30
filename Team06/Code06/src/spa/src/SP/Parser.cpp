#include "Parser.h"

Parser::Parser() {

}

std::unique_ptr<Program> Parser::parseProgram(std::deque<Token> tokens) {
	// Rule: procedure+
	std::unique_ptr<Program> program = std::make_unique<Program>();

	while (tokens.front().type != TokenType::ENDOFFILE) {
		program->procedureList.push_back(parseProcedure(tokens));
	}

	if (program->procedureList.size() == 0) {
		std::cerr << "Program should contain at least one procedure" << std::endl;
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

	if (statementList->statements.size() == 0) {
		std::cerr << "Statement List should contain at least one statement" << std::endl;
	}

	return statementList;
}

std::unique_ptr<Statement> Parser::parseStatement(std::deque<Token>& tokens) {
	// Rule: read | print | call | while | if | assign
	if (tokens.front().type == TokenType::NAME) {
		if (tokens.front().value == "read") {
			auto readStatement = parseReadStatement(tokens);
			return readStatement;
		} else if (tokens.front().value == "print") {
			auto printStatement = parsePrintStatement(tokens);
			return printStatement;
		} else if (tokens.front().value == "call") {
			auto callStatement = parseCallStatement(tokens);
			return callStatement;
		} else if (tokens.front().value == "while") {
			auto whileStatement = parseWhileStatement(tokens);
			return whileStatement;
		} else if (tokens.front().value == "if") {
			auto ifStatement = parseIfStatement(tokens);
			return ifStatement;
		} else { // Assign statement
			auto assignStatement = std::make_unique<AssignStatement>();
			return assignStatement;
		}
	}
	else {
		std::cerr << "Unexpected token when parsing statement" << tokens.front().value << std::endl;
		return std::make_unique<ReadStatement>(); // To be removed
	}
}

std::unique_ptr<ReadStatement> Parser::parseReadStatement(std::deque<Token>& tokens) {
	// Rule: 'read' var_name';'
	std::unique_ptr<ReadStatement> readStatement = std::make_unique<ReadStatement>();

	tokens.pop_front(); // Pop 'read'

	if (tokens.front().type != TokenType::NAME) {
		std::cerr << "Expected var_name in read statement" << std::endl;
	}

	readStatement->varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name

	if (tokens.front().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ; at end of read statement" << std::endl;
	}
	tokens.pop_front(); // Pop ;

	return readStatement;
}

std::unique_ptr<PrintStatement> Parser::parsePrintStatement(std::deque<Token>& tokens) {
	// Rule: 'print' var_name';'
	std::unique_ptr<PrintStatement> printStatement = std::make_unique<PrintStatement>();

	tokens.pop_front(); // Pop 'print'

	if (tokens.front().type != TokenType::NAME) {
		std::cerr << "Expected var_name in print statement" << std::endl;
	}

	printStatement->varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name

	if (tokens.front().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ; at end of print statement" << std::endl;
	}
	tokens.pop_front(); // Pop ;

	return printStatement;
}

std::unique_ptr<CallStatement> Parser::parseCallStatement(std::deque<Token>& tokens) {
	// Rule: 'call' proc_name';'
	std::unique_ptr<CallStatement> callStatement = std::make_unique<CallStatement>();

	tokens.pop_front(); // Pop 'call'

	if (tokens.front().type != TokenType::NAME) {
		std::cerr << "Expected proc_name in call statement" << std::endl;
	}

	callStatement->procName = tokens.front().value;
	tokens.pop_front(); // Pop proc_name

	if (tokens.front().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ; at end of call statement" << std::endl;
	}
	tokens.pop_front(); // Pop ;

	return callStatement;
}

std::unique_ptr<WhileStatement> Parser::parseWhileStatement(std::deque<Token>& tokens) {
	// Rule: 'while' '(' cond_expr ')' '{' stmtLst '}'
	std::unique_ptr<WhileStatement> whileStatement = std::make_unique<WhileStatement>();

	tokens.pop_front(); // Pop 'while'

	if (tokens.front().type != TokenType::LEFT_PARENTHESIS) {
		std::cerr << "Expected '(' in while statement" << std::endl;
	}
	tokens.pop_front(); // Pop '('

	// Parse conditional expr
	whileStatement->condExpr = parseConditionalExpression(tokens);

	if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
		std::cerr << "Expected ')' in while statement" << std::endl;
	}
	tokens.pop_front(); // Pop ')'

	if (tokens.front().type != TokenType::LEFT_BRACE) {
		std::cerr << "Expected '{' in while statement" << std::endl;
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	whileStatement->stmtList = parseStatementList(tokens);

	if (tokens.front().type != TokenType::RIGHT_BRACE) {
		std::cerr << "Expected '}' in while statement" << std::endl;
	}
	tokens.pop_front(); // Pop '}'

	return whileStatement;
}

std::unique_ptr<IfStatement> Parser::parseIfStatement(std::deque<Token>& tokens) {
	// Rule: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
	std::unique_ptr<IfStatement> ifStatement = std::make_unique<IfStatement>();

	tokens.pop_front(); // Pop 'if'

	if (tokens.front().type != TokenType::LEFT_PARENTHESIS) {
		std::cerr << "Expected '(' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop '('

	// Parse conditional expr
	ifStatement->condExpr = parseConditionalExpression(tokens);

	if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
		std::cerr << "Expected ')' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop ')'

	if (tokens.front().type != TokenType::LEFT_BRACE) {
		std::cerr << "Expected '{' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	ifStatement->thenStmtList = parseStatementList(tokens);

	if (tokens.front().type != TokenType::RIGHT_BRACE) {
		std::cerr << "Expected '}' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop '}'

	if (tokens.front().type != TokenType::NAME || tokens.front().value != "else") {
		std::cerr << "Expected 'else' in if statement" << std::endl;
	}

	if (tokens.front().type != TokenType::LEFT_BRACE) {
		std::cerr << "Expected '{' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop '{'

	// Parse stmtList
	ifStatement->elseStmtList = parseStatementList(tokens);

	if (tokens.front().type != TokenType::RIGHT_BRACE) {
		std::cerr << "Expected '}' in if statement" << std::endl;
	}
	tokens.pop_front(); // Pop '}'

	return ifStatement;
}

std::unique_ptr<AssignStatement> Parser::parseAssignStatement(std::deque<Token>& tokens) {
	// Rule: var_name '=' expr ';'
	std::unique_ptr<AssignStatement> assignStatement = std::make_unique<AssignStatement>();

	assignStatement->varName = tokens.front().value;
	tokens.pop_front(); // Pop var_name

	if (tokens.front().type != TokenType::ASSIGN) {
		std::cerr << "Expected \"=\" in assign statement" << std::endl;
	}
	tokens.pop_front(); // Pop "="

	assignStatement->expr = parseExpression(tokens);

	if (tokens.front().type != TokenType::SEMICOLON) {
		std::cerr << "Expected ; at end of assign statement" << std::endl;
	}
	tokens.pop_front(); // Pop ;

	return assignStatement;
}

std::unique_ptr<ConditionalExpression> Parser::parseConditionalExpression(std::deque<Token>& tokens) {
	// Rule: rel_expr | '!' '(' cond_expr ')' | '(' cond_expr ')' '&&' '(' cond_expr ')' | '(' cond_expr ')' '||' '(' cond_expr ')'
	if (tokens.front().type == TokenType::NOT) { // '!' '(' path
		std::unique_ptr<ConditionalExpression> conditionalExpression;
		tokens.pop_front(); // Pop '!'
		if (tokens.front().type != TokenType::LEFT_PARENTHESIS) {
			std::cerr << "Expecting ( in conditional expression" << std::endl;
		}
		tokens.pop_front(); // Pop '('
		//conditionalExpression->
		if (tokens.front().type != TokenType::RIGHT_PARENTHESIS) {
			std::cerr << "Expecting ( in conditional expression" << std::endl;
		}
		tokens.pop_front(); // Pop ')'

	} else if (tokens.front().type == TokenType::LEFT_PARENTHESIS) {

	}
	
	return std::make_unique<ConditionalExpression>();
}

std::unique_ptr<Expression> Parser::parseExpression(std::deque<Token>& tokens) {
	// Rule: expr: expr '+' term | expr '-' term | term
	return std::make_unique<ConditionalExpression>(); // To be removed
}
